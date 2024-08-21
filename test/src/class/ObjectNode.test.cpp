#include "../../../src/class/json/ObjectNode.hpp"
#include "../../../src/class/json/StringNode.hpp"
#include "ObjectNode.test.hpp"

namespace ObjectNodeTests {
    using MapType = std::unordered_map<std::string, std::unique_ptr<JSON::ValueNodeBase>>;

    Test::TestGroup tests{};

    void init() {
        tests.add({ "JSON::ObjectNode class tests" });

        tests.add({ "ObjectNode::getType() returns Type::Object" , [](){
            auto instance = JSON::ObjectNode();
            if (instance.getType() != JSON::Type::Object)
                return false;

            return true;
        }});

        tests.add({ "ObjectNode::getValue() returns a pointer to an unordered_map of pointers to ValueNode objects" , [](){
            auto instance = JSON::ObjectNode();
            auto ptr = instance.getValue();

            auto mapPtr = static_cast<MapType*>(ptr);
            if ((*mapPtr) != MapType()) {
                return false;
            }

            return true;
        }});

        tests.add({ "ObjectNode::insert() takes ownership of the ValueNode pointed to by a provided unique_ptr", [](){
            auto instance = JSON::ObjectNode();
            const char* rudeKey = "BLAH BLAH BLAH WHO CARES";
            auto stringNodePtr = std::make_unique<JSON::StringNode>("");

            instance.insert(rudeKey, std::move(stringNodePtr));
            if (stringNodePtr) {
                return false;
            }

            return true;
        }, true});
    }
}
