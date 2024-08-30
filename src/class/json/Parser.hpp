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
    class Parser {
        struct StackElement {
            std::unique_ptr<std::string> key;
            std::unique_ptr<ValueNodeBase> value;
            bool open{ false };

            bool isArray() const;
            bool isObject() const;
        };

        std::stack<StackElement> stack;

        bool canBeginObjectOrArray() const;
        void collapseContainer(const Type);
        bool expectingKey() const;
        bool readyForObjectKey() const;
        void validateParserEndState(
            std::unique_ptr<ValueNodeBase>&,
            std::string&
        );

    public:
        Parser() : stack() {};

        // takes an istream& and returns a JSON instance, or throws runtime_error
        JSON parse(std::istream&);
    };
}

#endif // PARSER_H
