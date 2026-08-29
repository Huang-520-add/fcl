# FCL 食物链语言 —— 构建配置
# 用法: make build | make test | make clean

CXX      ?= g++
CXXFLAGS ?= -std=c++17 -O2 -Wall -Wextra -Wpedantic
TARGET    = fcl
SRCS      = src/main.cpp src/interpreter.cpp src/parser.cpp src/expr.cpp src/ecology.cpp
OBJS      = $(SRCS:.cpp=.o)

.PHONY: all build test clean

all: build

build: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

test: build
	bash tests/run_tests.sh
	bash tests/output_tests.sh

clean:
	rm -f $(TARGET) $(OBJS)
