EXEC_NAME := star
DATA_FILE := data.txt

SRCS := main.cpp cache.cpp constants.cpp data.cpp theory.cpp matrix.cpp

BUILD_DIR := ./build
SRC_DIR := ./src

CXX := g++

ABSL_FLAGS := $(shell pkg-config --cflags --libs-only-l absl_flat_hash_map absl_strings)
FMT_FLAGS := $(shell pkg-config --cflags --libs-only-l fmt)

CFLAGS := -Wall -Wextra -Wpedantic -O3
CXXFLAGS := $(CFLAGS) -std=c++2b -fno-exceptions -Wno-pre-c++2b-compat

OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)

.PHONY: all
all: $(BUILD_DIR) $(EXEC_NAME)

.PHONY: plot
plot: $(BUILD_DIR) $(EXEC_NAME) $(DATA_FILE)
	gnuplot -c graph.gp

.PHONY: clean
clean:
	rm -f $(EXEC_NAME) $(OBJS) $(DATA_FILE) out.png

$(DATA_FILE): $(EXEC_NAME)
	./$(EXEC_NAME) > $@ 	

$(BUILD_DIR):
	mkdir $(BUILD_DIR)

$(EXEC_NAME): $(OBJS)
	$(CXX) -o $@ $(CXXFLAGS) $(ABSL_FLAGS) $(FMT_FLAGS) $^

$(BUILD_DIR)/%.cpp.o: $(SRC_DIR)/%.cpp
	$(CXX) -o $@ $(CXXFLAGS) -c $^
