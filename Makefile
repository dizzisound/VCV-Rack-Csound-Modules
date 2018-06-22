SLUG = Csound
VERSION = 0.6.0

# If RACK_DIR is not defined when calling the Makefile, default to two levels above
RACK_DIR ?= ../..

# Default Csound paths
CSOUND_INCLUDE ?= /usr/local/include/csound
CSOUND_LIBRARY ?= /usr/local/lib/

# FLAGS will be passed to both the C and C++ compiler
FLAGS +=
CFLAGS +=
CXXFLAGS +=

# Platform detection
include $(RACK_DIR)/arch.mk

# Careful about linking to libraries, since you can't assume much about the user's environment and library search path.
# Static libraries are fine.
ifeq ($(ARCH), win)
	FLAGS += -DUSE_DOUBLE -I$(CSOUND_INCLUDE)
	CXXFLAGS += -I $(CSOUND_INCLUDE)
	LDFLAGS +=  -L"$(CSOUND_LIBRARY)" -lcsound64
else
	LDFLAGS += -L"$(CSOUND_LIBRARY)" -lcsound64
endif

# Add .cpp and .c files to the build
SOURCES = $(wildcard src/*.cpp)
DISTRIBUTABLES += $(wildcard LICENSE*) res  csd
	
# Must include the VCV plugin Makefile framework
include $(RACK_DIR)/plugin.mk
