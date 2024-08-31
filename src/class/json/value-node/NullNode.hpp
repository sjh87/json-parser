#ifndef NULL_NODE_H
#define NULL_NODE_H

#include "ValueNodeBase.hpp"

namespace JSON {
    class NullNode : public ValueNodeBase {
        const Type type;
        const std::unique_ptr<std::string> value;

    public:
        NullNode() : type(Type::Null), value(std::make_unique<std::string>("null")) {}

        bool operator==(const ValueNodeBase& other) const override;
        bool operator!=(const ValueNodeBase& other) const override;

        Type getType() const override {
            return type;
        }

        void* getValue() const override {
            return value.get();
        }
    };
}

#endif // NULL_NODE_H
