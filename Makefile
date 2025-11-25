TARGET = raytracer

CXX = clang++
CXXFLAGS = -Wall -Wextra -std=c++17 -g
LIBS = -lsfml-graphics -lsfml-window -lsfml-system -lGL -lm

INC = -I include
INC += -I include/logger

SRC = src/main.cpp

LOGGER = src/logger/logger.cpp

#ENGINE  = src/Engine/builder/build_engine.cpp
#ENGINE += src/Engine/seter/set_config.cpp
#ENGINE += src/Engine/run.cpp
#ENGINE += src/Engine/debug/display_config.cpp
#
#PARSING  = src/ParserCmd/parse_flag.cpp
#PARSING += src/ParserCmd/getter/get_config.cpp
#PARSING += src/ParserCmd/getter/get_error.cpp
#PARSING += src/ParserCmd/flag_tab.cpp
#PARSING += src/ParserCmd/Fill_flag/check_flag_G.cpp
#PARSING += src/ParserCmd/config/builder.cpp
#
#SRC += $(ENGINE)
#SRC += $(PARSING)
SRC += $(LOGGER)

OBJ = $(SRC:.cpp=.o)

all: $(TARGET)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INC) -c $< -o $@

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) $(OBJ) -o $(TARGET) $(LIBS)

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(TARGET)

re: fclean all

.PHONY: all clean fclean re

