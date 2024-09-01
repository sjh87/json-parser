#ifndef OBJECT_NODE_H
#define OBJECT_NODE_H

#include <algorithm>

#include "ArrayNode.hpp"
#include "BooleanNode.hpp"
#include "NullNode.hpp"
#include "NumberNode.hpp"
#include "ObjectNode.hpp"
#include "StringNode.hpp"
#include "ValueNodeBase.hpp"

namespace JSON {
    using ObjectStorageType = std::unordered_map<std::string, std::unique_ptr<ValueNodeBase>>;
    class ObjectNode : public ValueNodeBase {

        const Type type;
        const std::unique_ptr<ObjectStorageType> value;

    public:
        ObjectNode() : type(Type::Object), value(std::make_unique<ObjectStorageType>()) {}

        bool operator==(const ValueNodeBase& other) const override;
        bool operator!=(const ValueNodeBase& other) const override;

        Type getType() const override {
            return type;
        }

        // may want to overload this to take a string x and return a pointer to
        // the value y at x, but that's a nice-to-have feature and I want to
        // keep the interface across ValueNode derivatives the same for now
        void* getValue() const override {
            return value.get();
        }

        void insert(std::unique_ptr<std::string>&& key, std::unique_ptr<ValueNodeBase>&& valuePtr) {
            if (!key)
                throw std::runtime_error("null pointer provided as key to ObjectNode::insert");
            else if (!value)
                throw std::runtime_error("null pointer provided as value to ObjectNode::insert");

            auto pos = value->find(*key);
            if (pos != value->end()) {
                // weird JSON spec says that the last-occuring value for a given
                // key is "the" value for said key
                pos->second = std::move(valuePtr);
                return;
            }

            value->insert(std::make_pair(std::move(*key), std::move(valuePtr)));
        }
    };
}

#endif // OBJECT_NODE_H
