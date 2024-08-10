#include "JSON.hpp"

void JSON::Array::add(JSON& v) {
    v.setParent(this->shared_from_this());
    value.push_back(v);
}

std::shared_ptr<JSON::JSON> JSON::JSON::getParent() {
    return parent.lock();
};

void JSON::JSON::setParent(std::shared_ptr<JSON> p) {
    this->parent = p;
}

void JSON::Object::add(const std::string& k, JSON& v) {
    v.setParent(this->shared_from_this());
    value.insert(std::pair<const std::string, JSON>(k, v));
}

JSON::JSON JSON::Array::get(size_t index) const {
    return value.at(index);
}

JSON::JSON JSON::Object::get(const std::string& key) const {
    return value.at(key);
}
