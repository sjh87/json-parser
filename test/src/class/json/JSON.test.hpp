#ifndef JSON_TESTS_H
#define JSON_TESTS_H

#include "../../../../src/class/test/TestRunner.hpp"
#include "../../../../src/class/json/value-node/NumberNode.hpp"
#include "../../../../src/class/json/JSON.hpp"

namespace JSONTests {
    extern Test::TestGroup tests;
    void init();
}

#endif // JSON_TESTS_H
