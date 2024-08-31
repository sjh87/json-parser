#include "NullNode.hpp"

namespace JSON {
    bool NullNode::operator==(const ValueNodeBase& other) const {
        return other.getType() == Type::Null;
    }

    bool NullNode::operator!=(const ValueNodeBase& other) const {
        return other.getType() != Type::Null;
    }
}
