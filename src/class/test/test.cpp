#include "../JSON.hpp"

#include <algorithm>
#include <functional>
#include <iostream>
#include <memory>
#include <sstream>
#include <queue>
#include <vector>

#define COLOR_RED "\033[1;31m"
#define COLOR_GREEN "\033[1;32m"
#define RESET_COLOR "\033[0m"

struct TestCase {
    std::string message;
    std::function<bool()> function;
    bool only{false};
};

class TestRunner {
    std::queue<TestCase> queue;
    size_t failed;
    size_t passed;
    bool isolate;

public:
    TestRunner() : failed(0), passed(0), isolate(false){}

    void add(const TestCase& tc) {
        if (tc.only)
            isolate = true;

        queue.push(tc);
    }

    void run() {
        while(!queue.empty()) {
            auto tc = queue.front();
            queue.pop();

            if (isolate && !tc.only) {
                continue;
            }

            bool result = tc.function();

            std::cout <<
                (result ? COLOR_GREEN : COLOR_RED)
                << (result ? "Passed: " : "Failed: ")
                << tc.message
                << RESET_COLOR
                << std::endl;
        }
    }
};

int main(void) {
    auto tr = TestRunner();

    tr.add({ "Array children point to parent Array instance", []() {
        auto array = std::make_shared<JSON::Array>();

        auto number1 = JSON::Number(1);
        auto number2 = JSON::Number(2);

        if (number1.getParent() != nullptr || number2.getParent() != nullptr) {
            return false;
        }

        array->add(number1);
        if (array->get(0).getParent() != array) {
            return false;
        }

        array->add(number2);
        if (array->get(1).getParent() != array) {
            return false;
        }

        return true;
    }});

    tr.add({ "Object children point to parent Object instance", []() {
        auto object = std::make_shared<JSON::Object>();

        auto number1 = JSON::Number(1);
        auto number2 = JSON::Number(2);

        if (number1.getParent() != nullptr || number2.getParent() != nullptr) {
            return false;
        }

        object->add("number 1", number1);
        if (object->get("number 1").getParent() != object) {
            return false;
        }

        object->add("number 2", number2);
        if (object->get("number 2").getParent() != object) {
            return false;
        }

        return true;
    }});

    tr.run();
}
