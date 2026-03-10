TARGET = raytracer

CXX = clang++
CXXFLAGS = -Wall -Wextra -std=c++20 -O3
LIBS = -lsfml-graphics -lsfml-window -lsfml-system -lGL -lm -lconfig++ -g

INC = -I include
INC += -I include/CmdParser
INC += -I include/logger

SRC = src/main.cpp

LOGGER = src/logger/logger.cpp

CMDPARSER += src/CmdParser/BuildConfig.cpp

RAYTRACER = src/RayTracer/RayTracer.cpp
RAYTRACER += src/RayTracer/buildRay.cpp

SCENE = src/Scene/BuildScene.cpp
SCENE += src/Factory/ShapeFactory.cpp
SCENE += src/Factory/MaterialFactory.cpp
SCENE += src/Scene/camera.cpp
SCENE += src/Scene/ParseObj.cpp

PLUGIN = src/Object/object.cpp
PLUGIN += src/Shape/sphere.cpp
PLUGIN += src/Shape/RectangleXZ.cpp
PLUGIN += src/Shape/triangle.cpp
PLUGIN += src/Material/mirror.cpp
PLUGIN += src/Material/default.cpp

RENDER = src/Render/render.cpp
RENDER += src/Render/shade.cpp
RENDER += src/Render/fillRayBuffer.cpp

BVH += src/SpacePartitionning/bvh.cpp
BVH += src/SpacePartitionning/AABB.cpp

GRAPHICAL = src/Load/graphicLoad.cpp

SRC += $(LOGGER)
SRC += $(CMDPARSER)
SRC += $(SCENE)
SRC += $(RAYTRACER)
SRC += $(PLUGIN)
SRC += $(RENDER)
SRC += $(BVH)
SRC += $(GRAPHICAL)

OBJ = $(SRC:.cpp=.o)

all: $(TARGET)

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
