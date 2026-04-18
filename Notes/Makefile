CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra
INCLUDES = -Isrc/commands -Isrc/managers -Isrc/essenses -Isrc/facade -Isrc/repositories -Iexternal/spdlog/include

# Все cpp файлы
SRC = $(shell find src/ -name "*.cpp")

# Объектные файлы
OBJ = $(SRC:.cpp=.o)

TARGET = app

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) $(OBJ) -lsqlite3 -o $(TARGET)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)