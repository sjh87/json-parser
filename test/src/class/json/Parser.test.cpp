#include "Parser.test.hpp"

namespace ParserTests {
    Test::TestGroup tests{};

    void init() {
        tests.add({ "JSON::Parser class tests" });

        tests.add({ "JSON::Parser::parse() parses a number JSON payload", [](){
            auto parser = JSON::Parser();

            std::stringstream numboStream("12345");

            auto json = parser.parse(numboStream);

            auto numboNode = static_cast<JSON::NumberNode*>(json.get());
            auto value = static_cast<double*>(numboNode->getValue());
            if (*value != 12345)
                return false;

            return true;
        }});

        tests.add({ "JSON::Parser::parse() parses a fractional number JSON payload", [](){
            auto parser = JSON::Parser();

            std::stringstream numboStream("123.45");

            auto json = parser.parse(numboStream);

            auto numboNode = static_cast<JSON::NumberNode*>(json.get());
            auto value = static_cast<double*>(numboNode->getValue());
            if (*value != 123.45)
                return false;

            return true;
        }});

        tests.add({ "JSON::Parser::parse() parses a negative number JSON payload", [](){
            auto parser = JSON::Parser();

            std::stringstream numboStream("-12345");

            auto json = parser.parse(numboStream);

            auto numboNode = static_cast<JSON::NumberNode*>(json.get());
            auto value = static_cast<double*>(numboNode->getValue());
            if (*value != -12345)
                return false;

            return true;
        }});
    }
}
