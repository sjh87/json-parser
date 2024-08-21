#ifndef ARRAY_NODE_H
#define ARRAY_NODE_H

#include "ValueNodeBase.hpp"

namespace JSON {
    class ArrayNode : public ValueNodeBase {
        using StorageType = std::vector<std::unique_ptr<ValueNodeBase>>;

        const Type type;
        const std::unique_ptr<StorageType> value;

    public:
        ArrayNode() : type(Type::Array), value(std::make_unique<StorageType>()) {}

        Type getType() const override {
            return type;
        }

        // may want to overload this to take an unsigned int x and return a
        // pointer to the value y at x, but that's a nice-to-have feature and I
        // want to keep the interface across ValueNode derivatives the same for
        // now
        void* getValue() const override {
            return value.get();
        }

        void insert(std::unique_ptr<ValueNodeBase>&& up) {
            value->push_back(std::move(up));
        }
    };
}

#endif // ARRAY_NODE_H
