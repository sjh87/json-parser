#include "class/ValueNodeBase.test.hpp"
#include "class/NumberNode.test.hpp"

int main(void) {
    Test::TestRunner tr = Test::TestRunner();
    
    ValueNodeBaseTests::init();
    tr.add(ValueNodeBaseTests::tests);

    NumberNodeTests::init();
    tr.add(NumberNodeTests::tests);

    tr.run();
}
