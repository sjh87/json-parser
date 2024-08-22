#ifndef JSON_H
#define JSON_H

#include "ValueNodeBase.hpp"

namespace JSON {
    class JSON {
        const std::unique_ptr<ValueNodeBase> head;

    public:
        JSON() = delete;
        JSON(std::unique_ptr<ValueNodeBase>&& up) : head(std::move(up)) {}

        Type getType() const {
            return head->getType();
        }

        void* get() const {
            return head.get();
        }
    };
}

#endif // JSON_H
