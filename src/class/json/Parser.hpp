#ifndef PARSER_H
#define PARSER_H

#include <memory>
#include <regex>
#include <set>
#include <sstream>
#include <stack>
#include <string>
#include "JSON.hpp"
#include "value-node/ValueNodeBase.hpp"
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

            bool isOpenArray() const;
            bool isOpenObject() const;
            bool isKeyValuePair() const;
        };

        std::stack<StackElement> stack;

        bool canBeginObjectOrArray() const;
        void collapseContainer(const Type);
        bool expectingKey() const;
        bool expectingValue() const;
        bool readyForObjectKey() const;

    public:
        Parser() : stack() {};

        // takes an istream& and returns a JSON instance, or throws runtime_error
        JSON parse(std::istream&);
    };
}

#endif // PARSER_H
