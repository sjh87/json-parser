#ifndef JSON_PARSER
#define JSON_PARSER

#include <iostream>
#include <memory>
#include <unordered_map>
#include <variant>
#include <vector>

namespace JSON {
    enum class NodeType {
        ARRAY_TYPE,
        BOOLEAN_TYPE,
        NULL_TYPE,
        NUMBER_TYPE,
        OBJECT_TYPE,
        STRING_TYPE
    };

    using Array   = std::vector<std::unique_ptr<Node>>;
    using Boolean = bool;
    using Null    = std::nullptr_t;
    using Number  = double;
    using Object  = std::unordered_map<std::string, std::unique_ptr<Node>>;
    using String  = std::string;
    using Value   = std::variant<
        Array,
        Boolean,
        Null,
        Number,
        Object,
        String
    >;

    class Node {
        NodeType type;
        Value value;

    public:
        Node(NodeType type, Value value) : type(type), value(value) {}
    };

    class JSON {
        std::unique_ptr<Node> head;
        bool valid;

    public:
        JSON(std::unique_ptr<Node> head, bool valid) : head(std::move(head)), valid(valid) {}
    };

    std::unique_ptr<JSON> parse(std::istream& byteStream) {
        // make the root Node, keep a count of { and } and make sure they are equal (0 == 0, mind you)
        // try to parse as primitive first, then array, then object
        // create a tree and pass ownership of it to a new JSON object,
        // then return a pointer to the new JSON object.
    }
};

#endif
