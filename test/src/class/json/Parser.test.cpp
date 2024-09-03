#include "Parser.test.hpp"

#define SHOULD_THROW true

struct numberTestRow {
    const std::string payload;
    const double expected;
    const bool shouldThrow{false};
    bool only{false};
};

std::vector<numberTestRow> numberTestrows{
    { "",       0,     SHOULD_THROW },
    { " ",      0,     SHOULD_THROW },
    { " -.1",   0,     SHOULD_THROW },
    { " 9  ",   9                   },
    { "-0e12",  -0e12               },
    { "-0E12",  -0E12               },
    { "-10e12", -10e12              },
    { ".",      0,     SHOULD_THROW },
    { ".0",     0,     SHOULD_THROW },
    { "0.",     0,     SHOULD_THROW },
    { "0 .",    0,     SHOULD_THROW },
    { "0.0",    0                   },
    { "0.1",    0.1                 },
    { "0.1234", 0.1234              },
    { "0",      0                   },
    { "00",     0,     SHOULD_THROW },
    { "0E0",    0E0                 },
    { "0e12",   0e12                },
    { "1.3.4",  0,     SHOULD_THROW },
    { "10-10",  0,     SHOULD_THROW },
    { "10..0",  0,     SHOULD_THROW },
    { "10..0",  0,     SHOULD_THROW },
    { "10.01",  10.01               },
    { "10e12",  10e12               },
    { "123.45", 123.45              },
    { "12345",  12345               },
    { "1e",     0,     SHOULD_THROW },
    { "e1",     0,     SHOULD_THROW },
    { "ee",     0,     SHOULD_THROW },
    { "eE",     0,     SHOULD_THROW },
    { "Ee",     0,     SHOULD_THROW },
    { "EE",     0,     SHOULD_THROW },
    { "3 3",    0,     SHOULD_THROW },
};

namespace ParserTests {
    Test::TestGroup tests{};

