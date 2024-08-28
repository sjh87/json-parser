#include "Parser.hpp"

namespace JSON {
    static double parseNumber(std::string& buffer) {
        bool isPositive{true};
        if (buffer.front() == '-') {
            isPositive = false;
            buffer.erase(buffer.begin());
        }

        if (buffer.front() == '0' && !(buffer[1] == '.' || buffer.size() == 1)) {
            throw std::runtime_error(buffer + "is invalid JSON");
        } else if (buffer.front() == '.' || buffer.back() == '.')
            throw std::runtime_error(buffer + "is invalid JSON");

        double d = std::atof(buffer.data());

        return isPositive ? d : -1 * d;
    }

    // static void trimValueString(std::string& vs) {
    //     if (vs.empty())
    //         return;

    //     while (std::isspace(vs.front()))
    //         vs.erase(vs.begin());

    //     while (std::isspace(vs.back()))
    //         vs.erase(vs.end() - 1);
    // }

    static bool appearsToBeANumber(const std::string& candidate) {
        return (
            std::isdigit(candidate.front()) || candidate.front() == '-'
        ) && std::isdigit(candidate.back());
    }

    static std::unique_ptr<ValueNodeBase> parsePrimitive(std::string& buffer) {
        if (buffer == "true" || buffer == "false") {
            return std::make_unique<BooleanNode>(buffer == "true");
        } else if (appearsToBeANumber(buffer)) {
            double d = parseNumber(buffer); // TODO handle exceptions
            return std::make_unique<NumberNode>(d);
        } else if (buffer == "null") {
            return std::make_unique<NullNode>();
        } else if (buffer.front() == '"' && buffer.back() == '"') {
            buffer.erase(buffer.begin());
            buffer.erase(buffer.end() - 1);
            return std::make_unique<StringNode>(std::move(buffer));
        }

        throw std::runtime_error(buffer + " is invalid JSON");
    }

    static void collapse(StackType& stack, const char &c) {
        StackType temp;
        Type type{Type::Empty};

        if (c == ']') {
            type = Type::Array;
        } else if (c == '}') {
            type = Type::Object;
        } else
            std::runtime_error("NONONNONONO BAD BAD BAD");

        while (!stack.empty() && stack.top().second->getType() != type) {
            temp.push(std::move(stack.top()));
            stack.pop();
        }

        if (stack.top().second->getType() != type)
            throw std::runtime_error("NONONONONONO BAD BAD BAD");
        
        if (type == Type::Object) {
            auto objectPtr = static_cast<ObjectNode*>(stack.top().second.get());

            while (!temp.empty()) {
                auto tempTop = std::move(temp.top());
                if (tempTop.first.empty())
                    throw std::runtime_error("empty key encountered while collapsing Object");
                
                objectPtr->insert(std::move(tempTop.first), std::move(tempTop.second));

                temp.pop();
            }
        } else {
            auto arrayPtr = static_cast<ArrayNode*>(stack.top().second.get());

            while (!temp.empty()) {
                auto tempTop = std::move(temp.top());
                if (!tempTop.first.empty())
                    throw std::runtime_error("non-empty key encountered while collapsing Array");
                
                arrayPtr->insert(std::move(tempTop.second));
                
                temp.pop();
            }
        }
    }

