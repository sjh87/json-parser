.PHONY: run build

DEBUG_FLAGS := $(sh [[ -n "$(DEBUG)" ]] && echo "-gdwarf-2 -g3 ")
SRC := ../src
TEST_SRC := ./src
BUILD_DIR := ./build

run: build
	./${BUILD_DIR}/run

build:
	mkdir -p build && g++ \
	-std=c++17 -Wall -Wextra -Wpedantic -Werror \
	${TEST_SRC}/class/json/value-node/ArrayNode.test.cpp \
	${TEST_SRC}/class/json/value-node/BooleanNode.test.cpp \
	${TEST_SRC}/class/json/value-node/NullNode.test.cpp \
	${TEST_SRC}/class/json/value-node/NumberNode.test.cpp \
	${TEST_SRC}/class/json/value-node/ObjectNode.test.cpp \
	${TEST_SRC}/class/json/value-node/StringNode.test.cpp \
	${TEST_SRC}/class/json/value-node/ValueNodeBase.test.cpp \
	${TEST_SRC}/class/json/JSON.test.cpp \
	${SRC}/class/json/Parser.cpp \
	${TEST_SRC}/class/json/Parser.test.cpp \
	${TEST_SRC}/run.cpp \
	$^ $(DEBUG_FLAGS) \
	-o ${BUILD_DIR}/run
