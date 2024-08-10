.PHONY: all run build

DEBUG_FLAGS := $(shell [[ -n "$(DEBUG)" ]] && echo "-gdwarf-2 -g3 ")

all: build run

run:
	./build/test

build:
	g++ test.cpp ../JSON.cpp$^ $(DEBUG_FLAGS)-std=c++17 -Wall -Wextra -Wpedantic -Werror -o build/test
