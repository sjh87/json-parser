#ifndef NUMBER_NODE_H
#define NUMBER_NODE_H

#include "ValueNodeBase.hpp"

namespace JSON {
    class NumberNode : public ValueNodeBase {
        const Type type;
        const std::unique_ptr<double> value;

    public:
        NumberNode(double n) : type(Type::Number), value(std::make_unique<double>(n)) {}

        Type getType() const override {
            return type;
        }

        void* getValue() const override {
            return value.get();
        }
    };
}

#endif // NUMBER_NODE_H