    void init() {
        tests.add({ "JSON::Parser::parse(): Numbers" });

        std::for_each(numberTestrows.begin(), numberTestrows.end(), [](auto& r){
            std::string message = r.shouldThrow ? "throws when given" : "correctly parses";
            tests.add({ message + " " + r.payload, [&r](){
                auto parser = JSON::Parser();

                try {
                    std::stringstream sstream(r.payload);
                    auto json = parser.parse(sstream);
                    if (r.shouldThrow)
                        return false; // should have thrown and been caught below

                    return json == Test::createJSON<JSON::NumberNode>(r.expected);
                } catch (std::runtime_error& error) {
                    if (r.shouldThrow) {
                        return true;
                    }

                        throw error;
                }
            }, r.only});
        });

        tests.add({ "JSON::Parser::parse(): Booleans" });
        tests.add({ "correctly parses true", [](){
            std::stringstream sstream(" true ");
            auto json = JSON::Parser().parse(sstream);
            auto v = true;

            return json == Test::createJSON<JSON::BooleanNode>(v);
        }});

        tests.add({ "correctly parses false", [](){
            std::stringstream sstream(" false");
            auto json = JSON::Parser().parse(sstream);
            auto v = false;

            return json == Test::createJSON<JSON::BooleanNode>(v);
        }});

        tests.add({ "JSON::Parser::parse(): null" });
        tests.add({ "correctly parses null", [](){
            std::stringstream sstream(" null");
            auto json = JSON::Parser().parse(sstream);

            return json == Test::createJSON<JSON::NullNode>();
        }});

        tests.add({ "JSON::Parser::parse(): String" });
        tests.add({ "correctly parses \"pork chop sandwiches\"", [](){
            std::stringstream sstream(R"("pork chop sandwiches")");
            auto json = JSON::Parser().parse(sstream);

            return json == Test::createJSON<JSON::StringNode>("pork chop sandwiches");
        }});

        tests.add({ R"(throws on "\x15")", [](){
            std::stringstream sstream(R"("\x15")");

            try {
                JSON::Parser().parse(sstream);
            } catch (std::runtime_error& e) {
                return e.what() == std::string("naughty escape sequence: \\x");
            }

            return false; // should have thrown
        }});


        tests.add({ R"(throws on ["Illegal backslash escape: \017"])", [](){
            std::stringstream sstream(R"(["Illegal backslash escape: \017"])");

            try {
                JSON::Parser().parse(sstream);
            } catch (std::runtime_error& e) {
                return e.what() == std::string("naughty escape sequence: \\0");
            }

            return false; // should have thrown
        }});

        tests.add({ "correctly parses \"\"", [](){
            std::stringstream sstream(R"("")");
            auto json = JSON::Parser().parse(sstream);

            return json == Test::createJSON<JSON::StringNode>("");
        } });

        tests.add({ "JSON::Parser::parse(): Object" });
        tests.add({ "correctly parses {\"emptyMsg\": \"\"}", [](){
            std::stringstream sstream(R"({"emptyMsg": ""})");
            auto json = JSON::Parser().parse(sstream);

            JSON::ObjectStorageType map;
            map.emplace("emptyMsg", std::make_unique<JSON::StringNode>(""));

            return json == Test::createJSON<JSON::ObjectNode>(map);
        }});

        tests.add({ "correctly parses {\"blah\": 4, \"blah\": 5}", [](){
            std::stringstream sstream(R"({"blah": 4, "blah": 5})");
            auto json = JSON::Parser().parse(sstream);

            JSON::ObjectStorageType map;
            map.emplace("blah", std::make_unique<JSON::NumberNode>(5));

            return json == Test::createJSON<JSON::ObjectNode>(map);
        }});

        tests.add({ "correctly parses {\"amount\": 3}", [](){
            std::stringstream sstream(R"({"amount": 3})");
            auto json = JSON::Parser().parse(sstream);

            JSON::ObjectStorageType map;
            map.emplace("amount", std::make_unique<JSON::NumberNode>(3));

            return json == Test::createJSON<JSON::ObjectNode>(map);
        }});

        tests.add({ "correctly parses {\"\": 3}", [](){
            std::stringstream sstream(R"({"": 3})");
            auto json = JSON::Parser().parse(sstream);

            JSON::ObjectStorageType map;
            map.emplace("", std::make_unique<JSON::NumberNode>(3));

            return json == Test::createJSON<JSON::ObjectNode>(map);
        }});

        tests.add({ "correctly parses {\"name\": \"Big Steve\"}", [](){
            std::stringstream sstream(R"({"name": "Big Steve"})");
            auto json = JSON::Parser().parse(sstream);

            JSON::ObjectStorageType map;
            map.emplace("name", std::make_unique<JSON::StringNode>("Big Steve"));

            return json == Test::createJSON<JSON::ObjectNode>(map);
        }});

        tests.add({ "correctly parses {\"name\": \"\"}", [](){
            std::stringstream sstream(R"({"name": ""})");
            auto json = JSON::Parser().parse(sstream);

            JSON::ObjectStorageType map;
            map.emplace("name", std::make_unique<JSON::StringNode>(""));

            return json == Test::createJSON<JSON::ObjectNode>(map);
        }});

        tests.add({ "correctly parses {\"numbos\": [1, 2, 3]}", [](){
            std::stringstream sstream(R"({"numbos": [1, 2, 3]})");
            auto json = JSON::Parser().parse(sstream);

            JSON::ArrayStorageType numbos;
            numbos.push_back(std::make_unique<JSON::NumberNode>(1));
            numbos.push_back(std::make_unique<JSON::NumberNode>(2));
            numbos.push_back(std::make_unique<JSON::NumberNode>(3));

            JSON::ObjectStorageType map;
            map.emplace("numbos", std::make_unique<JSON::ArrayNode>(std::move(numbos)));

            return json == Test::createJSON<JSON::ObjectNode>(map);
        }});

        tests.add({ "correctly parses {\"amount\": 3, \"category\": 2}", [](){
            std::stringstream sstream(R"({"amount": 3, "category": 2})");
            auto json = JSON::Parser().parse(sstream);

            JSON::ObjectStorageType map;
            map.emplace("amount", std::make_unique<JSON::NumberNode>(3));
            map.emplace("category", std::make_unique<JSON::NumberNode>(2));

            return json == Test::createJSON<JSON::ObjectNode>(map);
        }});

        tests.add({ "throws on {\"amount\": 3, \"category\": 2", [](){
            std::stringstream sstream(R"({"amount": 3, "category": 2)");

            try {
                JSON::Parser().parse(sstream);
                return false; // should have thrown
            } catch(std::runtime_error& error) {
                if (error.what() != std::string("malformed JSON"))
                    return false;
            }

            return true;
        }});

        tests.add({ "throws on {\"amount: 3}", [](){
            std::stringstream sstream("{\"amount: 3}");

            try {
                JSON::Parser().parse(sstream);
                return false; // should have thrown
            } catch(std::runtime_error& error) {
                if (error.what() != std::string("string never terminates: \"amount: 3}")) {
                    std::cout << error.what() << std::endl;
                    return false;
                }
            }

            return true;
        }});

        tests.add({ "throws on {\"amount\": }", [](){
            std::stringstream sstream("{\"amount\": }");

            try {
                JSON::Parser().parse(sstream);
                return false; // should have thrown
            } catch(std::runtime_error& error) {
                if (error.what() != std::string("unexpected '}' encountered")) {
                    std::cout << error.what() << std::endl;
                    return false;
                }
            }

            return true;
        }});

        tests.add({ "throws on {: 3}", [](){
            std::stringstream sstream("{: 3}");

            try {
                JSON::Parser().parse(sstream);
                return false; // should have thrown
            } catch(std::runtime_error& error) {
                if (error.what() != std::string("expected key, got ':'")) {
                    std::cout << error.what() << std::endl;
                    return false;
                }
            }

            return true;
        }});

        tests.add({ "throws on {\"thing\": 2, : 3}", [](){
            std::stringstream sstream("{\"thing\": 2, : 3}");

            try {
                JSON::Parser().parse(sstream);
                return false; // should have thrown
            } catch(std::runtime_error& error) {
                if (error.what() != std::string("expected key, got ':'")) {
                    std::cout << error.what() << std::endl;
                    return false;
                }
            }

            return true;
        }});

        tests.add({ "throws on {\"thing\": 2, 3}", [](){
            std::stringstream sstream("{\"thing\": 2, 3}");

            try {
                JSON::Parser().parse(sstream);
                return false; // should have thrown
            } catch(std::runtime_error& error) {
                if (error.what() != std::string("unexpected '3' when key was expected")) {
                    std::cout << error.what() << std::endl;
                    return false;
                }
            }

            return true;
        }});

        tests.add({ "throws on {{}}", [](){
            std::stringstream sstream("{{}}");

            try {
                JSON::Parser().parse(sstream);
                return false; // should have thrown
            } catch(std::runtime_error& error) {
                if (error.what() != std::string("null key encountered while collapsing Object")) {
                    std::cout << error.what() << std::endl;
                    return false;
                }
            }

            return true;
        }});

        tests.add({ "throws on missing colon between object key and value", [](){
            std::stringstream sstream(R"({"i am a bad json object" null})");
            try {
                JSON::Parser().parse(sstream);
                return false; // should have thrown
            } catch(std::runtime_error& error) {
                if (error.what() != std::string("expected ':', saw 'n'")) {
                    std::cout << error.what() << std::endl;
                    return false;
                }
            }

            return true;
        }});

        tests.add({ "throws on {amount\": 3}", [](){
            std::stringstream sstream("{amount\": 3}");

            try {
                JSON::Parser().parse(sstream);
                return false; // should have thrown
            } catch(std::runtime_error& error) {
                if (error.what() != std::string("unexpected 'a' when key was expected")) {
                    std::cout << error.what() << std::endl;
                    return false;
                }
            }

            return true;
        }});

        tests.add({ "throws on {\"amount\": 3,}", [](){
            std::stringstream sstream("{\"amount\": 3,}");

            try {
                JSON::Parser().parse(sstream);
                return false; // should have thrown
            } catch(std::runtime_error& error) {
                if (error.what() != std::string("unexpected '}' encountered")) {
                    std::cout << error.what() << std::endl;
                    return false;
                }
            }

            return true;
        }});

        tests.add({ "throws on \"amount\": 3, \"category\": 2}", [](){
            std::stringstream sstream(R"("amount": 3, "category": 2})");

            try {
                JSON::Parser().parse(sstream);
                return false; // should have thrown
            } catch(std::runtime_error& error) {
                if (error.what() != std::string("':' encountered outside of object"))
                    return false;
            }

            return true;
        }});

        tests.add({ "throws on {\"amount\": , \"category\": 2}", [](){
            std::stringstream sstream(R"({"amount": , "category": 2})");

            try {
                JSON::Parser().parse(sstream);
                return false; // should have thrown
            } catch(std::runtime_error& error) {
                if (error.what() != std::string("unexpected ',' encountered")) {
                    std::cout << error.what() << std::endl;
                    return false;
                }
            }

            return true;
        }});

        tests.add({ "throws on {\"amount\":: 7, \"category\": 2}", [](){
            std::stringstream sstream(R"({"amount":: 7, "category": 2})");

            try {
                JSON::Parser().parse(sstream);
                return false; // should have thrown
            } catch(std::runtime_error& error) {
                if (error.what() != std::string("double colon '::' encountered")) {
                    std::cout << error.what() << std::endl;
                    return false;
                }
            }

            return true;
        }});

        tests.add({ "throws on {\"amount\": 7,, \"category\": 2}", [](){
            std::stringstream sstream(R"({"amount": 7,, "category": 2})");

            try {
                JSON::Parser().parse(sstream);
                return false; // should have thrown
            } catch(std::runtime_error& error) {
                if (error.what() != std::string("double comma ',,' encountered")) {
                    std::cout << error.what() << std::endl;
                    return false;
                }
            }

            return true;
        }});

        tests.add({ "throws on {\"amount\": 3 \"category\": 2}", [](){
            std::stringstream sstream(R"({"amount": 3 "category": 2})");

            try {
                JSON::Parser().parse(sstream);
                return false; // should have thrown
            } catch(std::runtime_error& error) {
                if (error.what() != std::string("unexpected double-quote (\")"))
                    return false;
            }

            return true;
        }});

        tests.add({ "JSON::Parser::parse(): Array" });

        tests.add({ "correctly parses [9]", [](){
            std::stringstream sstream("[9]");

            auto json = JSON::Parser().parse(sstream);

            JSON::ArrayStorageType vector;
            vector.push_back(std::make_unique<JSON::NumberNode>(9));

            return json == Test::createJSON<JSON::ArrayNode>(vector);
        }});

        tests.add({ "throws on 9]", [](){
            std::stringstream sstream("9]");

            try {
                JSON::Parser().parse(sstream);
                return false; // should have thrown
            } catch(std::runtime_error& e) {
                if (e.what() != std::string("unexpected ']' encountered"))
                    return false;
            }

            return true;
        }});

        tests.add({ "throws on [9", [](){
            std::stringstream sstream("[9");

            try {
                JSON::Parser().parse(sstream);
                return false; // should have thrown
            } catch(std::runtime_error& e) {
                if (e.what() != std::string("malformed JSON")) {
                    return false;
                }
            }

            return true;
        }});

        tests.add({ "correctly parses [9, 10]", [](){
            std::stringstream sstream("[9, 10]");

            auto json = JSON::Parser().parse(sstream);

            JSON::ArrayStorageType vector;
            vector.push_back(std::make_unique<JSON::NumberNode>(9));
            vector.push_back(std::make_unique<JSON::NumberNode>(10));

            return json == Test::createJSON<JSON::ArrayNode>(vector);
        }});

        tests.add({ "correctly parses [9, {\"otherNumber\": 10}]", [](){
            std::stringstream sstream(R"([9, {"otherNumber": 10}])");

            auto json = JSON::Parser().parse(sstream);

            JSON::ObjectStorageType map;
            map.emplace("otherNumber", std::make_unique<JSON::NumberNode>(10));

            JSON::ArrayStorageType vector;
            vector.push_back(std::make_unique<JSON::NumberNode>(9));
            vector.push_back(std::make_unique<JSON::ObjectNode>(std::move(map)));

            return json == Test::createJSON<JSON::ArrayNode>(vector);
        }});

        tests.add({ "correctly parses [{}, {}. {}]", [](){
            std::stringstream sstream(R"([{}, {}, {}])");

            auto json = JSON::Parser().parse(sstream);

            JSON::ArrayStorageType vector;
            vector.push_back(std::make_unique<JSON::ObjectNode>());
            vector.push_back(std::make_unique<JSON::ObjectNode>());
            vector.push_back(std::make_unique<JSON::ObjectNode>());

            return json == Test::createJSON<JSON::ArrayNode>(vector);
        }});

        tests.add({ "correctly parses [[], []. []]", [](){
            std::stringstream sstream(R"([[], [], []])");

            auto json = JSON::Parser().parse(sstream);

            JSON::ArrayStorageType vector;
            vector.push_back(std::make_unique<JSON::ArrayNode>());
            vector.push_back(std::make_unique<JSON::ArrayNode>());
            vector.push_back(std::make_unique<JSON::ArrayNode>());

            return json == Test::createJSON<JSON::ArrayNode>(vector);
        }});

        tests.add({ "correctly parses [[[]]]", [](){
            std::stringstream sstream(R"([[[]]])");

            auto json = JSON::Parser().parse(sstream);

            JSON::ArrayStorageType secondVector;
            secondVector.push_back(std::make_unique<JSON::ArrayNode>());

            JSON::ArrayStorageType outerVector;
            outerVector.push_back(std::make_unique<JSON::ArrayNode>(std::move(secondVector)));

            return json == Test::createJSON<JSON::ArrayNode>(outerVector);
        }});

        tests.add({"throws on [, 3]", [](){
            std::stringstream ss("[, 3]");

            try {
                JSON::Parser().parse(ss);
                return false; // should have thrown
            } catch (std::runtime_error& error) {
                if (error.what() != std::string("unexpected ',' encountered")) {
                    std::cout << "got unexpected error: " << error.what() << std::endl;
                    return false;
                }
            }
            return true;
        }});

        tests.add({"throws on [1, 2],", [](){
            std::stringstream ss("[1, 2],");

            try {
                JSON::Parser().parse(ss);
                return false; // should have thrown
            } catch (std::runtime_error& error) {
                if (error.what() != std::string("unexpected ',' encountered")) {
                    std::cout << "got unexpected error: " << error.what() << std::endl;
                    return false;
                }
            }
            return true;
        }});

        tests.add({"throws on [3, ]", [](){
            std::stringstream ss("[3, ]");

            try {
                JSON::Parser().parse(ss);
                return false; // should have thrown
            } catch (std::runtime_error& error) {
                if (error.what() != std::string("dangling comma before ']'")) {
                    std::cout << "got unexpected error: " << error.what() << std::endl;
                    return false;
                }
            }
            return true;
        }});

        tests.add({"JSON::Parser::parse(): Junk Values"});
        tests.add({"throws on 'Why, hello! I am junk. Nice to meet you!'", [](){
            std::stringstream ss("Why, hello! I am junk. Nice to meet you!");

            try {
                JSON::Parser().parse(ss);
                return false; // should have thrown
            } catch (std::runtime_error& error) {
                if (error.what() != std::string("'W' is invalid in JSON")) {
                    std::cout << "got unexpected error: " << error.what() << std::endl;
                    return false;
                }
            }
            return true;
        }});

        tests.add({"throws on 'falsy'", [](){
            std::stringstream ss("falsy");

            try {
                JSON::Parser().parse(ss);
                return false; // should have thrown
            } catch (std::runtime_error& error) {
                if (error.what() != std::string("'y' is invalid in JSON")) {
                    std::cout << "got unexpected error: " << error.what() << std::endl;
                    return false;
                }
            }
            return true;
        }});

        tests.add({"throws on 'fal3se'", [](){
            std::stringstream ss("fal3se");

            try {
                JSON::Parser().parse(ss);
                return false; // should have thrown
            } catch (std::runtime_error& error) {
                if (error.what() != std::string("'fal3se' is invalid JSON")) {
                    std::cout << "got unexpected error: " << error.what() << std::endl;
                    return false;
                }
            }
            return true;
        }});

        tests.add({"throws on '13true'", [](){
            std::stringstream ss("13true");

            try {
                JSON::Parser().parse(ss);
                return false; // should have thrown
            } catch (std::runtime_error& error) {
                if (error.what() != std::string("'13true' is invalid JSON")) {
                    std::cout << "got unexpected error: " << error.what() << std::endl;
                    return false;
                }
            }
            return true;
        }});

        tests.add({"throws on 'true13'", [](){
            std::stringstream ss("true13");

            try {
                JSON::Parser().parse(ss);
                return false; // should have thrown
            } catch (std::runtime_error& error) {
                if (error.what() != std::string("'true13' is invalid JSON")) {
                    std::cout << "got unexpected error: " << error.what() << std::endl;
                    return false;
                }
            }
            return true;
        }});

        tests.add({"throws on 'nu ll'", [](){
            std::stringstream ss("nu ll");

            try {
                JSON::Parser().parse(ss);
                return false; // should have thrown
            } catch (std::runtime_error& error) {
                if (error.what() != std::string("'nu ll' is invalid JSON")) {
                    std::cout << "got unexpected error: " << error.what() << std::endl;
                    return false;
                }
            }
            return true;
        }});


        tests.add({"throws on 'fa lse'", [](){
            std::stringstream ss("fa lse");

            try {
                JSON::Parser().parse(ss);
                return false; // should have thrown
            } catch (std::runtime_error& error) {
                if (error.what() != std::string("'fa lse' is invalid JSON")) {
                    std::cout << "got unexpected error: " << error.what() << std::endl;
                    return false;
                }
            }
            return true;
        }});

        tests.add({"throws on '1true3'", [](){
            std::stringstream ss("1true3");

            try {
                JSON::Parser().parse(ss);
                return false; // should have thrown
            } catch (std::runtime_error& error) {
                if (error.what() != std::string("'1true3' is invalid JSON")) {
                    std::cout << "got unexpected error: " << error.what() << std::endl;
                    return false;
                }
            }
            return true;
        }});
    }
}
