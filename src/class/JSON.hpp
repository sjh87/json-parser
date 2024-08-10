#ifndef JSON_H
#define JSON_H

#include <istream>
#include <memory>
#include <string>
#include <unordered_map>
#include <variant>
#include <vector>

namespace JSON {
    class JSON {
        std::weak_ptr<JSON> parent;

    public:
        std::shared_ptr<JSON> getParent();
        void setParent(std::shared_ptr<JSON>);
    };

    class Array: public JSON, public std::enable_shared_from_this<Array> {
        std::vector<JSON> value;
    
    public:
        void add(JSON&);
        JSON get(size_t index) const;
    };

    class Boolean : public JSON {
        bool value;

    public:
        Boolean(const bool v) : value(v) {};
    };

    class Null : public JSON {
    public:
        Null() = default;
    };

    class Number : public JSON {
        std::variant<double, int> value;

    public:
        Number(const int v) : value(v) {};
        Number(const double v) : value(v) {};
    };

    class Object: public JSON, public std::enable_shared_from_this<Object> {
        std::unordered_map<std::string, JSON> value;
    
    public:
        void add(const std::string&, JSON&);
        JSON get(const std::string&) const;
    };

    class String : public JSON {
        std::string value;

    public:
        String(const std::string&& v) : value(std::move(v)) {};
    };
}

#endif