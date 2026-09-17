CXX := g++
CXXFLAGS := -std=c++23 -Wall -Wextra -Iinclude

TARGET := build/game

SRC := $(shell find src -name '*.cpp')
OBJ := $(SRC:src/%.cpp=build/%.o)

LIB := lib/libraylib.a

LDFLAGS := $(LIB) \
           -lGL \
           -lm \
           -lpthread \
           -ldl \
           -lrt \
           -lX11

.PHONY: build clean run

build: $(TARGET)

$(TARGET): $(OBJ)
	@mkdir -p $(dir $@)
	$(CXX) $(OBJ) $(LDFLAGS) -o $@

build/%.o: src/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

run: build
	./$(TARGET)

clean:
	rm -rf build
