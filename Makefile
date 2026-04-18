TARGET = raytracer

KERNEL = $(shell uname)

CXX = clang++
CXXFLAGS = -g -Wall -Wextra -std=c++20 -O3

ifeq ($(KERNEL),Darwin)
SFML_FLAG = -L ./SFML-3.0.2/lib -l sfml-graphics -l sfml-window -l sfml-system
else
SFML_FLAG = $(shell pkg-config --cflags --libs sfml-graphics sfml-window sfml-system)
endif

LIBS = $(SFML_FLAG) -lm -lconfig++ -g
INC = -iquote include

SRC_DIRS = $(shell find src -type d)
INC += $(addprefix -iquote, $(SRC_DIRS))

ifeq ($(KERNEL),Darwin)
INC += -I ./SFML-3.0.2/include
endif

SRC = src/main.cpp
SRC += $(shell find src/ -name "*.cpp" ! -name "main.cpp")

OBJ = $(SRC:.cpp=.o)

all: $(TARGET)
ifeq ($(KERNEL),Darwin)
all:
	@ install_name_tool -add_rpath ./SFML-3.0.2/lib ./raytracer
endif

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INC) -c $< -o $@

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) $(OBJ) -o $(TARGET) $(LIBS)

clean:
	@ $(RM) $(OBJ)
	@ $(LOG_TIME) "$(C_YELLOW) RM $(C_PURPLE) $(OBJ) $(C_RESET)"

fclean: clean
	@ $(RM) $(TARGET)
	@ $(LOG_TIME) "$(C_YELLOW) RM $(C_PURPLE) $(TARGET) $(C_RESET)"

fast: CXXFLAGS += -ffast-math -march=native
fast: all

.NOTPARALLEL: re
re:	fclean all

.PHONY: all clean fclean re

PREFIX ?=
BINDIR ?= $(PREFIX)/bin

.PHONY: install uninstall
install: $(NAME_release)
	install -Dm755 -t $(BINDIR) $(NAME_release)

uninstall:
	$(RM) $(BINDIR)/$(NAME_release)

V ?= 0
ifneq ($(V),0)
  Q :=
else
  Q := @
endif

ifneq ($(shell command -v tput),)
  ifneq ($(shell tput colors),0)

mk-color = \e[$(strip $1)m

C_BEGIN := \033[A
C_RESET := $(call mk-color, 00)

C_RED := $(call mk-color, 31)
C_GREEN := $(call mk-color, 32)
C_YELLOW := $(call mk-color, 33)
C_BLUE := $(call mk-color, 34)
C_PURPLE := $(call mk-color, 35)
C_CYAN := $(call mk-color, 36)

  endif
endif

NOW = $(shell date +%s%3N)

ifndef STIME
STIME := $(call NOW)
endif

TIME_NS = $(shell expr $(call NOW) - $(STIME))
TIME_MS = $(shell expr $(call TIME_NS))

BOXIFY = "[$(C_BLUE)$(1)$(C_RESET)] $(2)"

ifneq ($(shell command -v printf),)
  LOG_TIME = printf $(call BOXIFY, %6s ,$(strip %b\n)) "$(call TIME_MS)"
else
  LOG_TIME = echo -e $(call BOXIFY, $(call TIME_MS) ,)
endif
