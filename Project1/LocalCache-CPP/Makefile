CXX ?= g++
CXXFLAGS ?= -std=c++17 -O0 -g3 -Wall -Wextra -Wpedantic -Wconversion -Wsign-conversion -Wshadow -Wformat=2
CPPFLAGS ?=

TARGET := build/LocalCache
SOURCES := $(wildcard *.cpp)
OBJECTS := $(patsubst %.cpp,build/%.o,$(SOURCES))
DEPS := $(OBJECTS:.o=.d)

.PHONY: all debug run clean

all: debug

debug: $(TARGET)

$(TARGET): $(OBJECTS) | build
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $^ $(LDLIBS) -o $@

build/%.o: %.cpp | build
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -MMD -MP -c $< -o $@

build:
	mkdir -p $@

run: debug
	./$(TARGET)

clean:
	rm -rf build

-include $(DEPS)
