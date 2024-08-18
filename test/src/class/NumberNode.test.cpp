#include "../../../src/class/json/NumberNode.hpp"
#include "NumberNode.test.hpp"

namespace NumberNodeTests {
    Test::TestGroup tests{};

    void init() {
        tests.add({ "JSON::NumberNode class tests" });

        tests.add({ "JSON::NumberNode::getType() returns Type::NumberNode enum value", [](){
            const auto instance = JSON::NumberNode();
            if (instance.getType() != JSON::Type::Number) {
                return false;
            }

            return true;
        }});
    }
}
