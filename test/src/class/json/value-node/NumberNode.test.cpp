#include "NumberNode.test.hpp"

namespace NumberNodeTests {
    Test::TestGroup tests{};

    void init() {
        tests.add({ "JSON::NumberNode class tests" });

        tests.add({ "JSON::NumberNode::getType() returns Type::Number enum value", [](){
            const auto instance = JSON::NumberNode(0);
            if (instance.getType() != JSON::Type::Number) {
                return false;
            }

            return true;
        }});

        tests.add({ "JSON::NumberNode::getValue() returns pointer to double = 0 when constructor given 0", [](){
            const auto instance = JSON::NumberNode(0);
            if (*(static_cast<double*>(instance.getValue())) != 0) {
                return false;
            }

            return true;
        }});

        tests.add({ "JSON::NumberNode::getValue() returns pointer to double = 12345 when constructor given 12345", [](){
            const auto instance = JSON::NumberNode(12345);
            if (*(static_cast<double*>(instance.getValue())) != 12345) {
                return false;
            }

            return true;
        }});

        tests.add({ "JSON::NumberNode(22) == JSON::NumberNode(22) is true", [](){
            auto node1 = JSON::NumberNode(22);
            auto node2 = JSON::NumberNode(22);

            return node1 == node2;
        }});

        tests.add({ "JSON::NumberNode(22) == JSON::NumberNode(21) is false", [](){
            auto node1 = JSON::NumberNode(22);
            auto node2 = JSON::NumberNode(21);

            return !(node1 == node2);
        }});

        tests.add({ "JSON::NumberNode(22) != JSON::NumberNode(22) is false", [](){
            auto node1 = JSON::NumberNode(22);
            auto node2 = JSON::NumberNode(22);

            return !(node1 != node2);
        }});

        tests.add({ "JSON::NumberNode(22) != JSON::NumberNode(21) is true", [](){
            auto node1 = JSON::NumberNode(22);
            auto node2 = JSON::NumberNode(21);

            return node1 != node2;
        }});
    }
}
