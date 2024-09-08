#include "ArrayNode.hpp"

namespace JSON {
    static bool isEqual(const std::unique_ptr<ValueNodeBase>&v1, const std::unique_ptr<ValueNodeBase>&v2) {
        if (v1->getType() != v2->getType())
            return false;

        switch(v1->getType()) {
            case Type::Array:
                return castAndCompare<ArrayNode>(v1.get(), v2.get());
            case Type::Boolean:
                return castAndCompare<BooleanNode>(v1.get(), v2.get());
            case Type::Null:
                return true;
            case Type::Number:
                return castAndCompare<NumberNode>(v1.get(), v2.get());
            case Type::Object:
                return castAndCompare<ObjectNode>(v1.get(), v2.get());
            case Type::String:
                return castAndCompare<StringNode>(v1.get(), v2.get());;
            default:
                throw std::runtime_error("ArrayNode::isEqual cannot compare Empty values");
        }
    };

    bool ArrayNode::operator==(const ValueNodeBase& other) const {

        if (other.getType() == Type::Array) {
            auto otherVector = static_cast<ArrayStorageType*>(other.getValue());
            if (value->size() != otherVector->size())
                return false;

            return std::equal(value->begin(), value->end(), otherVector->begin(), isEqual);
        }

        return false;
    }

    bool ArrayNode::operator!=(const ValueNodeBase& other) const {
        return !(*this == other);
    }
}
