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
        const Type type;
        std::nullptr_t value;

    public:
        ValueNodeBase() : type(Type::Empty) {}

        virtual Type getType() const {
            return type;
        }

        virtual std::nullptr_t getValue() const {
            return value;
        };
    };
}

#endif // VALUE_NODE_BASE_H
