#include "StringNode.hpp"

namespace JSON {
    bool StringNode::operator==(const ValueNodeBase& other) const {
        if (other.getType() == Type::String)
            return *value == *(static_cast<std::string*>(other.getValue()));
        
        return false;
    }

    bool StringNode::operator!=(const ValueNodeBase& other) const {
        if (other.getType() == Type::String)
            return *value != *(static_cast<std::string*>(other.getValue()));
        
        return true;
    }
}
