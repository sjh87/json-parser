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

        tests.add({ "JSON::NumberNode::getValue() returns pointer to double = 0 when default constructor used", [](){
            const auto instance = JSON::NumberNode();
            if (*(static_cast<double*>(instance.getValue())) != 0) {
                return false;
            }

            return true;
        }});

        tests.add({ "JSON::NumberNode::getValue() returns pointer to double = 69 when constructor used to set value", [](){
            const auto instance = JSON::NumberNode(69);
            if (*(static_cast<double*>(instance.getValue())) != 69) {
                return false;
            }

            return true;
        }});
    }
}
