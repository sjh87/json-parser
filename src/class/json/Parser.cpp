#include "Parser.hpp"

namespace JSON {
    const std::set<char> permittedEscapes{ '"', '\\', 'b', 'f', 'n', 'r', 't', '/' };

    // stole this and tested it pretty thoroughly; guilty till proven innocent,
    // like all regex :-D
    const std::regex numberPattern(R"(^-?(?:0|[1-9]\d*)(?:\.\d+)?(?:[eE][+-]?\d+)?$)");

    static std::unique_ptr<ValueNodeBase> parsePrimitive(std::string& buffer) {
        while (std::isspace(buffer.back())) {
            buffer.erase(buffer.end() - 1);
        }

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

    bool Parser::StackElement::isKeyValuePair() const {
        return key && value;
    }

    bool Parser::StackElement::isOpenArray() const {
        return value
            && value->getType() == Type::Array
            && open;
    }

    bool Parser::StackElement::isOpenObject() const {
        return value
            && value->getType() == Type::Object
            && open;
    }

    void Parser::collapseContainer(const Type type) {
        if (type != Type::Array && type != Type::Object)
            throw std::runtime_error("JSON::collapse called with non-container Type enum");

        std::stack<StackElement> temp;

        while (!stack.empty() && !stack.top().open) {
            temp.push(std::move(stack.top()));
            stack.pop();
        }

        if (stack.empty())
            throw std::runtime_error("did not find beginning of container when collapsing");
        
        if (type == Type::Object) {
            auto objectPtr = dynamic_cast<ObjectNode*>(stack.top().value.get());

            while (!temp.empty()) {
                auto tempTop = std::move(temp.top());
                if (!tempTop.key)
                    throw std::runtime_error("null key encountered while collapsing Object");
                
                objectPtr->insert(std::move(tempTop.key), std::move(tempTop.value));

                temp.pop();
            }
        } else {
            auto arrayPtr = dynamic_cast<ArrayNode*>(stack.top().value.get());

            while (!temp.empty()) {
                auto tempTop = std::move(temp.top());
                if (tempTop.key)
                    throw std::runtime_error("non-null key encountered while collapsing Array");
                
                arrayPtr->insert(std::move(tempTop.value));
                
                temp.pop();
            }
        }

        stack.top().open = false;
    }

    bool Parser::canBeginObjectOrArray() const {
        return stack.empty()
            || (!stack.top().key && stack.top().value)
            || expectingValue()
            || stack.top().isOpenArray()
            || !stack.top().open;
    }

    // top of stack has an open Object or complete key-value pair
    bool Parser::readyForObjectKey() const {
        return (stack.top().isKeyValuePair()
            || stack.top().isOpenObject())
            && !stack.top().isOpenArray();
    }

    bool Parser::expectingKey() const {
        return !stack.empty()
            && !stack.top().isOpenArray() // could be a key-value pair with an open Array
            && (stack.top().isOpenObject() || stack.top().isKeyValuePair())
            && !expectingValue();
    }

    bool Parser::expectingValue() const {
        return !stack.empty()
            && stack.top().key
            && !stack.top().value;
    }

    // stole this and refactored it a little
    static constexpr bool validateUtf8(const char* string) noexcept {
        while (*string) {
            if ((*string & 0b10000000)) {
                if (!(*string & 0b01000000)) return false;
                if (*string & 0b00100000) {
                    if (*string & 0b00010000) {
                        if (*string & 0b00001000)
                            return false;

                        if ((*++string & 0b11000000) != 0b10000000)
                            return false;
                    }

                    if ((*++string & 0b11000000) != 0b10000000)
                        return false;
                }

                if ((*++string & 0b11000000) != 0b10000000)
                    return false;
            }

            ++string;
        }

        return true;
    }

    // take next four bytes from stream and validate them as a UTF-8 ASCII representation;
    // throw if invalid, otherwise return them
    static std::string handleEscapedUnicode(std::istream& stream) {
        std::string fourBytes;
        char byte;

        while (stream.good() && fourBytes.size() < 4) {
            stream.get(byte);
            fourBytes.push_back(byte);
        }

        if (fourBytes.size() < 4)
            throw std::runtime_error("unvalid unicode sequence: \\u" + fourBytes);

        if (validateUtf8(fourBytes.data()))
            return "\\u" + fourBytes;

        throw std::runtime_error("invalid unicode sequence: \\ud" + fourBytes);
    }

    JSON Parser::parse(std::istream& stream) {
        bool justSawColon{ false };
        bool justSawComma{ false };
        std::string parsingBuffer;
        std::unique_ptr<ValueNodeBase> head;

        char byte;
        while (true) {
            stream.get(byte);

            if (stream.eof()) {
                if (stack.size() == 1 && !stack.top().open && parsingBuffer.empty()) {
                    head = std::move(stack.top().value);
                    stack.pop();
                } else if (!parsingBuffer.empty() && stack.empty()) {
                    head = std::move(parsePrimitive(parsingBuffer));
                } else {
                    break; // let the check after the while loop catch it
                }

                parsingBuffer.clear();

                break;
            } else if (stream.bad() || stream.fail()) {
                throw std::runtime_error("failed to read stream");
            }

            std::unique_ptr<ValueNodeBase> node;

            switch (byte) {
            case ',':
                if (justSawComma)
                    throw std::runtime_error("double comma ',,' encountered");

                if (stack.empty()
                    || ((stack.top().isOpenArray()
                    || expectingValue()) && parsingBuffer.empty())
                    || (stack.size() == 1 && !stack.top().open)
                    || justSawColon) {
                    throw std::runtime_error("unexpected ',' encountered");
                }

                justSawComma = true;

                if (!parsingBuffer.empty()) {
                    node = std::move(parsePrimitive(parsingBuffer));
                    parsingBuffer.clear();
                } else {
                    continue;
                }

                if (expectingValue()) {
                    stack.top().value = std::move(node);
                } else if (stack.top().value && !stack.top().isOpenObject()) {
                    stack.push(StackElement{
                        std::unique_ptr<std::string>(nullptr),
                        std::move(node)
                    });
                }

                break;
            case '[':
                if (canBeginObjectOrArray()) {
                    node = std::make_unique<ArrayNode>();
                    if (stack.empty() || !stack.top().key || stack.top().isOpenArray()) {
                        stack.push(StackElement{
                            std::unique_ptr<std::string>(nullptr),
                            std::move(node),
                            true
                        });
                    } else if (stack.top().key && justSawColon) {
                        stack.top().value = std::move(node);
                        stack.top().open = true;
                        justSawColon = false;
                    }

                    justSawComma = false;
                }

                break;
            case ']':
                if (stack.empty()) {
                    throw std::runtime_error("unexpected ']' encountered");
                }

                if (justSawComma && parsingBuffer.empty())
                    throw std::runtime_error("dangling comma before ']'");

                if (!parsingBuffer.empty()
                    && stack.top().value
                    && (stack.top().isOpenArray() || !stack.top().key)) {
                    std::unique_ptr<ValueNodeBase> ptr = parsePrimitive(parsingBuffer);

                    stack.push(StackElement{
                        std::unique_ptr<std::string>(nullptr),
                        std::move(ptr)
                    });


                    parsingBuffer.clear();
                }

                justSawComma = false;

                collapseContainer(Type::Array);

                break;
            case '{':
                if (canBeginObjectOrArray()) {
                    node = std::make_unique<ObjectNode>();
                    if (stack.empty() || !stack.top().key || stack.top().isOpenArray()) {
                        stack.push(StackElement{
                            std::unique_ptr<std::string>(nullptr),
                            std::move(node),
                            true
                        });
                    } else if (stack.top().key && justSawColon) {
                        stack.top().value = std::move(node);
                        stack.top().open = true;
                        justSawColon = false;
                    }
                } else {
                    throw std::runtime_error("unexpected '{' encountered");
                }

                justSawComma = false;

                break;
            case '}':
                if (stack.empty()
                    || justSawColon
                    || ((justSawComma || expectingValue())
                    && parsingBuffer.empty())) {
                    throw std::runtime_error("unexpected '}' encountered");
                }

                if (!parsingBuffer.empty() && expectingValue()) {
                    std::unique_ptr<ValueNodeBase> ptr = parsePrimitive(parsingBuffer);

                    stack.top().value = std::move(ptr);
                    parsingBuffer.clear();
                }

                collapseContainer(Type::Object);

                break;
            case ':':
                if (justSawColon)
                    throw std::runtime_error("double colon '::' encountered");

                if (expectingKey()) {
                    throw std::runtime_error("expected key, got ':'");
                } else if (!expectingValue()) {
                    throw std::runtime_error("':' encountered outside of object");
                }

                justSawColon = true;

                break;
            case '-':
            case '.':
            case '+':
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

                if (!justSawColon && expectingValue() && parsingBuffer.empty())
                    throw std::runtime_error("expected ':', saw '" + std::string(1, byte) + "'");

                parsingBuffer.push_back(byte);
                justSawColon = false;
                justSawComma = false;

                break;
            case '"':
                if (!parsingBuffer.empty())
                    throw std::runtime_error("unexpected double-quote (\")");

                parsingBuffer.push_back('"');
                justSawComma = false;

                while (stream.get(byte)) {
                    if (byte == '\\') {
                        if (stream.get(byte)) {
                            if (byte == 'u') {
                                parsingBuffer += handleEscapedUnicode(stream);

                                continue;
                                if (stream.eof())
                                    break;
                            } else if (permittedEscapes.find(byte) == permittedEscapes.end()) {
                                throw std::runtime_error("naughty escape sequence: \\" + std::string(1, byte));
                            } else {
                                parsingBuffer.push_back(byte);

                                continue;
                                if (stream.eof())
                                    break;
                            }
                        } else {
                            break;
                        }
                    } else if ('\t' == byte) {
                        throw std::runtime_error("unescaped tab character in string is not permitted");
                    } else if ('\n' == byte || '\r' == byte) {
                        throw std::runtime_error("unescaped newline or carriage return character in string is not permitted");
                    } else if ('\b' == byte) {
                        throw std::runtime_error("unescaped backspace character in string is not permitted");
                    } else if ('\f' == byte) {
                        throw std::runtime_error("unescaped form feed character in string is not permitted");
                    }

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
                            std::unique_ptr<ValueNodeBase>()
                        });

                        parsingBuffer.clear();
                    }
                }

                justSawColon = false;

                break;
            default:
                if (!std::isspace(byte))
                    throw std::runtime_error("'" + std::string(1, byte) + "'" + " is invalid in JSON");

                if (!parsingBuffer.empty())
                    parsingBuffer.push_back(byte);
            }
        }

        // should end up with an empty stack, empty buffer and a non-null pointer
        if (!(stack.empty() && head && parsingBuffer.empty()))
            throw std::runtime_error("malformed JSON");

        return JSON(std::move(head));
    }
}
