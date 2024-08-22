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
            auto vectorPtr = static_cast<std::vector<std::unique_ptr<JSON::ValueNodeBase>>*>(ptr);
            if (*vectorPtr != std::vector<std::unique_ptr<JSON::ValueNodeBase>>()) {
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
    }
}
