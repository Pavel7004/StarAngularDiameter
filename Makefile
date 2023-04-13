EXEC_NAME := star

SRCS := main.cpp

BUILD_DIR := ./build
SRC_DIR := ./src

CXX := g++

CFLAGS := -Wall -Wextra -Wpedantic -O4
CXXFLAGS := $(CFLAGS) -std=c++20

OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)

.PHONY: all
all: $(BUILD_DIR) $(EXEC_NAME)

.PHONY: plot
plot: $(BUILD_DIR) $(EXEC_NAME)
	gnuplot -c graph.gp	

.PHONY: dots
dots: $(BUILD_DIR) $(EXEC_NAME)
	gnuplot -c dots.gp

.PHONY: clean
clean:
	rm -f $(EXEC_NAME) $(OBJS) out.png

$(BUILD_DIR):
	mkdir build

$(EXEC_NAME): $(OBJS)
	$(CXX) -o $@ $(CXXFLAGS) $^

$(BUILD_DIR)/%.cpp.o: $(SRC_DIR)/%.cpp
	$(CXX) -o $@ $(CXXFLAGS) -c $^
