#include "../../../src/class/json/JSON.hpp"
#include "../../../src/class/json/NumberNode.hpp"
#include "JSON.test.hpp"

namespace JSONTests {
    Test::TestGroup tests{};

    void init() {
        tests.add({ "JSON::JSON class tests" });

        tests.add({ "JSON::JSON's constructor takes a unique_ptr to a JSON::ValueNodeBase-derived class member and takes ownership of the latter", [](){
            auto numberNodePtr = std::make_unique<JSON::NumberNode>(7);

            JSON::JSON(std::move(numberNodePtr));
            if(numberNodePtr)
                return false;

            return true;
        }});

        tests.add({ "JSON::JSON::getType() returns the Type enum value of its top-most value node", [](){
            auto numberNodePtr = std::make_unique<JSON::NumberNode>(7);
            auto json = JSON::JSON(std::move(numberNodePtr));

            if (json.getType() != JSON::Type::Number)
                return false;

            return true;
        }});

        tests.add({ "JSON::JSON::get() returns a pointer to its top-most value node", [](){
            const double numbo = 7;
            auto numberNodePtr = std::make_unique<JSON::NumberNode>(numbo);
            auto json = JSON::JSON(std::move(numberNodePtr));

            auto ptr = static_cast<JSON::NumberNode*>(json.get());
            if (!ptr)
                return false;
            
            auto value = static_cast<double*>(ptr->getValue());
            if (*value != numbo)
                return false;

            return true;
        }});
    }
}
