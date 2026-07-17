
# VoideEngine Makefile
# - Builds sources under src/
# - Uses headers from include/
# - Places executable into ../../bin (consistent with workspace root Makefile)

CXX := g++
CC  := gcc
CXXFLAGS := -Wall -Wextra -std=c++17 -Iinclude
LDFLAGS :=

BIN_DIR := ../../bin
APP_NAME := voide_engine
TARGET := $(BIN_DIR)/$(APP_NAME)

SRCS_CPP := $(wildcard src/*.cpp)
SRCS_C   := $(wildcard src/*.c)

.PHONY: all clean run

all: $(TARGET)

$(TARGET): $(SRCS_CPP) $(SRCS_C)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $(SRCS_CPP) $(SRCS_C) -o $(TARGET) $(LDFLAGS)
	@echo "✅ Built: $(TARGET)"

run: all
	@echo "\n🚀 Running: $(TARGET)"
	@"$(TARGET)"

clean:
	@rm -f $(TARGET)
	@echo "🧹 Cleaned: $(TARGET)"

# Local debug target
debug: CXXFLAGS += -g -O0
debug: clean all

# Release target
release: CXXFLAGS += -O2
release: clean all

