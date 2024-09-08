#include "ArrayNode.hpp"

namespace JSON {
    static bool isEqual(const std::unique_ptr<ValueNodeBase>&v1, const std::unique_ptr<ValueNodeBase>&v2) {
        if (v1->getType() != v2->getType())
            return false;

        switch(v1->getType()) {
            case Type::Array:
                return *(dynamic_cast<ArrayNode*>(v1.get())) ==
                    *(dynamic_cast<ArrayNode*>(v2.get()));
            case Type::Boolean:
                return *(dynamic_cast<BooleanNode*>(v1.get())) ==
                    *(dynamic_cast<BooleanNode*>(v2.get()));
            case Type::Null:
                return true;
            case Type::Number:
                return *(dynamic_cast<NumberNode*>(v1.get())) ==
                    *(dynamic_cast<NumberNode*>(v2.get()));
            case Type::Object:
                return *(dynamic_cast<ObjectNode*>(v1.get())) ==
                    *(dynamic_cast<ObjectNode*>(v2.get()));
            case Type::String:
                return *(dynamic_cast<StringNode*>(v1.get())) ==
                    *(dynamic_cast<StringNode*>(v2.get()));
            default:
                return false;
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
        if (other.getType() == Type::Array) {
            auto otherVector = static_cast<ArrayStorageType*>(other.getValue());
            if (value->size() != otherVector->size())
                return true;

            return !std::equal(value->begin(), value->end(), otherVector->begin(), isEqual);
        }

        return true;
    }
}
