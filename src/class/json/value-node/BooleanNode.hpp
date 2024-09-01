#ifndef BOOLEAN_NODE_H
#define BOOLEAN_NODE_H

#include "ValueNodeBase.hpp"

namespace JSON {
    class BooleanNode : public ValueNodeBase {
        const Type type;
        const std::unique_ptr<bool> value;

    public:
        BooleanNode(bool n) : type(Type::Boolean), value(std::make_unique<bool>(n)) {}

        bool operator==(const ValueNodeBase&) const override;
        bool operator!=(const ValueNodeBase&) const override;

        Type getType() const override {
            return type;
        }

        void* getValue() const override {
            return value.get();
        }
    };
}

#endif // BOOLEAN_NODE_H
