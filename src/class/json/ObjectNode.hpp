#ifndef OBJECT_NODE_H
#define OBJECT_NODE_H

#include "ValueNodeBase.hpp"

namespace JSON {
    class ObjectNode : public ValueNodeBase {
        using StorageType = std::unordered_map<const char*, std::unique_ptr<ValueNodeBase>>;

        const Type type;
        const std::unique_ptr<StorageType> value;

    public:
        ObjectNode() : type(Type::Object), value(std::make_unique<StorageType>()) {}

        Type getType() const override {
            return type;
        }

        void* getValue() const override {
            return value.get();
        }

        void insert(const char* keyPtr, std::unique_ptr<ValueNodeBase>&& valuePtr) {
            value->insert(std::make_pair(keyPtr, std::move(valuePtr)));
        }
    };
}

#endif // OBJECT_NODE_H
