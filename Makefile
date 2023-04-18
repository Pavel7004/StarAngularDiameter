EXEC_NAME := star
DATA_FILE := data.txt

SRCS := main.cpp theory.cpp cache.cpp

BUILD_DIR := ./build
SRC_DIR := ./src

CXX := g++

CFLAGS := -Wall -Wextra -Wpedantic -O3
CXXFLAGS := $(CFLAGS) -std=c++20

OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)

.PHONY: all
all: $(BUILD_DIR) $(EXEC_NAME)

.PHONY: plot
plot: $(BUILD_DIR) $(EXEC_NAME) $(DATA_FILE)
	cat $(DATA_FILE) | gnuplot -c graph.gp

.PHONY: clean
clean:
	rm -f $(EXEC_NAME) $(OBJS) $(DATA_FILE) out.png

$(DATA_FILE): $(EXEC_NAME)
	./$(EXEC_NAME) > $@ 	

$(BUILD_DIR):
	mkdir $(BUILD_DIR)

$(EXEC_NAME): $(OBJS)
	$(CXX) -o $@ $(CXXFLAGS) $^

$(BUILD_DIR)/%.cpp.o: $(SRC_DIR)/%.cpp
	$(CXX) -o $@ $(CXXFLAGS) -c $^
