CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++20

SRC = main.cpp \
      inc/funcs.cpp \
      inc/structs.cpp

TARGET = connect4

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET) -lraylib

clean:
	rm -f $(TARGET)

run: $(TARGET)
	./$(TARGET)
