#include "Parser.hpp"

constexpr size_t NUMBER_BUFFER_SIZE = 10;

namespace JSON {
    double parseNumber(std::istream& stream, char& b) {
        std::array<char, NUMBER_BUFFER_SIZE> buffer{ 0 };

        buffer[0] = b;

        for (size_t i = 1; i < NUMBER_BUFFER_SIZE; i++) {
            if (!stream.get(buffer[i]))
                break;
        }

        if (buffer[0] == '0' && !(buffer[1] == '.' || !buffer[1])) {
            throw InvalidJSONError("are you nuts?");
        }

        return std::atof(buffer.data());
    }

    JSON Parser::parse(std::istream& stream) {
        std::unique_ptr<ValueNodeBase> head;

        char byte;
        while (stream.get(byte)) {
            switch (byte) {
                case '-':
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
                    double v = parseNumber(stream, byte);
                    if (!head) {
                        head = std::move(std::make_unique<NumberNode>(v));
                        continue;
                    }
            }
        }

        return JSON(std::move(head));
    }
}
