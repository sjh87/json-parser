#include "Parser.hpp"

namespace JSON {
    // stole this and tested it pretty thoroughly; guilty till proven innocent,
    // like all regex :-D
    const std::regex numberPattern(R"(^-?(?:0|[1-9]\d*)(?:\.\d+)?(?:[eE][+-]?\d+)?$)");

    static std::unique_ptr<ValueNodeBase> parsePrimitive(std::string& buffer) {
        if (buffer == "true" || buffer == "false") {
            return std::make_unique<BooleanNode>(buffer == "true");
        } else if (std::regex_match(buffer, numberPattern)) {
            double d = std::atof(buffer.data()); // TODO handle exceptions
            return std::make_unique<NumberNode>(d);
        } else if (buffer == "null") {
            return std::make_unique<NullNode>();
        } else if (buffer.front() == '"' && buffer.back() == '"') {
            buffer.erase(buffer.begin());
            buffer.erase(buffer.end() - 1); // iterators behave like pointers?

            // reassign buffer storage to new StringNode, clearing buffer and saving a copy operation (I think)
            return std::make_unique<StringNode>(std::move(buffer));
        }

        throw std::runtime_error("'"+ buffer +"'" + " is invalid JSON");
    }

    void Parser::collapseContainer(const Type type) {
        if (type != Type::Array && type != Type::Object)
            throw std::runtime_error("JSON::collapse called with non-container Type enum");

        std::stack<StackElement> temp;

        while (!stack.empty() && stack.top().value->getType() != type) {
            if (stack.top().open)
                throw std::runtime_error("cannot collapse open container into another");

            temp.push(std::move(stack.top()));
            stack.pop();
        }

        if (stack.empty())
            throw std::runtime_error("did not find beginning of container when collapsing");
        
        if (type == Type::Object) {
            auto objectPtr = static_cast<ObjectNode*>(stack.top().value.get());

            while (!temp.empty()) {
                auto tempTop = std::move(temp.top());
                if (!tempTop.key)
                    throw std::runtime_error("empty key encountered while collapsing Object");
                
                objectPtr->insert(std::move(tempTop.key), std::move(tempTop.value));

                temp.pop();
            }
        } else {
            auto arrayPtr = static_cast<ArrayNode*>(stack.top().value.get());

            while (!temp.empty()) {
                auto tempTop = std::move(temp.top());
                if (tempTop.key)
                    throw std::runtime_error("non-empty key encountered while collapsing Array");
                
                arrayPtr->insert(std::move(tempTop.value));
                
                temp.pop();
            }
        }

        stack.top().open = false;
    }

    void Parser::validateParserEndState(
        std::unique_ptr<ValueNodeBase>& head,
        std::string& parsingBuffer
    ) {
        if (!stack.empty()) {
            throw std::runtime_error("Parsing error: reached EOF and stack not empty");
        } else if (!head && parsingBuffer.empty()) {
            throw std::runtime_error("payload is empty");
        } else if (head && !parsingBuffer.empty()) {
            throw std::runtime_error("invalid JSON");
        }
    }

    bool Parser::canBeginObjectOrArray() const {
        if (stack.empty()) {
            return true;
        } else if (!stack.top().key && stack.top().value) {
            return true;
        } else if (stack.top().key && !stack.top().value) {
            return true;
        } else if (stack.top().value->getType() == Type::Array) {
            return true;
        } else if (stack.top().value->getType() == Type::Object && !stack.top().open)
            return false;

        return false;
    }

    // if prior stack element has a key and value, or its value is an Object
    // itself, return true
    bool Parser::readyForObjectKey() const {
        if (stack.top().key && stack.top().value)
            return true;

        return stack.top().value && stack.top().value->getType() == Type::Object;
    }

    bool Parser::expectingKey() const {
        return !stack.empty() // in a container, at least
            && stack.top().value // not waiting for a value to pair with a key
            && stack.top().value->getType() != Type::Array // just started an Array in an object
            && ( // just started an object, or last item was a complete key/v pair
                stack.top().value->getType() == Type::Object
                || (stack.top().key && stack.top().value)
            );
    }

