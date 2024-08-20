#include "../../../src/class/json/BooleanNode.hpp"
#include "BooleanNode.test.hpp"

namespace BooleanNodeTests {
    Test::TestGroup tests{};

    void init() {
        tests.add({ "JSON::BooleanNode class tests" });

        tests.add({ "JSON::BooleanNode::getType() returns Type::Boolean enum value", [](){
            const auto instance = JSON::BooleanNode(false);
            if (instance.getType() != JSON::Type::Boolean) {
                return false;
            }

            return true;
        }});

        tests.add({ "JSON::BooleanNode::getValue() returns pointer to boolean = false when constructor called with false", [](){
            const auto instance = JSON::BooleanNode(false);
            if (*(static_cast<bool*>(instance.getValue())) != false) {
                return false;
            }

            return true;
        }});

        tests.add({ "JSON::BooleanNode::getValue() returns pointer to boolean = true when constructor called with true", [](){
            const auto instance = JSON::BooleanNode(true);
            if (*(static_cast<bool*>(instance.getValue())) != true) {
                return false;
            }

            return true;
        }});
    }
}
