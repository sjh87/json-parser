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
    }
}
