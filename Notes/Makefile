CXX = g++
CXXFLAGS = -std=c++20 -Wall -Wextra

INCLUDES = \
    -Isrc/ \
    -Isrc/commands \
    -Isrc/managers \
    -Isrc/essenses \
    -Isrc/facade \
    -Isrc/repositories \
    -Isrc/core \
    -Iexternal/spdlog/include \
    -Iapi-stubs/api \

SRC = $(shell find src/ api-stubs/api -name "*.cpp")
OBJ = $(SRC:.cpp=.o)
DEP = $(OBJ:.o=.d)

TARGET = app

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) $(OBJ) -lsqlite3 -lpistache -pthread -ludev -o $(TARGET)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -MMD -MP -ludev -c $< -o $@

-include $(DEP)

clean:
	rm -f $(OBJ) $(DEP) $(TARGET)