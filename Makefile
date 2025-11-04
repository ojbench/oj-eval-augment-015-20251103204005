CXX = g++
CXXFLAGS = -std=c++17 -O2 -Wall -Wextra
TARGET = code
SRC = main.cpp

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRC)

clean:
	rm -f $(TARGET) database.dat index.dat

.PHONY: all clean

