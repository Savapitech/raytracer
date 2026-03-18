BIN_NAME = raytracer

KERNEL = $(shell uname)

CXX = clang++
CXXFLAGS = -g -Wall -Wextra -std=c++20 -O3

ifeq ($(KERNEL),Darwin)
SFML_FLAG = -L ./SFML-3.0.2/lib -l sfml-graphics -l sfml-window -l sfml-system
else
SFML_FLAG = $(shell pkg-config --cflags --libs sfml-graphics sfml-window sfml-system)
endif

LDLIBS = $(SFML_FLAG) -lm -lconfig++ -g

INC = -I include
INC += -I include/CmdParser
INC += -I include/logger

ifeq ($(KERNEL),Darwin)
INC += -I ./SFML-3.0.2/include
endif

BUILD_DIR := .build

SRC = src/main.cpp

##LOGGER
SRC += src/logger/logger.cpp

##CMD_PARSER
SRC += src/CmdParser/BuildConfig.cpp

##RAY_TRACER
SRC += src/RayTracer/RayTracer.cpp
SRC += src/RayTracer/buildRay.cpp

##SCENE
SRC += src/Scene/BuildScene.cpp
SRC += src/Factory/ShapeFactory.cpp
SRC += src/Factory/MaterialFactory.cpp
SRC += src/Scene/camera.cpp
SRC += src/Scene/ParseObj.cpp

##OBJECTS
SRC += src/Object/object.cpp
SRC += src/Shape/sphere.cpp
SRC += src/Shape/RectangleXZ.cpp
SRC += src/Shape/cylinder.cpp
SRC += src/Shape/triangle.cpp
SRC += src/Material/mirror.cpp
SRC += src/Material/default.cpp

#RENDER
SRC += src/Render/render.cpp
SRC += src/Render/shade.cpp
SRC += src/Render/fillRayBuffer.cpp

#BVH
SRC += src/SpacePartitionning/bvh.cpp
SRC += src/SpacePartitionning/AABB.cpp

#GRAPHIC
SRC += src/Load/graphicLoad.cpp
SRC += src/Render/graphical.cpp

.PHONY: _start all
_start: all

# call mk-profile release, SRC, additional CXXFLAGS
define mk-profile

NAME_$(strip $1) := $4
OBJ_$(strip $1) := $$($(strip $2):%.cpp=$$(BUILD_DIR)/$(strip $1)/%.o)

LIB_OBJ_$(strip $1) := $$(LIB_SRC:%.cpp=$$(BUILD_DIR)/$(strip $1)/%.o)

$$(BUILD_DIR)/$(strip $1)/%.o: %.cpp
	@ mkdir -p $$(dir $$@)
	@ $$(COMPILE.c) $$(INC) $$< -o $$@
	@ $$(LOG_TIME) "$$(C_GREEN) CC $$(C_PURPLE) $$(notdir $$@) $$(C_RESET)"

$$(NAME_$(strip $1)): CXXFLAGS += -L $$(BUILD_DIR)/$(strip $1) $3
$$(NAME_$(strip $1)): $$(OBJ_$(strip $1))
	@ $$(LINK.c) $$(OBJ_$(strip $1)) $$(INC) $$(LDFLAGS) $$(LDLIBS) -o $$@
	@ $$(LOG_TIME) "$$(C_GREEN) CC $$(C_PURPLE) $$(notdir $$@) $$(C_RESET)"
	@ $$(LOG_TIME) "$$(C_GREEN) OK  Compilation finished $$(C_RESET)"

endef

$(eval $(call mk-profile, release, SRC, , $(BIN_NAME)))

all: $(NAME_release)
ifeq ($(KERNEL),Darwin)
all:
	@ install_name_tool -add_rpath ./SFML-3.0.2/lib ./raytracer
endif

clean:
	@ $(RM) -rf .build
	@ $(LOG_TIME) "$(C_YELLOW) RM $(C_PURPLE) .build $(C_RESET)"

fclean: clean
	@ $(RM) $(NAME_release) -rf .build
	@ $(LOG_TIME) "$(C_YELLOW) RM $(C_PURPLE) .build $(NAME_release) $(C_RESET)"


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
