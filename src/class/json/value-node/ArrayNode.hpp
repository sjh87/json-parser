#ifndef ARRAY_NODE_H
#define ARRAY_NODE_H

#include <algorithm>

#include "BooleanNode.hpp"
#include "NullNode.hpp"
#include "NumberNode.hpp"
#include "ObjectNode.hpp"
#include "StringNode.hpp"
#include "ValueNodeBase.hpp"

namespace JSON {
    using ArrayStorageType = std::vector<std::unique_ptr<ValueNodeBase>>;
    class ArrayNode : public ValueNodeBase {

        const Type type;
        const std::unique_ptr<ArrayStorageType> value;

    public:
        ArrayNode() : type(Type::Array), value(std::make_unique<ArrayStorageType>()) {}
        ArrayNode(ArrayStorageType&& vector) :
            type(Type::Array), value(std::make_unique<ArrayStorageType>(std::move(vector))) {};

        bool operator==(const ValueNodeBase&) const override;
        bool operator!=(const ValueNodeBase&) const override;

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
            if (!up)
                throw std::runtime_error("null pointer provided to ArrayNode::insert");

            value->push_back(std::move(up));
        }
    };
}

#endif // ARRAY_NODE_H
