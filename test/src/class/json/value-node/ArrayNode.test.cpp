#include "ArrayNode.test.hpp"

namespace ArrayNodeTests {
    Test::TestGroup tests{};

    void init() {
        tests.add({ "JSON::ArrayNode class tests" });

        tests.add({ "ArrayNode::getType() returns Type::Array" , [](){
            auto instance = JSON::ArrayNode();
            if (instance.getType() != JSON::Type::Array)
                return false;

            return true;
        }});

        tests.add({ "ArrayNode::getValue() returns a pointer to an empty vector after initialization", [](){
            auto instance = JSON::ArrayNode();
            auto ptr = instance.getValue();
            auto vectorPtr = static_cast<JSON::ArrayStorageType*>(ptr);
            if (*vectorPtr != JSON::ArrayStorageType()) {
                return false;
            }

            return true;
        }});

        tests.add({ "ArrayNode::insert() takes a unique_ptr to an ValueNodeBase and takes ownership of it", [](){
            auto arrayNode = JSON::ArrayNode();

            auto valueNodePtr = std::make_unique<JSON::ValueNodeBase>();
            if (!valueNodePtr) {
                return false;
            }

            arrayNode.insert(std::move(valueNodePtr));
            if (valueNodePtr) {
                return false;
            }

            return true;
        }});

        tests.add({ "ValueNodes given to ArrayNode via ::insert() maintain their state when fetched and recast", [](){
            auto arrayNode = JSON::ArrayNode();
            const char* aVeryImportantMessage = "The neighbor is a Russian spy named Василий, not Earl";
            auto stringNodePtr = std::make_unique<JSON::StringNode>(aVeryImportantMessage);

            arrayNode.insert(std::move(stringNodePtr));

            auto ptrToVector = static_cast<std::vector<std::unique_ptr<JSON::ArrayNode>>*>(arrayNode.getValue());

            JSON::Type fetchedType = ptrToVector->front()->getType();
            if (fetchedType != JSON::Type::String) {
                return false;
            }

            auto ptrToString = static_cast<std::string*>(ptrToVector->front()->getValue());
            if (*(ptrToString) != aVeryImportantMessage) {
                return false;
            }

            return true;
        }});

        tests.add({ "JSON::ArrayNode({ 1, 2, 3 }) == JSON::ArrayNode({ 1, 2, 3 }) is true", [](){
            auto node1 = JSON::ArrayNode();
            node1.insert(std::make_unique<JSON::NumberNode>(1));
            node1.insert(std::make_unique<JSON::NumberNode>(2));
            node1.insert(std::make_unique<JSON::NumberNode>(3));

            auto node2 = JSON::ArrayNode();
            node2.insert(std::make_unique<JSON::NumberNode>(1));
            node2.insert(std::make_unique<JSON::NumberNode>(2));
            node2.insert(std::make_unique<JSON::NumberNode>(3));

            return node1 == node2;
        }});

        tests.add({ "JSON::ArrayNode({ 1, 2, 3 }) == JSON::ArrayNode({ 1, 2 }) is false", [](){
            auto node1 = JSON::ArrayNode();
            node1.insert(std::make_unique<JSON::NumberNode>(1));
            node1.insert(std::make_unique<JSON::NumberNode>(2));
            node1.insert(std::make_unique<JSON::NumberNode>(3));

            auto node2 = JSON::ArrayNode();
            node2.insert(std::make_unique<JSON::NumberNode>(1));
            node2.insert(std::make_unique<JSON::NumberNode>(2));

            return !(node1 == node2);
        }});

        tests.add({ "JSON::ArrayNode({ 1, 2, 3 }) == JSON::ArrayNode({ 1, 2, 4 }) is false", [](){
            auto node1 = JSON::ArrayNode();
            node1.insert(std::make_unique<JSON::NumberNode>(1));
            node1.insert(std::make_unique<JSON::NumberNode>(2));
            node1.insert(std::make_unique<JSON::NumberNode>(3));

            auto node2 = JSON::ArrayNode();
            node2.insert(std::make_unique<JSON::NumberNode>(1));
            node2.insert(std::make_unique<JSON::NumberNode>(2));
            node2.insert(std::make_unique<JSON::NumberNode>(4));

            return !(node1 == node2);
        }});

        tests.add({ "JSON::ArrayNode({ 1, 2, 3 }) != JSON::ArrayNode({ 1, 2, 3 }) is false", [](){
            auto node1 = JSON::ArrayNode();
            node1.insert(std::make_unique<JSON::NumberNode>(1));
            node1.insert(std::make_unique<JSON::NumberNode>(2));
            node1.insert(std::make_unique<JSON::NumberNode>(3));

            auto node2 = JSON::ArrayNode();
            node2.insert(std::make_unique<JSON::NumberNode>(1));
            node2.insert(std::make_unique<JSON::NumberNode>(2));
            node2.insert(std::make_unique<JSON::NumberNode>(3));

            return !(node1 != node2);
        }});

        tests.add({ "JSON::ArrayNode({ 1, 2, 3 }) != JSON::ArrayNode({ 1, 2 }) is true", [](){
            auto node1 = JSON::ArrayNode();
            node1.insert(std::make_unique<JSON::NumberNode>(1));
            node1.insert(std::make_unique<JSON::NumberNode>(2));
            node1.insert(std::make_unique<JSON::NumberNode>(3));

            auto node2 = JSON::ArrayNode();
            node2.insert(std::make_unique<JSON::NumberNode>(1));
            node2.insert(std::make_unique<JSON::NumberNode>(2));

            return node1 != node2;
        }});

        tests.add({ "JSON::ArrayNode({ 1, 2, 3 }) != JSON::ArrayNode({ 1, 2, 4 }) is true", [](){
            auto node1 = JSON::ArrayNode();
            node1.insert(std::make_unique<JSON::NumberNode>(1));
            node1.insert(std::make_unique<JSON::NumberNode>(2));
            node1.insert(std::make_unique<JSON::NumberNode>(3));

            auto node2 = JSON::ArrayNode();
            node2.insert(std::make_unique<JSON::NumberNode>(1));
            node2.insert(std::make_unique<JSON::NumberNode>(2));
            node2.insert(std::make_unique<JSON::NumberNode>(4));

            return node1 != node2;
        }});
    }
}
