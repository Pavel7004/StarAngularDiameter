EXEC_NAME := star
DATA_FILE := data.txt

SRCS := main.cpp cache.cpp constants.cpp data.cpp theory.cpp data_stat.cpp gauss_newton.cpp

BUILD_DIR := ./build
SRC_DIR := ./src

CXX := clang++

ABSL_FLAGS := $(shell pkg-config --cflags --libs-only-l absl_flat_hash_map absl_strings absl_random_random)
FMT_FLAGS := $(shell pkg-config --cflags --libs-only-l fmt)

CFLAGS := -Wall -Wextra -Wpedantic -O3 -ggdb
CXXFLAGS := $(CFLAGS) -std=c++20 -Wundefined-func-template -fno-exceptions

OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)
LIBS :=$(ABSL_FLAGS) $(FMT_FLAGS)

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
	$(CXX) -o $@ $(CXXFLAGS) $(LIBS) $^

$(BUILD_DIR)/%.cpp.o: $(SRC_DIR)/%.cpp
	$(CXX) -o $@ $(CXXFLAGS) -c $^
