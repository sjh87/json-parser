#include "ObjectNode.test.hpp"

namespace ObjectNodeTests {
    using MapType = std::unordered_map<std::string, std::unique_ptr<JSON::ValueNodeBase>>;

    Test::TestGroup tests{};

    void init() {
        tests.add({ "JSON::ObjectNode class tests" });

        tests.add({ "ObjectNode::getType() returns Type::Object" , [](){
            auto instance = JSON::ObjectNode();

            return instance.getType() == JSON::Type::Object;
        }});

        tests.add({ "ObjectNode::getValue() returns a pointer to an unordered_map of pointers to ValueNode objects" , [](){
            auto instance = JSON::ObjectNode();
            auto ptr = instance.getValue();

            auto mapPtr = static_cast<MapType*>(ptr);

            return *mapPtr == MapType();
        }});

        tests.add({ "ObjectNode::insert() effectively moves key and value pairs into its inner storage", [](){
            auto instance = JSON::ObjectNode();
            auto rudeKey = std::make_unique<std::string>("BLAH BLAH BLAH WHO CARES");
            auto stringNodePtr = std::make_unique<JSON::StringNode>("beef");

            instance.insert(std::move(rudeKey), std::move(stringNodePtr));

            return !stringNodePtr && rudeKey->empty();
        }});

        tests.add({ "ObjectNode::insert() replaces value at key if key already exists", [](){
            auto instance = JSON::ObjectNode();
            auto value1Ptr = std::make_unique<JSON::StringNode>("beef");
            auto value2Ptr = std::make_unique<JSON::StringNode>("beefy beef beef");

            instance.insert(std::make_unique<std::string>("beefkey"), std::move(value1Ptr));
            auto vector = static_cast<JSON::ObjectStorageType*>(instance.getValue());
            auto string = static_cast<std::string*>(vector->at("beefkey")->getValue());
            if (*string != "beef")
                return false;

            instance.insert(std::make_unique<std::string>("beefkey"), std::move(value2Ptr));
            vector = static_cast<JSON::ObjectStorageType*>(instance.getValue());
            string = static_cast<std::string*>(vector->at("beefkey")->getValue());

            return *string == "beefy beef beef";
        }});

        tests.add({ "JSON::ObjectNode({{ \"pork\": true }, {\"chop\": false}}) == JSON::ObjectNode({{ \"pork\": true }, {\"chop\": false}}) is true", [](){
            auto node1 = JSON::ObjectNode();
            node1.insert(
                std::move(std::make_unique<std::string>("pork")),
                std::make_unique<JSON::BooleanNode>(true)
            );

            node1.insert(
                std::move(std::make_unique<std::string>("chop")),
                std::make_unique<JSON::BooleanNode>(false)
            );

            auto node2 = JSON::ObjectNode();
            node2.insert(
                std::move(std::make_unique<std::string>("pork")),
                std::make_unique<JSON::BooleanNode>(true)
            );

            node2.insert(
                std::move(std::make_unique<std::string>("chop")),
                std::make_unique<JSON::BooleanNode>(false)
            );

            return node1 == node2;
        }});

        tests.add({ "JSON::ObjectNode({{ \"pork\": true }}) == JSON::ObjectNode({{ \"pork\": true }, {\"chop\": false}}) is false", [](){
            auto node1 = JSON::ObjectNode();
            node1.insert(
                std::move(std::make_unique<std::string>("pork")),
                std::make_unique<JSON::BooleanNode>(true)
            );

            auto node2 = JSON::ObjectNode();
            node2.insert(
                std::move(std::make_unique<std::string>("pork")),
                std::make_unique<JSON::BooleanNode>(true)
            );

            node2.insert(
                std::move(std::make_unique<std::string>("chop")),
                std::make_unique<JSON::BooleanNode>(false)
            );

            return !(node1 == node2);
        }});

        tests.add({ "JSON::ObjectNode({{ \"pork\": true }, {\"chop\": false}}) == JSON::ObjectNode({{ \"pork\": true }, {\"chop\": true}}) is false", []() {
            auto node1 = JSON::ObjectNode();
            node1.insert(
                std::move(std::make_unique<std::string>("pork")),
                std::make_unique<JSON::BooleanNode>(true)
            );

            node1.insert(
                std::move(std::make_unique<std::string>("chop")),
                std::make_unique<JSON::BooleanNode>(false)
            );

            auto node2 = JSON::ObjectNode();
            node2.insert(
                std::move(std::make_unique<std::string>("pork")),
                std::make_unique<JSON::BooleanNode>(true)
            );

            node2.insert(
                std::move(std::make_unique<std::string>("chop")),
                std::make_unique<JSON::BooleanNode>(true)
            );

            return !(node1 == node2);
        }});

        tests.add({ "JSON::ObjectNode({{ \"pork\": true }, {\"loin\": false}}) == JSON::ObjectNode({{ \"pork\": true }, {\"chop\": false}}) is false", []() {
            auto node1 = JSON::ObjectNode();
            node1.insert(
                std::move(std::make_unique<std::string>("pork")),
                std::make_unique<JSON::BooleanNode>(true)
            );

            node1.insert(
                std::move(std::make_unique<std::string>("loin")),
                std::make_unique<JSON::BooleanNode>(false)
            );

            auto node2 = JSON::ObjectNode();
            node2.insert(
                std::move(std::make_unique<std::string>("pork")),
                std::make_unique<JSON::BooleanNode>(true)
            );

            node2.insert(
                std::move(std::make_unique<std::string>("chop")),
                std::make_unique<JSON::BooleanNode>(false)
            );

            return !(node1 == node2);
        }});

        tests.add({ "JSON::ObjectNode({{ \"pork\": true }, {\"chop\": false}}) != JSON::ObjectNode({{ \"pork\": true }, {\"chop\": true}}) is true", [](){
            auto node1 = JSON::ObjectNode();
            node1.insert(
                std::move(std::make_unique<std::string>("pork")),
                std::make_unique<JSON::BooleanNode>(true)
            );

            node1.insert(
                std::move(std::make_unique<std::string>("chop")),
                std::make_unique<JSON::BooleanNode>(false)
            );

            auto node2 = JSON::ObjectNode();
            node2.insert(
                std::move(std::make_unique<std::string>("pork")),
                std::make_unique<JSON::BooleanNode>(true)
            );

            node2.insert(
                std::move(std::make_unique<std::string>("chop")),
                std::make_unique<JSON::BooleanNode>(true)
            );

            return node1 != node2;
        }});

        tests.add({ "JSON::ObjectNode({{ \"pork\": true }, {\"chop\": false}}) != JSON::ObjectNode({{ \"pork\": true }, {\"chop\": false}}) is false", [](){
            auto node1 = JSON::ObjectNode();
            node1.insert(
                std::move(std::make_unique<std::string>("pork")),
                std::make_unique<JSON::BooleanNode>(true)
            );

            node1.insert(
                std::move(std::make_unique<std::string>("chop")),
                std::make_unique<JSON::BooleanNode>(false)
            );

            auto node2 = JSON::ObjectNode();
            node2.insert(
                std::move(std::make_unique<std::string>("pork")),
                std::make_unique<JSON::BooleanNode>(true)
            );

            node2.insert(
                std::move(std::make_unique<std::string>("chop")),
                std::make_unique<JSON::BooleanNode>(false)
            );

            return !(node1 != node2);
        }});

        tests.add({ "JSON::ObjectNode({{ \"pork\": true }}) != JSON::ObjectNode({{ \"pork\": true }, {\"chop\": false}}) is true", [](){
            auto node1 = JSON::ObjectNode();
            node1.insert(
                std::move(std::make_unique<std::string>("pork")),
                std::make_unique<JSON::BooleanNode>(true)
            );

            auto node2 = JSON::ObjectNode();
            node2.insert(
                std::move(std::make_unique<std::string>("pork")),
                std::make_unique<JSON::BooleanNode>(true)
            );

            node2.insert(
                std::move(std::make_unique<std::string>("chop")),
                std::make_unique<JSON::BooleanNode>(false)
            );

            return node1 != node2;
        }});

        tests.add({ "JSON::ObjectNode({{ \"pork\": true }, {\"loin\": false}}) != JSON::ObjectNode({{ \"pork\": true }, {\"chop\": false}}) is true", []() {
            auto node1 = JSON::ObjectNode();
            node1.insert(
                std::move(std::make_unique<std::string>("pork")),
                std::make_unique<JSON::BooleanNode>(true)
            );

            node1.insert(
                std::move(std::make_unique<std::string>("loin")),
                std::make_unique<JSON::BooleanNode>(false)
            );

            auto node2 = JSON::ObjectNode();
            node2.insert(
                std::move(std::make_unique<std::string>("pork")),
                std::make_unique<JSON::BooleanNode>(true)
            );

            node2.insert(
                std::move(std::make_unique<std::string>("chop")),
                std::make_unique<JSON::BooleanNode>(false)
            );

            return node1 != node2;
        }});
    }
}
