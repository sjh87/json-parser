#include "ValueNodeBase.hpp"

namespace JSON {
    bool ValueNodeBase::operator==(const ValueNodeBase&) const {
        return false; // uncast nodes should always be unequal
    }

    bool ValueNodeBase::operator!=(const ValueNodeBase&) const {
        return true; // uncast nodes should always be unequal
    }
}
