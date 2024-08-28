#include "Parser.test.hpp"

#define SHOULD_THROW true

struct numberTestRow {
    const std::string payload;
    const double expected;
    const bool shouldThrow{false};
    bool only{false};
};

std::vector<numberTestRow> numberTestrows{
    { "12345", 12345 },
    { "123.45", 123.45 },
    { "0", 0 },
    { "0.0", 0 },
    { "0.1", 0.1 },
    { "10.01", 10.01 },
    { "10e12", 10e12 },
    { "-10e12", -10e12 },
    { "00", 0, SHOULD_THROW },
    { ".0", 0, SHOULD_THROW },
    { "0.", 0, SHOULD_THROW },
    { ".", 0, SHOULD_THROW },
    { " 9  ", 9 },
    { " -.1", 0, SHOULD_THROW },
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
                        return false;
                    auto numboNode = static_cast<JSON::NumberNode*>(json.get());
                    auto value = static_cast<double*>(numboNode->getValue());
                    return *value == r.expected;
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
            auto parser = JSON::Parser();

            auto json = parser.parse(sstream);
            auto boolNode = static_cast<JSON::BooleanNode*>(json.get());
            auto value = static_cast<bool*>(boolNode->getValue());
            return *value;
        }});

        tests.add({ "correctly parses false", [](){
            std::stringstream sstream(" false");
            auto parser = JSON::Parser();

            auto json = parser.parse(sstream);
            auto boolNode = static_cast<JSON::BooleanNode*>(json.get());
            auto value = static_cast<bool*>(boolNode->getValue());
            return !*value;
        }});

        tests.add({ "JSON::Parser::parse(): null" });
        tests.add({ "correctly parses null", [](){
            std::stringstream sstream(" null");
            auto parser = JSON::Parser();

            auto json = parser.parse(sstream);
            auto nullNode = static_cast<JSON::NullNode*>(json.get());
            auto value = static_cast<std::string*>(nullNode->getValue());
            return "null" == *value;
        }});

        tests.add({ "JSON::Parser::parse(): String" });
        tests.add({ "correctly parses \"pork chop sandwiches\"", [](){
            std::stringstream sstream(R"("pork chop sandwiches")");
            auto parser = JSON::Parser();

            auto json = parser.parse(sstream);
            auto stringNode = static_cast<JSON::StringNode*>(json.get());
            auto value = static_cast<std::string*>(stringNode->getValue());
            return "pork chop sandwiches" == *value;
        }});


        tests.add({ "JSON::Parser::parse(): Object" });
        tests.add({ "correctly parses {\"amount\": 3}", [](){
            std::stringstream sstream(R"({"amount": 3})");
            auto parser = JSON::Parser();

            auto json = parser.parse(sstream);
            auto objectNode = static_cast<JSON::ObjectNode*>(json.get());
            auto value = static_cast<JSON::ObjectStorageType*>(objectNode->getValue());
            if (value->size() != 1)
                return false;

            if (value->at("amount")->getType() != JSON::Type::Number)
                return false;

            auto receivedNumberPtr = static_cast<double*>(value->at("amount")->getValue());
            if (*receivedNumberPtr != 3)
                return false;

            return true;
        } });

        tests.add({ "correctly parses {\"name\": \"Big Steve\"}", [](){
            std::stringstream sstream(R"({"name": "Big Steve"})");
            auto parser = JSON::Parser();

            auto json = parser.parse(sstream);
            auto objectNode = static_cast<JSON::ObjectNode*>(json.get());
            auto value = static_cast<JSON::ObjectStorageType*>(objectNode->getValue());
            if (value->size() != 1)
                return false;

            if (value->at("name")->getType() != JSON::Type::String)
                return false;

            auto receivedNumberPtr = static_cast<std::string*>(value->at("name")->getValue());
            if (*receivedNumberPtr != "Big Steve")
                return false;

            return true;
        }});

        tests.add({ "correctly parses {\"numbos\": [1, 2, 3]}", [](){
            std::stringstream sstream(R"({"numbos": [1, 2, 3]})");
            auto parser = JSON::Parser();

            auto json = JSON::Parser().parse(sstream);
            auto objectNode = static_cast<JSON::ObjectNode*>(json.get());
            auto received = static_cast<JSON::ObjectStorageType*>(objectNode->getValue());
            if (received->size() != 1)
                return false;

            if (received->at("numbos")->getType() != JSON::Type::Array)
                return false;

            auto receivedArrayPtr = static_cast<JSON::ArrayStorageType*>(received->at("numbos")->getValue());

            if (receivedArrayPtr->size() != 3)
                return false;
            
            auto num1Ptr = static_cast<double*>(receivedArrayPtr->at(0)->getValue());
            if (receivedArrayPtr->at(0)->getType() != JSON::Type::Number || *num1Ptr != 1) {
                return false;
            }

            auto num2Ptr = static_cast<double*>(receivedArrayPtr->at(1)->getValue());
            if (receivedArrayPtr->at(1)->getType() != JSON::Type::Number || *num2Ptr != 2) {
                return false;
            }

            auto num3Ptr = static_cast<double*>(receivedArrayPtr->at(2)->getValue());
            if (receivedArrayPtr->at(2)->getType() != JSON::Type::Number || *num3Ptr != 3) {
                return false;
            }

            return true;
        }});

        tests.add({ "correctly parses {\"amount\": 3, \"category\": 2}", [](){
            std::stringstream sstream(R"({"amount": 3, "category": 2})");
            auto parser = JSON::Parser();
            auto json = parser.parse(sstream);

            auto objectNode = static_cast<JSON::ObjectNode*>(json.get());
            auto received = static_cast<JSON::ObjectStorageType*>(objectNode->getValue());
            if (received->size() != 2)
                return false;

            if (received->at("amount")->getType() != JSON::Type::Number)
                return false;

            if (received->at("category")->getType() != JSON::Type::Number)
                return false;

            auto receivedNumberPtr = static_cast<double*>(received->at("amount")->getValue());
            if (*receivedNumberPtr != 3)
                return false;

            receivedNumberPtr = static_cast<double*>(received->at("category")->getValue());
            if (*receivedNumberPtr != 2)
                return false;

            return true;
        }});

        tests.add({ "throws on {\"amount\": 3, \"category\": 2", [](){
            std::stringstream sstream(R"({"amount": 3, "category": 2)");
            auto parser = JSON::Parser();

            try {
                parser.parse(sstream);
                return false; // should have thrown
            } catch(std::runtime_error& error) {
                if (error.what() != std::string("malformed JSON"))
                    return false;
            }

            return true;
        }});

        tests.add({ "throws on \"amount\": 3, \"category\": 2}", [](){
            std::stringstream sstream(R"("amount": 3, "category": 2})");
            auto parser = JSON::Parser();

            try {
                parser.parse(sstream);
                return false; // should have thrown
            } catch(std::runtime_error& error) {
                if (error.what() != std::string("':' encountered outside of object"))
                    return false;
            }

            return true;
        }});

        tests.add({ "throws on {\"amount\": 3 \"category\": 2}", [](){
            std::stringstream sstream(R"({"amount": 3 "category": 2})");
            auto parser = JSON::Parser();

            try {
                parser.parse(sstream);
                return false; // should have thrown
            } catch(std::runtime_error& error) {
                if (error.what() != std::string("unexpectd double-quote (\")"))
                    return false;
            }

            return true;
        }});

        tests.add({ "JSON::Parser::parse(): Array" });
        tests.add({ "correctly parses [9]", [](){
            std::stringstream sstream("[9]");
            auto parser = JSON::Parser();

            auto json = parser.parse(sstream);
            auto arrayNode = static_cast<JSON::ArrayNode*>(json.get());
            auto value = static_cast<JSON::ArrayStorageType*>(arrayNode->getValue());
            if (value->size() != 1)
                return false;

            if (value->front()->getType() != JSON::Type::Number)
                return false;

            auto receivedNumberPtr = static_cast<double*>(value->front()->getValue());
            if (*receivedNumberPtr != 9)
                return false;

            return true;
        }});

        tests.add({ "throws on 9]", [](){
            std::stringstream sstream("9]");
            auto parser = JSON::Parser();

            try {
                parser.parse(sstream);
                return false; // should have thrown
            } catch(std::runtime_error& e) {
                if (e.what() != std::string("unexpected ']' encountered"))
                    return false;
            }

            return true;
        }});

        tests.add({ "throws on [9", [](){
            std::stringstream sstream("[9");
            auto parser = JSON::Parser();

            try {
                parser.parse(sstream);
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
            auto parser = JSON::Parser();

            auto json = parser.parse(sstream);
            auto arrayNode = static_cast<JSON::ArrayNode*>(json.get());
            auto value = static_cast<JSON::ArrayStorageType*>(arrayNode->getValue());
            if (value->size() != 2)
                return false;

            if (value->front()->getType() != JSON::Type::Number)
                return false;

            auto receivedNumberPtr = static_cast<double*>(value->front()->getValue());
            if (*receivedNumberPtr != 9)
                return false;

            if (value->back()->getType() != JSON::Type::Number)
                return false;

            receivedNumberPtr = static_cast<double*>(value->back()->getValue());
            if (*receivedNumberPtr != 10)
                return false;

            return true;
        }});

        tests.add({ "correctly parses [9, {\"otherNumber\": 10}]", [](){
            std::stringstream sstream(R"([9, {"otherNumber": 10}])");
            auto parser = JSON::Parser();

            auto json = parser.parse(sstream);
            auto arrayNode = static_cast<JSON::ArrayNode*>(json.get());
            auto received = static_cast<JSON::ArrayStorageType*>(arrayNode->getValue());
            if (received->size() != 2)
                return false;

            auto firstElement = static_cast<double*>(received->front()->getValue());
            if (*firstElement != 9)
                return false;
            
            auto secondElement = static_cast<JSON::ObjectStorageType*>(received->back()->getValue());
            auto secondElementMapValuePtr = static_cast<double*>(secondElement->at("otherNumber")->getValue());
            if (*secondElementMapValuePtr != 10)
                return false;

            return true;
        }});
    }
}
