#include "class/json/value-node/ValueNodeBase.test.hpp"
#include "class/json/value-node/ArrayNode.test.hpp"
#include "class/json/value-node/BooleanNode.test.hpp"
#include "class/json/JSON.test.hpp"
#include "class/json/value-node/NullNode.test.hpp"
#include "class/json/value-node/NumberNode.test.hpp"
#include "class/json/value-node/ObjectNode.test.hpp"
#include "class/json/value-node/StringNode.test.hpp"

int main(void) {
    Test::TestRunner tr = Test::TestRunner();
    
    ValueNodeBaseTests::init();
    tr.add(ValueNodeBaseTests::tests);

    ArrayNodeTests::init();
    tr.add(ArrayNodeTests::tests);

    BooleanNodeTests::init();
    tr.add(BooleanNodeTests::tests);

    JSONTests::init();
    tr.add(JSONTests::tests);

    NullNodeTests::init();
    tr.add(NullNodeTests::tests);

    NumberNodeTests::init();
    tr.add(NumberNodeTests::tests);

    ObjectNodeTests::init();
    tr.add(ObjectNodeTests::tests);

    StringNodeTests::init();
    tr.add(StringNodeTests::tests);

    return !tr.run();
}
