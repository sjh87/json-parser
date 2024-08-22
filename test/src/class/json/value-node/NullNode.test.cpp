#include "NullNode.test.hpp"

namespace NullNodeTests {
    Test::TestGroup tests{};

    void init() {
        tests.add({ "JSON::NullNode class tests" });

        tests.add({ "JSON::NullNode::getType() returns Type::Null enum value", [](){
            const auto instance = JSON::NullNode();
            if (instance.getType() != JSON::Type::Null) {
                return false;
            }

            return true;
        }});

        tests.add({ "JSON::NullNode::getValue() returns pointer to std::string = \"null\" when default constructor used", [](){
            const auto instance = JSON::NullNode();
            if (*(static_cast<std::string*>(instance.getValue())) != "null") {
                return false;
            }

            return true;
        }});
    }
}
