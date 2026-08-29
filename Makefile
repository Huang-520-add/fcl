# FCL 食物链语言 —— 构建配置
# 用法: make build | make unit | make test | make coverage | make clean

CXX      ?= g++
CXXFLAGS ?= -std=c++17 -O2 -Wall -Wextra -Wpedantic
TARGET    = fcl
UNIT      = tests/unit_tests
SRCS      = src/main.cpp src/interpreter.cpp src/parser.cpp src/expr.cpp src/ecology.cpp
LIB_SRCS  = src/interpreter.cpp src/parser.cpp src/expr.cpp src/ecology.cpp
OBJS      = $(SRCS:.cpp=.o)
LIB_OBJS  = $(LIB_SRCS:.cpp=.o)

.PHONY: all build unit test coverage clean

all: build

build: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

# 单元测试：直接测试 expr / ecology / parser 模块
unit: $(UNIT)
	./$(UNIT)

$(UNIT): tests/unit_tests.cpp $(LIB_OBJS)
	$(CXX) $(CXXFLAGS) -Isrc -o $@ tests/unit_tests.cpp $(LIB_OBJS)

# 全量测试：单元 + 黑盒退出码 + 白盒输出断言
test: build unit
	bash tests/run_tests.sh
	bash tests/output_tests.sh

# 覆盖率：以 gcov 重新编译单元测试并输出覆盖率报告
coverage: clean
	$(CXX) -std=c++17 -O0 --coverage -Wall -Wextra -Wpedantic -o $(UNIT) \
	    tests/unit_tests.cpp $(LIB_SRCS)
	./$(UNIT) >/dev/null
	gcov -n $(LIB_OBJS) | grep -E "^File|^Lines" || true
	rm -f *.gcda *.gcno src/*.gcda src/*.gcno $(UNIT)

clean:
	rm -f $(TARGET) $(OBJS) $(UNIT) *.gcov *.gcda *.gcno src/*.gcda src/*.gcno
