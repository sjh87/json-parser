#include "class/ValueNodeBase.test.hpp"
#include "class/ArrayNode.test.hpp"
#include "class/BooleanNode.test.hpp"
#include "class/NullNode.test.hpp"
#include "class/NumberNode.test.hpp"
#include "class/ObjectNode.test.hpp"
#include "class/StringNode.test.hpp"

int main(void) {
    Test::TestRunner tr = Test::TestRunner();
    
    ValueNodeBaseTests::init();
    tr.add(ValueNodeBaseTests::tests);

    ArrayNodeTests::init();
    tr.add(ArrayNodeTests::tests);

    BooleanNodeTests::init();
    tr.add(BooleanNodeTests::tests);

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
