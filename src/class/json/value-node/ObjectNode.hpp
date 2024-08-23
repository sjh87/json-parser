#ifndef OBJECT_NODE_H
#define OBJECT_NODE_H

#include "ValueNodeBase.hpp"

namespace JSON {
    using ObjectStorageType = std::unordered_map<std::string, std::unique_ptr<ValueNodeBase>>;
    class ObjectNode : public ValueNodeBase {

        const Type type;
        const std::unique_ptr<ObjectStorageType> value;

    public:
        ObjectNode() : type(Type::Object), value(std::make_unique<ObjectStorageType>()) {}

        Type getType() const override {
            return type;
        }

        // may want to overload this to take a string x and return a pointer to
        // the value y at x, but that's a nice-to-have feature and I want to
        // keep the interface across ValueNode derivatives the same for now
        void* getValue() const override {
            return value.get();
        }

        void insert(std::string&& key, std::unique_ptr<ValueNodeBase>&& valuePtr) {
            value->insert(std::make_pair(std::move(key), std::move(valuePtr)));
        }
    };
}

#endif // OBJECT_NODE_H
