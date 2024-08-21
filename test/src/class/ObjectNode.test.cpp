#include "../../../src/class/json/ObjectNode.hpp"
#include "../../../src/class/json/StringNode.hpp"
#include "ObjectNode.test.hpp"

namespace ObjectNodeTests {
    Test::TestGroup tests{};

    void init() {
        tests.add({ "JSON::ObjectNode class tests" });

        tests.add({ "ObjectNode::getType() returns Type::Object" , [](){
            auto instance = JSON::ObjectNode();
            if (instance.getType() != JSON::Type::Object)
                return false;

            return true;
        }, true});
    }
}
