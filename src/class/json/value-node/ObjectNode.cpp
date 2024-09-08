#include "ObjectNode.hpp"

namespace JSON {
    using MapPairType = std::pair<const std::string, std::unique_ptr<ValueNodeBase>>;

    static std::function<bool(const MapPairType&)> isEqual(ObjectStorageType* other) {
        return [other](const MapPairType& pair) {
            const std::string& key = pair.first;
            ValueNodeBase* value = pair.second.get();

            if (other->find(key) != other->end()) {
                const Type type = pair.second->getType();
                if (other->at(key)->getType() == pair.second->getType()) {
                    switch(type) {
                        case Type::Array:
                            return castAndCompare<ArrayNode>(value, other->at(key).get());
                        case Type::Boolean:
                            return castAndCompare<BooleanNode>(value, other->at(key).get());
                        case Type::Null:
                            return true;
                        case Type::Number:
                            return castAndCompare<NumberNode>(value, other->at(key).get());
                        case Type::Object:
                            return castAndCompare<ObjectNode>(value, other->at(key).get());
                        case Type::String:
                            return castAndCompare<StringNode>(value, other->at(key).get());
                        default:
                            throw std::runtime_error("ObjectNode::isEqual cannot compare Empty values");
                    }
                }
            }

            return false;
        };
    }

    bool ObjectNode::operator==(const ValueNodeBase& other) const {
        if (other.getType() != Type::Object)
            return false;

        auto otherUmap = static_cast<ObjectStorageType*>(other.getValue());
        if (value->size() != otherUmap->size())
            return false;

        return std::all_of(value->begin(), value->end(), isEqual(otherUmap));
    }

    bool ObjectNode::operator!=(const ValueNodeBase& other) const {
        return !(*this == other);
    }
}
