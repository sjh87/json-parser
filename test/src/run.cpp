#include "class/ValueNodeBase.test.hpp"
#include "class/NullNode.test.hpp"
#include "class/NumberNode.test.hpp"

int main(void) {
    Test::TestRunner tr = Test::TestRunner();
    
    ValueNodeBaseTests::init();
    tr.add(ValueNodeBaseTests::tests);

    NullNodeTests::init();
    tr.add(NullNodeTests::tests);

    NumberNodeTests::init();
    tr.add(NumberNodeTests::tests);

    tr.run();
}
