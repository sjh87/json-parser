.PHONY: run build

DEBUG_FLAGS := $(sh [[ -n "$(DEBUG)" ]] && echo "-gdwarf-2 -g3 ")
SRC_DIR := ./src
BUILD_DIR := ./build

run: build
	./${BUILD_DIR}/run

build:
	mkdir -p build && g++ \
	-std=c++17 -Wall -Wextra -Wpedantic -Werror \
	${SRC_DIR}/class/json/value-node/ArrayNode.test.cpp \
	${SRC_DIR}/class/json/value-node/BooleanNode.test.cpp \
	${SRC_DIR}/class/json/value-node/NullNode.test.cpp \
	${SRC_DIR}/class/json/value-node/NumberNode.test.cpp \
	${SRC_DIR}/class/json/value-node/ObjectNode.test.cpp \
	${SRC_DIR}/class/json/value-node/StringNode.test.cpp \
	${SRC_DIR}/class/json/value-node/ValueNodeBase.test.cpp \
	${SRC_DIR}/class/json/JSON.test.cpp \
	${SRC_DIR}/run.cpp \
	$^ $(DEBUG_FLAGS) \
	-o ${BUILD_DIR}/run
