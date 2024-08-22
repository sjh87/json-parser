#ifndef JSON_H
#define JSON_H

#include "ValueNodeBase.hpp"

namespace JSON {
    class JSON {
        const std::unique_ptr<ValueNodeBase> value;

    public:
        JSON() = delete;
        JSON(std::unique_ptr<ValueNodeBase>&& up) : value(std::move(up)) {}

        // Type getType() const {
        //     return type;
        // }

        // void* getValue() const {
        //     return value.get();
        // }
    };
}

#endif // JSON_H
