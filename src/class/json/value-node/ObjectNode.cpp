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
                            return *(dynamic_cast<ArrayNode*>(value)) ==
                                *(dynamic_cast<ArrayNode*>(other->at(key).get()));
                        case Type::Boolean:
                            return *(dynamic_cast<BooleanNode*>(value)) ==
                                *(dynamic_cast<BooleanNode*>(other->at(key).get()));
                        case Type::Null:
                            return true;
                        case Type::Number:
                            return *(dynamic_cast<NumberNode*>(value)) ==
                                *(dynamic_cast<NumberNode*>(other->at(key).get()));
                        case Type::Object:
                            return *(dynamic_cast<ObjectNode*>(value)) ==
                                *(dynamic_cast<ObjectNode*>(other->at(key).get()));
                        case Type::String:
                            return *(dynamic_cast<StringNode*>(value)) ==
                                *(dynamic_cast<StringNode*>(other->at(key).get()));
                        default:
                            return false;
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
        if (other.getType() != Type::Object)
            return true;

        auto otherUmap = static_cast<ObjectStorageType*>(other.getValue());
        if (value->size() != otherUmap->size())
            return true;

        return !std::all_of(value->begin(), value->end(), isEqual(otherUmap));
    }
}
