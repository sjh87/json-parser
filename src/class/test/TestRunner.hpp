#ifndef TEST_RUNNER_H
#define TEST_RUNNER_H

#define COLOR_RED "\033[1;31m"
#define COLOR_GREEN "\033[1;32m"
#define COLOR_YELLOW "\033[1;33m"
#define RESET_COLOR "\033[0m"

#include <functional>
#include <iostream>
#include <memory>
#include <queue>
#include <string>

#include "../json/JSON.hpp"

namespace Test {
    constexpr bool ISOLATE_TEST = true;
    struct TestCase {
        std::string message;
        std::function<bool()> function{nullptr};
        bool onlyThis{false};
    };

    struct TestGroup {
        std::queue<TestCase> queue{};
        bool isolate{false};

        void add(const TestCase& tc) {
            if (tc.onlyThis)
                isolate = true;

            queue.push(tc);
        }
    };

    template<class NodeType, typename type>
    JSON::JSON createJSON(type& value) {
        return JSON::JSON(std::make_unique<NodeType>(std::move(value)));
    }

    template<class NodeType>
    JSON::JSON createJSON() {
        return JSON::JSON(std::make_unique<NodeType>());
    }

    class TestRunner {
        std::queue<TestCase> queue;
        size_t failedCount;
        size_t passedCount;
        bool isolate;

    public:
        TestRunner() : failedCount(0), passedCount(0), isolate(false) {}

        void add(TestGroup& tg) {
            if (tg.queue.size() < 2 && !(tg.queue.front().function)) {
                return;
            }

            if (tg.isolate) {
                isolate = true;
            }

            while (!tg.queue.empty()) {
                auto tc = tg.queue.front();
                tg.queue.pop();

                if (tg.isolate && !(tc.function))
                    tc.onlyThis = true;

                queue.push(tc);
            }
        }

        void add(const TestCase& tc) {
            if (tc.onlyThis)
                isolate = true;

            queue.push(tc);
        }

        bool run() {
            while(!queue.empty()) {
                auto tc = queue.front();
                queue.pop();

                if (isolate && !tc.onlyThis) {
                    continue;
                }

                if (!tc.function) {
                    std::cout <<
                    COLOR_YELLOW <<
                    "-------------------\n" <<
                    tc.message << '\n'      <<
                    "-------------------"   <<
                    RESET_COLOR << std::endl;
                    continue;
                }

                bool result = tc.function();
                result ? passedCount++ : failedCount++;

                std::cout <<
                    (result ? COLOR_GREEN : COLOR_RED)
                    << (result ? "Passed: " : "Failed: ")
                    << tc.message
                    << RESET_COLOR
                    << std::endl;
            }

            std::cout               <<
            '\n'                    <<
            COLOR_YELLOW            <<
            "-------------------\n" <<
            "Results\n"             <<
            "-------------------\n" <<
            COLOR_GREEN << "Passed: " << passedCount << "; " <<
            COLOR_RED   << "Failed: " << failedCount <<
            RESET_COLOR << '\n' << std::endl;

            return failedCount == 0;
        }
    };
};

#endif // TEST_RUNNER_H