    JSON Parser::parse(std::istream& stream) {
        char byte; // TODO figure out UTF-8 parsing, validation
        std::string parsingBuffer;
        std::unique_ptr<ValueNodeBase> head;

        while (true) {
            stream.get(byte);

            if (stream.eof()) {
                if (stack.size() == 1 && parsingBuffer.empty()) {
                    head = std::move(stack.top().value);
                    stack.pop();
                } else if (!parsingBuffer.empty() && stack.empty()) {
                    validateParserEndState(head, parsingBuffer);
                    head = std::move(parsePrimitive(parsingBuffer));
                } else {
                    break; // let the check after the while loop catch it
                }


                parsingBuffer.clear();
                break;
            } else if (stream.bad() || stream.fail()) {
                std::runtime_error("failed to read stream");
            }

            std::unique_ptr<ValueNodeBase> node;
            switch (byte) {
            case ',':
                if (stack.empty()) {
                    throw std::runtime_error("unexpected ',' encountered");
                }

                if (!parsingBuffer.empty()) {
                    node = std::move(parsePrimitive(parsingBuffer));
                    parsingBuffer.clear();
                }

                if (stack.top().key && !stack.top().value) {
                    stack.top().value = std::move(node);
                } else if (stack.top().value && stack.top().value->getType() != Type::Object) {
                    stack.push(StackElement{
                        std::unique_ptr<std::string>(nullptr),
                        std::move(node)
                    });
                }

                break;
            case '[':
                if (canBeginObjectOrArray()) {
                    node = std::make_unique<ArrayNode>();
                    if (stack.empty()) {
                        stack.push(StackElement{
                            std::move(std::unique_ptr<std::string>(nullptr)),
                            std::move(node),
                            true
                    });
                    } else if (stack.top().key) {
                        stack.top().value = std::move(node);
                        stack.top().open = true;
                    } else if (!stack.top().key) {
                        stack.push(StackElement{
                            std::move(std::unique_ptr<std::string>(nullptr)),
                            std::move(node),
                            true
                        });
                    } else if (stack.top().value->getType() == Type::Array) {
                        stack.push(StackElement{
                            std::move(std::unique_ptr<std::string>(nullptr)),
                            std::move(node),
                            true
                        });
                    }
                }
                break;
            case ']':
                if (stack.empty()) {
                    throw std::runtime_error("unexpected ']' encountered");
                }

                if (
                    !parsingBuffer.empty()
                    && stack.top().value
                    && (stack.top().value->getType() == Type::Array || !stack.top().key)
                ) {
                    auto ptr = parsePrimitive(parsingBuffer);
                    stack.push(StackElement{
                        std::move(std::unique_ptr<std::string>(nullptr)),
                        std::move(ptr)
                    });
                    parsingBuffer.clear();
                }

                collapseContainer(Type::Array);
                break;
            case '{':
                if (canBeginObjectOrArray()) {
                    node = std::make_unique<ObjectNode>();
                    if (stack.empty() || !stack.top().key || stack.top().value->getType() == Type::Array) {
                        stack.push(StackElement{
                            std::move(std::unique_ptr<std::string>(nullptr)),
                            std::move(node),
                            true
                        });
                    } else if (stack.top().key) {
                        stack.top().value = std::move(node);
                        stack.top().open = true;
                    }
                } else {
                    throw std::runtime_error("unexpected '{' encountered");
                }
                break;
            case '}':
                if (stack.empty()) {
                    throw std::runtime_error("unexpected '}' encountered");
                }

                if (!parsingBuffer.empty() && stack.top().key && !stack.top().value) {
                    auto ptr = parsePrimitive(parsingBuffer);
                    stack.top().value = std::move(ptr);
                    parsingBuffer.clear();
                }

                collapseContainer(Type::Object);
                break;
            case ':':
                if (stack.empty() || head || !stack.top().key || stack.top().value) {
                    throw std::runtime_error("':' encountered outside of object");
                }
                break;
            case '-':
            case '.':
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
            case 'a':
            case 'e':
            case 'E':
            case 'f':
            case 'l':
            case 'n':
            case 'r':
            case 's':
            case 't':
            case 'u':
                if (expectingKey())
                    throw std::runtime_error("unexpected '"+ std::string(1, byte) +"' when key was expected");
                parsingBuffer.push_back(byte);
                break;
            case '"':
                if (!parsingBuffer.empty())
                    throw std::runtime_error("unexpected double-quote (\")");

                parsingBuffer.push_back('"');
                while (stream.get(byte)) {
                    parsingBuffer.push_back(byte);
                    if (byte == '"')
                        break;
                }

                if (stream.eof())
                    throw (std::runtime_error("string never terminates: " + parsingBuffer));

                if (!stack.empty()) {
                    if (readyForObjectKey()) {
                        parsingBuffer.erase(parsingBuffer.begin());
                        parsingBuffer.erase(parsingBuffer.end() - 1);
                        auto key = std::make_unique<std::string>(std::move(parsingBuffer));
                        stack.push(StackElement{
                            std::move(key),
                            std::move(std::unique_ptr<ValueNodeBase>{})
                        });
                        parsingBuffer.clear();
                    }
                }
                break;
            default:
                if (!std::isspace(byte)) // locale-specific, I have read ¯\_(ツ)_/¯
                    throw std::runtime_error("'" + std::string(1, byte) + "'" + " is invalid in JSON");
            }
        }

        // should end up with an empty stack, empty buffer and a non-null pointer
        if (!(stack.empty() && head && parsingBuffer.empty()))
            throw std::runtime_error("malformed JSON");

        return JSON(std::move(head));
    }
}
