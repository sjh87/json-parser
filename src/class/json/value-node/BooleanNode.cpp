#include "BooleanNode.hpp"

namespace JSON {
    bool BooleanNode::operator==(const ValueNodeBase& other) const {
        if (other.getType() == Type::Boolean) {
            return *value == *(static_cast<bool*>(other.getValue()));
        }

        return false;
    }

    bool BooleanNode::operator!=(const ValueNodeBase& other) const {
        if (other.getType() == Type::Boolean) {
            return *value != *(static_cast<bool*>(other.getValue()));
        }

        return true;
    }
}
