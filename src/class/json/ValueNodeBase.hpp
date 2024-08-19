#ifndef VALUE_NODE_BASE_H
#define VALUE_NODE_BASE_H

#include <istream>
#include <memory>
#include <string>
#include <unordered_map>
#include <variant>
#include <vector>

namespace JSON {
    enum class Type {
        Empty,
        Number
    };

    class ValueNodeBase {
        const Type type; // so callers know how to cast getValue()'s returned pointer
        void *value;

    public:
        ValueNodeBase() : type(Type::Empty), value(nullptr) {}

        virtual Type getType() const {
            return type;
        }

        virtual void* getValue() const {
            return value;
        };
    };
}

#endif // VALUE_NODE_BASE_H
