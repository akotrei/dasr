# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
#     Compilation Options
# Debug mode [yes/no] (allowing to debug the library via gdb):
DEBUG   ?= no
# Specify your favourite C compiler here:
COMPILER ?= cc
# Specify build type [shared/static] (dynamic or static libraries)
BUILD_TYPE ?= static
# Specify your include directory (headers location):
INCDIR  ?= /usr/loca/include
# Specify your libraries directory:
LIBDIR  ?= /usr/loca/lib

# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
#     Preparations
# Compile as ANSI C code:
CFLAGS		= -Wall

# Debug and optimisation (as well as -static for valgrind) are not compatible:
ifeq '$(DEBUG)' 'yes'
	CFLAGS  += -g -O0 -D DEBUG
else ifeq '$(DEBUG)' 'no'
	CFLAGS  += -O3
else
$(error you must use 'yes' or 'no' for DEBUG flag)
endif

# Set type of building options for gcc
COBJFLAGS = -xc -ansi
ifeq '$(BUILD_TYPE)' 'shared'
	COBJFLAGS	+= -fPIC 
	CLIBFLAGS	= -shared
else ifeq '$(BUILD_TYPE)' 'static'
	CLIBFLAGS	= -static
else
$(error you must use 'shared' or 'static' for DEBUG flag)
endif

# Specify linker to use the library:
LFLAGS  = -L$(BUILD_DIR)
IFLAGS	= -I$(INCLUDE_DIR)

# Directories definitions:
INCLUDE_DIR  	= include
BUILD_DIR 		= build
SRC_DIR		 	= src
DEV_DIR			= dev
$(shell mkdir -p $(BUILD_DIR))

# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
#		dev/main.c
DEV_MAIN_C = $(DEV_DIR)/main.c
DEV_MAIN_OBJ = $(BUILD_DIR)/main.o
DEV_MAIN_EXE = $(BUILD_DIR)/main
$(DEV_MAIN_OBJ): $(DEV_MAIN_C)
	$(COMPILER) $(CFLAGS) $(IFLAGS) -c $(DEV_MAIN_C) -o $(DEV_MAIN_OBJ)
$(DEV_MAIN_EXE): $(DEV_MAIN_OBJ)
	$(COMPILER) $(CFLAGS) -o $(DEV_MAIN_EXE) $(DEV_MAIN_OBJ)

# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
#		allocator_std
ALLOCATOR_STD_H = $(INCLUDE_DIR)/allocator/allocator_std.h
ALLOCATOR_STD_C = $(SRC_DIR)/allocator/allocator_std.c
ALLOCATOR_STD_OBJ = $(BUILD_DIR)/allocator_std.o
ALLOCATOR_STD_SO = $(BUILD_DIR)/liballocator_std.so
ALLOCATOR_STD_A = $(BUILD_DIR)/liballocator_std.a
$(ALLOCATOR_STD_OBJ): $(ALLOCATOR_STD_C) $(ALLOCATOR_STD_H)
	$(COMPILER) $(CFLAGS) $(COBJFLAGS) $(IFLAGS) -c $(ALLOCATOR_STD_C) -o $(ALLOCATOR_STD_OBJ)
$(ALLOCATOR_STD_SO): $(ALLOCATOR_STD_OBJ)
ifeq '$(BUILD_TYPE)' 'shared'
	$(COMPILER) $(CFLAGS) $(CLIBFLAGS) -o $(ALLOCATOR_STD_SO) $(ALLOCATOR_STD_OBJ)
endif
$(ALLOCATOR_STD_A): $(ALLOCATOR_STD_OBJ)
ifeq '$(BUILD_TYPE)' 'static'
	ar rc $(ALLOCATOR_STD_A) $(ALLOCATOR_STD_OBJ)
	ranlib $(ALLOCATOR_STD_A)
endif

# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
#     Targets
.PHONY: all examples clean install uninstall

clean:
	rm -f $(BUILD_DIR)/*
