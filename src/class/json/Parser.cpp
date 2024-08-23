#include "Parser.hpp"

constexpr size_t NUMBER_BUFFER_SIZE = 10;

static double parseNumber(std::istream& stream, char& b) {
    std::array<char, NUMBER_BUFFER_SIZE> buffer;

    buffer[0] = b;

    for (size_t i = 1; i < NUMBER_BUFFER_SIZE; i++) {
        if (!stream.get(buffer[i]))
            break;
    }

    return std::atof(buffer.data());
}

namespace JSON {
    JSON Parser::parse(std::istream& stream) {
        std::unique_ptr<ValueNodeBase> head;

        char byte;
        while (stream.get(byte)) {
            switch (byte) {
                default:
                    if (byte == '-' || std::isdigit(byte)) {
                        double v = parseNumber(stream, byte);
                        if (!head) {
                            head = std::move(std::make_unique<NumberNode>(v));
                            continue;
                        }
                    }
            }
        }

        return JSON(std::move(head));
    }
}
