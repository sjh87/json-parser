#include "ValueNodeBase.test.hpp"

namespace ValueNodeBaseTests {
    Test::TestGroup tests{};

    void init() {
        tests.add({ "JSON::ValueNodeBase class tests" });

        tests.add({ "JSON::ValueNodeBase::getType() returns expected JSON::Type enum value", [](){
            const auto instance = JSON::ValueNodeBase();

            if (instance.getType() != JSON::Type::Empty) {
                return false;
            }

            return true;
        }});

        tests.add({ "JSON::ValueNodeBase::getValue() returns nullptr", [](){
            const auto instance = JSON::ValueNodeBase();

            if (instance.getValue() != nullptr) {
                return false;
            }

            return true;
        }});

        tests.add({ "JSON::ValueNodeBase == JSON::ValueNodeBase is false", [](){
            auto node1 = JSON::ValueNodeBase();
            auto node2 = JSON::ValueNodeBase();

            return !(node1 == node2);
        }});

        tests.add({ "JSON::ValueNodeBase != JSON::ValueNodeBase is true", [](){
            auto node1 = JSON::ValueNodeBase();
            auto node2 = JSON::ValueNodeBase();

            return node1 != node2;
        }});
    }
}
