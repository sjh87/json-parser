#include "ObjectNode.hpp"

namespace JSON {
    bool ObjectNode::operator==(const ValueNodeBase& other) const {
        if (other.getType() != Type::Object)
            return false;
        
        auto otherUmap = static_cast<ObjectStorageType*>(other.getValue());
        if (value->size() != otherUmap->size())
            return false;

        return std::all_of(value->begin(), value->end(), [&otherUmap](auto& pair) {
            const std::string& key = pair.first;
            ValueNodeBase* value = pair.second.get();

            if (otherUmap->find(key) != otherUmap->end()) {
                const Type type = pair.second->getType();
                if (otherUmap->at(key)->getType() == pair.second->getType()) {
                    switch(type) {
                        case Type::Array:
                            return *(static_cast<ArrayNode*>(value)) ==
                                *(static_cast<ArrayNode*>(otherUmap->at(key).get()));
                        case Type::Boolean:
                            return *(static_cast<BooleanNode*>(value)) ==
                                *(static_cast<BooleanNode*>(otherUmap->at(key).get()));
                        case Type::Null:
                            return true;
                        case Type::Number:
                            return *(static_cast<NumberNode*>(value)) ==
                                *(static_cast<NumberNode*>(otherUmap->at(key).get()));
                        case Type::Object:
                            return *(static_cast<ObjectNode*>(value)) ==
                                *(static_cast<ObjectNode*>(otherUmap->at(key).get()));
                        case Type::String:
                            return *(static_cast<StringNode*>(value)) ==
                                *(static_cast<StringNode*>(otherUmap->at(key).get()));
                        default:
                            return false;
                    }
                }
            }

            return false;
        });
    }

    bool ObjectNode::operator!=(const ValueNodeBase& other) const {
        if (other.getType() != Type::Object)
            return true;
        
        auto otherUmap = static_cast<ObjectStorageType*>(other.getValue());
        if (value->size() != otherUmap->size())
            return true;

        return !std::all_of(value->begin(), value->end(), [&otherUmap](auto& pair) {
            const std::string& key = pair.first;
            ValueNodeBase* value = pair.second.get();

            if (otherUmap->find(key) != otherUmap->end()) {
                const Type type = pair.second->getType();
                if (otherUmap->at(key)->getType() == pair.second->getType()) {
                    switch(type) {
                        case Type::Array:
                            return *(static_cast<ArrayNode*>(value)) ==
                                *(static_cast<ArrayNode*>(otherUmap->at(key).get()));
                        case Type::Boolean:
                            return *(static_cast<BooleanNode*>(value)) ==
                                *(static_cast<BooleanNode*>(otherUmap->at(key).get()));
                        case Type::Null:
                            return true;
                        case Type::Number:
                            return *(static_cast<NumberNode*>(value)) ==
                                *(static_cast<NumberNode*>(otherUmap->at(key).get()));
                        case Type::Object:
                            return *(static_cast<ObjectNode*>(value)) ==
                                *(static_cast<ObjectNode*>(otherUmap->at(key).get()));
                        case Type::String:
                            return *(static_cast<StringNode*>(value)) ==
                                *(static_cast<StringNode*>(otherUmap->at(key).get()));
                        default:
                            return false;
                    }
                }
            }

            return false;
        });
    }
}
