#ifndef PARSER_H
#define PARSER_H

#include <memory>
#include <stack>
#include <string>
#include "../JSON.hpp"
#include "../value-node/ValueNodeBase.hpp"

namespace JSON {
    struct StackElement {
        std::pair<unsigned, unsigned> loc{ 0 , 0 };
        std::unique_ptr<std::string> key{ nullptr };
        std::unique_ptr<ValueNodeBase> value{ nullptr };
    };

    class Parser {
        std::stack<StackElement> majorStack;
        std::stack<StackElement> minorStack;

    public:
        void parse(std::unique_ptr<JSON>& jsonPtr) {}
    };
}

#endif // PARSER_H
