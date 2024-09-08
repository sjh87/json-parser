#ifndef VALUE_NODE_BASE_H
#define VALUE_NODE_BASE_H

#include <istream>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

namespace JSON {
    enum class Type {
        Array,
        Boolean,
        Empty,
        Null,
        Number,
        Object,
        String
    };

    class ValueNodeBase {
        const Type type; // so callers know how to cast getValue()'s returned pointer
        void *value;

    public:
        ValueNodeBase() : type(Type::Empty), value(nullptr) {}

        virtual bool operator==(const ValueNodeBase& other) const;
        virtual bool operator!=(const ValueNodeBase& other) const;

        virtual Type getType() const {
            return type;
        }

        virtual void* getValue() const {
            return value;
        };
    };

    template<class C>
    bool castAndCompare(const ValueNodeBase* v1, const ValueNodeBase* v2) {
        if (!v1)
            throw std::runtime_error("v1 in JSON::castAndCompare is nullptr");
        else if (!v2)
            throw std::runtime_error("v2 in JSON::castAndCompare is nullptr");

        return *(dynamic_cast<const C*>(v1)) == *(dynamic_cast<const C*>(v2));
    }
}

#endif // VALUE_NODE_BASE_H
