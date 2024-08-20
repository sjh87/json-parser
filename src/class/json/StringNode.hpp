#ifndef STRING_NODE_H
#define STRING_NODE_H

#include "ValueNodeBase.hpp"

namespace JSON {
    class StringNode : public ValueNodeBase {
        const Type type;
        const std::unique_ptr<std::string> value;

    public:
        StringNode(std::string&& s) : type(Type::String), value(std::make_unique<std::string>(std::move(s))) {}

        Type getType() const override {
            return type;
        }

        void* getValue() const override {
            return value.get();
        }
    };
}

#endif // STRING_NODE_H
