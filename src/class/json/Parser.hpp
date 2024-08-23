#ifndef PARSER_H
#define PARSER_H

#include <array>
#include <memory>
#include <sstream>
#include <stack>
#include <string>
#include "JSON.hpp"
#include "value-node/ValueNodeBase.hpp"
#include "value-node/NumberNode.hpp"

namespace JSON {
    class InvalidJSONError : public std::exception {
        const std::string message;

        InvalidJSONError(std::string m) : message(m) {} 
    };

    struct StackElement {
        std::pair<unsigned, unsigned> loc{ 0, 0 };
        std::unique_ptr<std::string> key{ nullptr };
        std::unique_ptr<ValueNodeBase> value{ nullptr };
    };

    class Parser {
        std::stack<StackElement> majorStack;
        std::stack<StackElement> minorStack;

    public:
        Parser() : majorStack(), minorStack() {};

        // takes an istream& and returns a JSON instance, or throws
        // InvalidJSONError
        JSON parse(std::istream& stream);
    };
}

#endif // PARSER_H
