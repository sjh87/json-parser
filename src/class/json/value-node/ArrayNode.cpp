#include "ArrayNode.hpp"

namespace JSON {
    bool ArrayNode::operator==(const ValueNodeBase& other) const {
        if (other.getType() == Type::Array) {
            auto otherVector = static_cast<ArrayStorageType*>(other.getValue());
            if (value->size() == otherVector->size()) {
                for (size_t i = 0; i < value->size(); i++) {
                    auto type = value->at(i)->getType();
                    if (type != otherVector->at(i)->getType())
                        return false;
                    
                    switch(type) {
                        case Type::Array:
                            if (*(static_cast<ArrayNode*>(value->at(i).get())) ==
                                *(static_cast<ArrayNode*>(otherVector->at(i).get())))
                                    break;
                            
                            return false;
                        case Type::Boolean:
                            if (*(static_cast<BooleanNode*>(value->at(i).get())) ==
                                *(static_cast<BooleanNode*>(otherVector->at(i).get())))
                                    break;
                            
                            return false;
                        case Type::Null:
                            break;
                        case Type::Number:
                            if (*(static_cast<NumberNode*>(value->at(i).get())) ==
                                *(static_cast<NumberNode*>(otherVector->at(i).get())))
                                    break;
                            
                            return false;
                        case Type::Object:
                            if (*(static_cast<ObjectNode*>(value->at(i).get())) ==
                                *(static_cast<ObjectNode*>(otherVector->at(i).get())))
                                    break;
                            
                            return false;
                        case Type::String:
                            if (*(static_cast<StringNode*>(value->at(i).get())) ==
                                *(static_cast<StringNode*>(otherVector->at(i).get())))
                                    break;
                            
                            return false;
                        default:
                            return false;
                    }
                }

                return true;
            }
        }

        return false;
    }

    bool ArrayNode::operator!=(const ValueNodeBase& other) const {
        if (other.getType() == Type::Array) {
            auto otherVector = static_cast<ArrayStorageType*>(other.getValue());
            if (value->size() == otherVector->size()) {
                for (size_t i = 0; i < value->size(); i++) {
                    auto type = value->at(i)->getType();
                    if (type != otherVector->at(i)->getType())
                        return true;
                    
                    switch(type) {
                        case Type::Array:
                            if (*(static_cast<ArrayNode*>(value->at(i).get())) ==
                                *(static_cast<ArrayNode*>(otherVector->at(i).get())))
                                    break;
                            
                            return true;
                        case Type::Boolean:
                            if (*(static_cast<BooleanNode*>(value->at(i).get())) ==
                                *(static_cast<BooleanNode*>(otherVector->at(i).get())))
                                    break;
                            
                            return true;
                        case Type::Null:
                            break;
                        case Type::Number:
                            if (*(static_cast<NumberNode*>(value->at(i).get())) ==
                                *(static_cast<NumberNode*>(otherVector->at(i).get())))
                                    break;
                            
                            return true;
                        case Type::Object:
                            if (*(static_cast<ObjectNode*>(value->at(i).get())) ==
                                *(static_cast<ObjectNode*>(otherVector->at(i).get())))
                                    break;
                            
                            return true;
                        case Type::String:
                            if (*(static_cast<StringNode*>(value->at(i).get())) ==
                                *(static_cast<StringNode*>(otherVector->at(i).get())))
                                    break;
                            
                            return true;
                        default:
                            return true;
                    }
                }

                return false;
            }
        }

        return true;
    }
}
