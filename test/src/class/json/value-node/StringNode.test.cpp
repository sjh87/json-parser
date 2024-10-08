#include "StringNode.test.hpp"

namespace StringNodeTests {
    Test::TestGroup tests{};

    void init() {
        tests.add({ "JSON::StringNode class tests" });

        tests.add({ "JSON::StringNode::getType() returns Type::String enum value", [](){
            auto v = std::string("");
            const auto instance = JSON::StringNode(std::move(v));
            if (instance.getType() != JSON::Type::String) {
                return false;
            }

            return true;
        }});

        tests.add({ "JSON::StringNode::getValue() returns pointer to std::string = \"\" when constructor given \"\"", [](){
            auto v = std::string("");
            const auto instance = JSON::StringNode(std::move(v));

            auto strPtr = static_cast<std::string*>(instance.getValue());
            if (*(strPtr) != v) {
                return false;
            }

            return true;
        }});

        tests.add({ "JSON::StringNode constructor moves string contents from given std::string object to self", [](){
            const char* message = "potato pancakes";
            auto v = std::string(message);

            const auto instance = JSON::StringNode(std::move(v));
            auto strPtr = static_cast<std::string*>(instance.getValue());
            if (*(strPtr) != message) {
                return false;
            }

            if (!v.empty()) {
                return false; // verify storage ownership was moved to new owner
            }

            return true;
        }});

        tests.add({ "JSON::StringNode(\"Houston, we have a problem\") == JSON::StringNode(\"Houston, we have a problem\") is true", [](){
            const auto node1 = JSON::StringNode("Houston, we have a problem");
            const auto node2 = JSON::StringNode("Houston, we have a problem");

            return node1 == node2;
        }});

        tests.add({ "JSON::StringNode(\"Houston, we have a problem\") == JSON::StringNode(\"Houston, we have a program\") is false", [](){
            const auto node1 = JSON::StringNode("Houston, we have a problem");
            const auto node2 = JSON::StringNode("Houston, we have a program");

            return !(node1 == node2);
        }});

        tests.add({ "JSON::StringNode(\"Houston, we have a problem\") != JSON::StringNode(\"Houston, we have a program\") is true", [](){
            const auto node1 = JSON::StringNode("Houston, we have a problem");
            const auto node2 = JSON::StringNode("Houston, we have a program");

            return node1 != node2;
        }});

        tests.add({ "JSON::StringNode(\"Houston, we have a problem\") != JSON::StringNode(\"Houston, we have a problem\") is false", [](){
            const auto node1 = JSON::StringNode("Houston, we have a problem");
            const auto node2 = JSON::StringNode("Houston, we have a problem");

            return !(node1 != node2);
        }});
    }
}
