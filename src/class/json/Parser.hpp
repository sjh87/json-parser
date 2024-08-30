#ifndef PARSER_H
#define PARSER_H

#include <array>
#include <memory>
#include <regex>
#include <sstream>
#include <stack>
#include <string>
#include "JSON.hpp"
#include "value-node/ArrayNode.hpp"
#include "value-node/BooleanNode.hpp"
#include "value-node/NullNode.hpp"
#include "value-node/NumberNode.hpp"
#include "value-node/ObjectNode.hpp"
#include "value-node/StringNode.hpp"

namespace JSON {
    struct StackElement {
        std::unique_ptr<std::string> key;
        std::unique_ptr<ValueNodeBase> value;
    };

    using StackType = std::stack<StackElement>;

    class Parser {
        StackType stack;

    public:
        Parser() : stack() {};

        // takes an istream& and returns a JSON instance, or throws runtime_error
        JSON parse(std::istream& stream);
    };
}

#endif // PARSER_H
