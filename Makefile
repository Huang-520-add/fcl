# FCL 食物链语言 —— 构建配置
# 用法: make build | make test | make clean

CXX      ?= g++
CXXFLAGS ?= -std=c++17 -O2 -Wall -Wextra -Wpedantic
TARGET    = fcl
SRC       = src/main.cpp

.PHONY: all build test clean

all: build

build: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -o $@ $<

test: build
	bash tests/run_tests.sh

clean:
	rm -f $(TARGET)
