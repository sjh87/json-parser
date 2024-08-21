.PHONY: run build

DEBUG_FLAGS := $(shell [[ -n "$(DEBUG)" ]] && echo "-gdwarf-2 -g3 ")
SRC_DIR := ./src
BUILD_DIR := ./build

run: build
	./${BUILD_DIR}/run

build:
	mkdir -p build && g++ \
	-std=c++17 -Wall -Wextra -Wpedantic -Werror \
	${SRC_DIR}/class/BooleanNode.test.cpp \
	${SRC_DIR}/class/NullNode.test.cpp \
	${SRC_DIR}/class/NumberNode.test.cpp \
	${SRC_DIR}/class/StringNode.test.cpp \
	${SRC_DIR}/class/ValueNodeBase.test.cpp \
	${SRC_DIR}/run.cpp \
	$^ $(DEBUG_FLAGS) \
	-o ${BUILD_DIR}/run
