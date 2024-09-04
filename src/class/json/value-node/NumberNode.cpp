#include "NumberNode.hpp"

namespace JSON {
    bool NumberNode::operator==(const ValueNodeBase& other) const {
        if (other.getType() == Type::Number)
            return *value == *(static_cast<double*>(other.getValue()));

        return false;
    }

    bool NumberNode::operator!=(const ValueNodeBase& other) const {
        if (other.getType() == Type::Number)
            return *value != *(static_cast<double*>(other.getValue()));

        return true;
    }
}
