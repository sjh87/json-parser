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

        tests.add({ "ObjectNode::insert() effectively moves key and value pairs into its inner storage", [](){
            auto instance = JSON::ObjectNode();
            std::string rudeKey("BLAH BLAH BLAH WHO CARES");
            auto stringNodePtr = std::make_unique<JSON::StringNode>("beef");

            instance.insert(std::move(rudeKey), std::move(stringNodePtr));
            if (stringNodePtr || !rudeKey.empty()) { // both should have been cannibalized
                return false;
            }

            return true;
        }});
    }
}
