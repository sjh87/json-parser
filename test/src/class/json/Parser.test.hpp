#ifndef PARSER_TESTS_H
#define PARSER_TESTS_H

#include <sstream>

#include "../../../../src/class/test/TestRunner.hpp"
#include "../../../../src/class/json/JSON.hpp"
#include "../../../../src/class/json/Parser.hpp"
#include "../../../../src/class/json/value-node/NumberNode.hpp"

namespace ParserTests {
    extern Test::TestGroup tests;
    void init();
}

#endif // PARSER_TESTS_H
