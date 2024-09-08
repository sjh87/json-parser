#ifndef JSON_H
#define JSON_H

#include "value-node/ValueNodeBase.hpp"

namespace JSON {
    class JSON {
        const std::unique_ptr<ValueNodeBase> head;

    public:
        JSON() = delete;

        JSON(JSON&) = delete;

        JSON(std::unique_ptr<ValueNodeBase>&& up) : head(std::move(up)) {}

        bool operator==(const JSON&) const;
        bool operator!=(const JSON&) const;

        Type getType() const {
            return head->getType();
        }

        ValueNodeBase* get() const {
            return head.get();
        }
    };
}

#endif // JSON_H
