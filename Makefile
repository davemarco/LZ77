CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2
TARGET = lz77
SRC = lz77.cpp

TARGET_D = lz77d
SRC_D = lz77d.cpp

all: $(TARGET) $(TARGET_D)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRC)

$(TARGET_D): $(SRC_D)
	$(CXX) $(CXXFLAGS) -o $(TARGET_D) $(SRC_D)

clean:
	rm -f $(TARGET) $(TARGET_D)
