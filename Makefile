EXEC_NAME := star

SRCS := main.cpp

BUILD_DIR := ./build
SRC_DIR := ./src

CXX := g++

CFLAGS := -Wall -Wextra -Wpedantic
CXXFLAGS := $(CFLAGS) -std=c++17

OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)

.PHONY: all
all: $(BUILD_DIR)/$(EXEC_NAME)

.PHONY: clean
clean:
	rm $(BUILD_DIR)/$(EXEC_NAME) $(OBJS)

$(BUILD_DIR)/$(EXEC_NAME): $(OBJS)
	$(CXX) -o $@ $(CXXFLAGS) $^

$(BUILD_DIR)/%.cpp.o: $(SRC_DIR)/%.cpp
	$(CXX) -o $@ $(CXXFLAGS) -c $^