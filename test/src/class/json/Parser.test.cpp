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

        tests.add({ "JSON::Parser::parse() parses a '0' JSON payload", [](){
            auto parser = JSON::Parser();

            std::stringstream numboStream("0");

            auto json = parser.parse(numboStream);

            auto numboNode = static_cast<JSON::NumberNode*>(json.get());
            auto value = static_cast<double*>(numboNode->getValue());
            if (*value != 0)
                return false;

            return true;
        }});

        tests.add({ "JSON::Parser::parse() parses a '0.0' JSON payload", [](){
            auto parser = JSON::Parser();

            std::stringstream numboStream("0.0");

            auto json = parser.parse(numboStream);

            auto numboNode = static_cast<JSON::NumberNode*>(json.get());
            auto value = static_cast<double*>(numboNode->getValue());
            if (*value != 0.0)
                return false;

            return true;
        }});

        tests.add({ "JSON::Parser::parse() parses a '0.1' JSON payload", [](){
            auto parser = JSON::Parser();

            std::stringstream numboStream("0.1");

            auto json = parser.parse(numboStream);

            auto numboNode = static_cast<JSON::NumberNode*>(json.get());
            auto value = static_cast<double*>(numboNode->getValue());
            if (*value != 0.1)
                return false;

            return true;
        }});

        tests.add({ "JSON::Parser::parse() parses a '10.01' JSON payload", [](){
            auto parser = JSON::Parser();

            std::stringstream numboStream("10.01");

            auto json = parser.parse(numboStream);

            auto numboNode = static_cast<JSON::NumberNode*>(json.get());
            auto value = static_cast<double*>(numboNode->getValue());
            if (*value != 10.01)
                return false;

            return true;
        }});

        tests.add({ "JSON::Parser::parse() throws on a '00' JSON payload", [](){
            auto parser = JSON::Parser();
            const std::string expectedWhat = "0-leading numbers are not valid JSON";

            std::stringstream numboStream("00");

            try {
                parser.parse(numboStream);
                return false;
            } catch (std::runtime_error& e) {
                return e.what() == expectedWhat;
            }
        }});

        tests.add({ "JSON::Parser::parse() throws on a '.1' JSON payload", [](){
            auto parser = JSON::Parser();
            const std::string expectedWhat = "'.' is not valid JSON";

            std::stringstream numboStream(".1");

            try {
                parser.parse(numboStream);
                return false;
            } catch (std::runtime_error& e) {
                return e.what() == expectedWhat;
            }
        }});
    }
}
