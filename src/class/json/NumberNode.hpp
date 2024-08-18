#ifndef NUMBER_NODE_H
#define NUMBER_NODE_H

#include "ValueNodeBase.hpp"

namespace JSON {
    class NumberNode : public ValueNodeBase {
        const Type type;

    public:
        NumberNode() : type(Type::Number) {}

        Type getType() const override {
            return type;
        }
    };
}

#endif // NUMBER_NODE_H
