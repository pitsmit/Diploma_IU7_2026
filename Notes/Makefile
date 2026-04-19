CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra

INCLUDES = \
    -Isrc/commands \
    -Isrc/managers \
    -Isrc/essenses \
    -Isrc/facade \
    -Isrc/repositories \
    -Iexternal/spdlog/include \
    -Iapi-stubs/api \

SRC = $(shell find src/ api-stubs/api -name "*.cpp")
OBJ = $(SRC:.cpp=.o)
DEP = $(OBJ:.o=.d)

TARGET = app

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) $(OBJ) -lsqlite3 -lpistache -pthread -o $(TARGET)

# Компиляция + генерация .d файлов
%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -MMD -MP -c $< -o $@

# Подключаем зависимости
-include $(DEP)

clean:
	rm -f $(OBJ) $(DEP) $(TARGET)