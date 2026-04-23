BIN_NAME = raytracer

KERNEL = $(shell uname)

BUILD_DIR := .build

CXX := clang++
CXXFLAGS += $(shell cat warning_flags.txt) -Wall -Wextra -std=c++20 -O3

ifeq ($(KERNEL),Darwin)
SFML_FLAG = -L ./SFML-3.0.2/lib -l sfml-graphics -l sfml-window -l sfml-system
else
SFML_FLAG = $(shell pkg-config --cflags --libs sfml-graphics sfml-window sfml-system)
endif

LDFLAGS += $(SFML_FLAG) -lm -lconfig++ -g

SRC_DIRS = $(shell find src -type d)
CXXFLAGS += -iquote include $(addprefix -iquote, $(SRC_DIRS))

ifeq ($(KERNEL),Darwin)
INC += -I ./SFML-3.0.2/include
endif

SRC = src/main.cpp
SRC += $(shell find src/ -name "*.cpp" ! -name "main.cpp")

include utils.mk

.PHONY: _start all
_start: all

# call mk-profile release, SRC, additional CXXFLAGS
define mk-profile

NAME_$(strip $1) := $4
OBJ_$(strip $1) := $$($(strip $2):%.cpp=$$(BUILD_DIR)/$(strip $1)/%.o)

$$(BUILD_DIR)/$(strip $1)/%.o: %.cpp
	@ mkdir -p $$(dir $$@)
	@ $$(COMPILE.cpp) $$(CXXFLAGS) $$< -o $$@
	@ $$(LOG_TIME) "$$(C_GREEN) CC $$(C_PURPLE) $$(notdir $$@) $$(C_RESET)"

$$(NAME_$(strip $1)): $$(OBJ_$(strip $1))
	@ $$(LINK.cpp) $$(OBJ_$(strip $1)) $$(LDFLAGS) $$(LDLIBS) -o $$@
	@ $$(LOG_TIME) "$$(C_BLUE) LD $$(C_PURPLE) $$(notdir $$@) $$(C_RESET)"
	@ $$(LOG_TIME) "$$(C_GREEN) OK  Compilation finished $$(C_RESET)"

endef

$(eval $(call mk-profile, release, SRC, , $(BIN_NAME)))

all: $(NAME_release)
ifeq ($(KERNEL),Darwin)
all:
	@ install_name_tool -add_rpath ./SFML-3.0.2/lib ./raytracer
endif

format:
	@ find src/ -name "*.cpp" -type f -exec clang-format -i {} ";"
	@ find src/ -name "*.hpp" -type f -exec clang-format -i {} ";"
	@ $(LOG_TIME) "$(C_BLUE) CF $(C_GREEN) Code formated  $(C_RESET)"

clean:
	@ $(RM) $(OBJ)
	@ $(LOG_TIME) "$(C_YELLOW) RM $(C_PURPLE) $(OBJ) $(C_RESET)"

fclean:
	@ $(RM) -r $(NAME_release) $(NAME_debug) $(BUILD_DIR)
	@ $(LOG_TIME) "$(C_YELLOW) RM $(C_PURPLE) $(NAME_release) $(NAME_debug) \
		$(C_RESET)"

fast: CXXFLAGS += -ffast-math -march=native
fast: all

.NOTPARALLEL: re
re:	fclean all

.PHONY: all clean fclean re