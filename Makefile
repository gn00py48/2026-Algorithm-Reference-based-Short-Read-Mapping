CXX := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -O2 -Iinclude

SRC := src/main.cpp \
       src/common.cpp \
       src/algorithms/native_sliding.cpp \
       src/algorithms/kmp_seed.cpp \
       src/algorithms/hash_seed.cpp \
       src/algorithms/weighted_hash_seed.cpp

TARGET := short_read_mapping

.PHONY: all clean run

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)
	rm -rf build