    static void validateParserEndState(
        StackType& stack,
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

    static bool canBeginObjectOrArray(const StackType& stack) {
        if (stack.empty()) {
            return true;
        } else if (stack.top().first.empty() && stack.top().second) {
            return true;
        } else if (!stack.top().first.empty() && !stack.top().second) {
            return true;
        } else if (stack.top().second->getType() == Type::Array) {
            return true;
        }


        return false;
    }

    // if prior stack element has a key and value, or its value is an Object
    // itself, return true
    static bool readyForObjectKey(const StackType &stack) {
        if (!stack.top().first.empty() && stack.top().second)
            return true;

        return stack.top().second && stack.top().second->getType() == Type::Object;
    }

    JSON Parser::parse(std::istream& stream) {
        char byte; // TODO figure out UTF-8 parsing, validation
        std::string parsingBuffer;
        std::unique_ptr<ValueNodeBase> head;

        while (true) {
            stream.get(byte);

            if (stream.eof()) {
                if (stack.size() == 1 && parsingBuffer.empty()) {
                    head = std::move(stack.top().second);
                    stack.pop();
                } else if (!parsingBuffer.empty() && stack.empty()) {
                    validateParserEndState(stack, head, parsingBuffer);
                    head = std::move(parsePrimitive(parsingBuffer));
                } else {
                    break; // check after the while loop catch it
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

                if (!stack.top().first.empty() && !stack.top().second) {
                    stack.top().second = std::move(node);
                } else if (stack.top().second) {
                    stack.push(std::make_pair("", std::move(node)));
                }

                break;
            case '[':
                if (canBeginObjectOrArray(stack)) {
                    node = std::make_unique<ArrayNode>();
                    if (stack.empty()) {
                        stack.push(std::make_pair("", std::move(node)));
                    } else if (!stack.top().first.empty()) {
                        stack.top().second = std::move(node);
                    } else if (stack.top().first.empty()) {
                        stack.push(std::make_pair("", std::move(node)));
                    } else if (stack.top().second->getType() == Type::Array) {
                        stack.push(std::make_pair("", std::move(node)));
                    }
                }
                break;
            case ']':
                if (stack.empty()) {
                    throw std::runtime_error("unexpected ']' encountered");
                }

                if (
                    !parsingBuffer.empty()
                    && stack.top().second
                    && (stack.top().second->getType() == Type::Array || stack.top().first.empty())
                ) {
                    auto ptr = parsePrimitive(parsingBuffer);
                    stack.push(std::make_pair("", std::move(ptr)));
                    parsingBuffer.clear();
                }

                collapse(stack, ']');
                break;
            case '{':
                if (canBeginObjectOrArray(stack)) {
                    node = std::make_unique<ObjectNode>();
                    if (stack.empty()) {
                        stack.push(std::make_pair("", std::move(node)));
                    } else if (!stack.top().first.empty()) {
                        stack.top().second = std::move(node);
                    } else if (stack.top().first.empty()) {
                        stack.push(std::make_pair("", std::move(node)));
                    } else if (stack.top().second->getType() == Type::Array) {
                        stack.push(std::make_pair("", std::move(node)));
                    }
                } else {
                    throw std::runtime_error("unexpected '{' encountered");
                }
                break;
            case '}':
                if (stack.empty()) {
                    throw std::runtime_error("unexpected '}' encountered");
                }

                if (!parsingBuffer.empty() && !stack.top().first.empty() && !stack.top().second) {
                    auto ptr = parsePrimitive(parsingBuffer);
                    stack.top().second = std::move(ptr);
                    parsingBuffer.clear();
                }

                collapse(stack, '}');
                break;
            case ':':
                if (stack.empty() || head || stack.top().first.empty() || stack.top().second) {
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
                parsingBuffer.push_back(byte);
                break;
            case '"':
                if (!parsingBuffer.empty())
                    throw std::runtime_error("unexpectd double-quote (\")");

                parsingBuffer.push_back('"');
                while (stream.get(byte)) {
                    parsingBuffer.push_back(byte);
                    if (byte == '"')
                        break;
                }

                if (!stack.empty()) {
                    if (readyForObjectKey(stack)) {
                        parsingBuffer.erase(parsingBuffer.begin());
                        parsingBuffer.erase(parsingBuffer.end() - 1);
                        stack.push(
                            std::make_pair(
                                parsingBuffer,
                                std::move(std::unique_ptr<ValueNodeBase>{}))
                        );
                        parsingBuffer.clear();
                    }
                }

                break;
            }
        }

        // should end up with an empty stack, empty buffer and a non-null pointer
        if (!(stack.empty() && head && parsingBuffer.empty()))
            throw std::runtime_error("malformed JSON");

        return JSON(std::move(head));
    }
}
