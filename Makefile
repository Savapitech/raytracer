MAKE_FLAGS += -j

BIN_NAME := raytracer

SRC := $(wildcard src/*.cpp)
SRC += $(wildcard src/*/*.cpp)

BUILD_DIR := .build

CFLAGS += -std=c++17 -iquote include/CmdConfig -iquote include/logger -iquote include/CmdParser -iquote include/Plugin -iquote include/RayTracer
CFLAGS += $(shell cat $/warning_flags.txt)

LDFLAGS += -lsfml-graphics -lsfml-window -lsfml-system -lm 

.PHONY: _start all
_start: all

# call mk-profile release, SRC, additional CFLAGS
define mk-profile

NAME_$(strip $1) := $4
OBJ_$(strip $1) := $$($(strip $2):%.cpp=$$(BUILD_DIR)/$(strip $1)/%.o)

$$(BUILD_DIR)/$(strip $1)/%.o: %.cpp
	@ mkdir -p $$(dir $$@)
	$$(COMPILE.cpp) $$(CFLAGS) $$< -o $$@
	@ $$(LOG_TIME) "$$(C_GREEN) CC $$(C_PURPLE) $$(notdir $$@) $$(C_RESET)"

$$(NAME_$(strip $1)): $$(OBJ_$(strip $1))
	$$(LINK.cpp) $$(OBJ_$(strip $1)) $$(LDFLAGS) $$(LDLIBS) -o $$@
	@ $$(LOG_TIME) "$$(C_GREEN) CC $$(C_PURPLE) $$(notdir $$@) $$(C_RESET)"
	@ $$(LOG_TIME) "$$(C_GREEN) OK  Compilation finished $$(C_RESET)"

endef

$(eval $(call mk-profile, release, SRC, , $(BIN_NAME)))
$(eval $(call mk-profile, debug, SRC, -D DEBUG_MODE -fanalyzer -g3, debug))

all: $(NAME_release)

clean:
	@ $(RM) $(OBJ)
	@ $(LOG_TIME) "$(C_YELLOW) RM $(C_PURPLE) $(OBJ) $(C_RESET)"

fclean:
	@ $(RM) -r $(NAME_release) $(NAME_debug) $(BUILD_DIR)
	@ $(LOG_TIME) "$(C_YELLOW) RM $(C_PURPLE) $(NAME_release) $(NAME_debug) $(BUILD_DIR) \
		$(C_RESET)"

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
