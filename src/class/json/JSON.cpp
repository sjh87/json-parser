#include "JSON.hpp"

namespace JSON {
    bool JSON::operator==(const JSON& other) const {
        return *head == *(other.head);
    }

    bool JSON::operator!=(const JSON& other) const {
        return *head != *(other.head);
    }
}
