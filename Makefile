CXX = g++
CXXFLAGS = -Wall -Wextra

SRC = main.cpp \
      inc/funcs.cpp \
      inc/structs.cpp

SRC_SERVER = server/server.cpp

TARGET = connect4
TARGET_SERVER = serv

.PHONY: all clean run run_server

all: $(TARGET) $(TARGET_SERVER)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET) -lraylib

clean:
	rm $(TARGET) $(TARGET_SERVER)

run: $(TARGET)
	./$(TARGET)

$(TARGET_SERVER): $(SRC_SERVER)
	$(CXX) $(CXXFLAGS) $(SRC_SERVER) -o $(TARGET_SERVER)

run_server: $(TARGET_SERVER)
	./$(TARGET_SERVER)
