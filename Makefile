# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
#     Compilation Options
# Debug mode [yes/no] (allowing to debug the library via gdb):
DEBUG ?= no
# Specify your favourite C compiler here:
COMPILER ?= cc
# Specify build type [shared/static] (dynamic or static libraries)
BUILD_TYPE ?= static
# Specify your include directory (headers location):
INCDIR ?= /usr/loca/include
# Specify your libraries directory:
LIBDIR ?= /usr/loca/lib
# Specify lib name:
LIBNAME ?= dast
# Specify path to this file
CURRENT_DIR = $(shell pwd)


# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
#     Preparations
# Compile as ANSI C code:
CFLAGS ?= -Wall


# Debug and optimisation (as well as -static for valgrind) are not compatible:
ifeq '$(DEBUG)' 'yes'
	CFLAGS  += -g -O0 -D DEBUG
else ifeq '$(DEBUG)' 'no'
	CFLAGS  += -O3
else
$(error you must use 'yes' or 'no' for DEBUG flag)
endif


# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
# Set type of building options for gcc
COBJFLAGS = -xc -ansi
ifeq '$(BUILD_TYPE)' 'shared'
	COBJFLAGS	+= -fPIC
	CLIBFLAGS	= -shared
else ifeq '$(BUILD_TYPE)' 'static'
	CLIBFLAGS	= -static
else
$(error you must use 'shared' or 'static' for BUILD_TYPE flag)
endif


# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
# Specify linker to use the library:
LFLAGS  = -L$(CURRENT_DIR)/$(BUILD_DIR)
IFLAGS	= -I$(CURRENT_DIR)/$(INCLUDE_DIR)


# Directories definitions:
INCLUDE_DIR  	= include
BUILD_DIR 		= build
SRC_DIR		 	= src
DEV_DIR			= dev
$(shell mkdir -p $(BUILD_DIR))


# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
#     targets
.PHONY: clean
.PHONY: all

all: LIB_BUILD;
clean:
	rm -f $(BUILD_DIR)/*


# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
#		lib
OBJ_ALL = $(ALLOCATOR_OBJ) $(TREE_OBJ)
LIB_BUILD: $(OBJ_ALL)
ifeq '$(BUILD_TYPE)' 'shared'
	$(COMPILER) $(CFLAGS) $(CLIBFLAGS) -o $(BUILD_DIR)/lib$(LIBNAME).so $(OBJ_ALL)
else ifeq '$(BUILD_TYPE)' 'static'
	ar rc $(BUILD_DIR)/lib$(LIBNAME).a $(OBJ_ALL)
	ranlib $(BUILD_DIR)/lib$(LIBNAME).a
else
$(error you must use 'shared' or 'static' for BUILD_TYPE flag)
endif


# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
#		allocator
ALLOCATOR_H = $(INCLUDE_DIR)/utils/allocator.h $(INCLUDE_DIR)/interface/iallocator.h
ALLOCATOR_C = $(SRC_DIR)/utils/allocator.c
ALLOCATOR_OBJ = $(BUILD_DIR)/allocator.o
ALLOCATOR_OBJ_BUILD: $(ALLOCATOR_OBJ)
$(ALLOCATOR_OBJ): $(BUILD_DIR)/%.o: $(SRC_DIR)/utils/%.c $(ALLOCATOR_H)
	$(COMPILER) $(CFLAGS) $(COBJFLAGS) $(IFLAGS) -c $< -o $@


# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
#		tree
TREE_H = $(wildcard $(INCLUDE_DIR)/tree/*.h) $(INCLUDE_DIR)/interface/iallocator.h\
$(INCLUDE_DIR)/interface/iiterator.h
TREE_C = $(wildcard $(SRC_DIR)/tree/*.c)
TREE_OBJ = $(subst $(SRC_DIR)/tree/,$(BUILD_DIR)/,$(subst .c,.o,$(TREE_C)))
TREE_OBJ_BUILD: $(TREE_OBJ)
$(TREE_OBJ): $(BUILD_DIR)/%.o: $(SRC_DIR)/tree/%.c $(TREE_H)
	$(COMPILER) $(CFLAGS) $(COBJFLAGS) $(IFLAGS) -c $< -o $@


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
#		dev/test_allocator.c
DEV_ALLOCATOR_C = $(DEV_DIR)/test_allocator.c
DEV_ALLOCATOR_OBJ = $(BUILD_DIR)/test_allocator.o
DEV_ALLOCATOR_EXE = $(BUILD_DIR)/test_allocator
$(DEV_ALLOCATOR_OBJ): $(DEV_ALLOCATOR_C)
	$(COMPILER) $(CFLAGS) $(IFLAGS) -c $(DEV_ALLOCATOR_C) -o $(DEV_ALLOCATOR_OBJ)
$(DEV_ALLOCATOR_EXE): $(DEV_ALLOCATOR_OBJ)
	$(COMPILER) $(CFLAGS) -o $(DEV_ALLOCATOR_EXE) $(DEV_ALLOCATOR_OBJ) \
	$(LFLAGS) -l$(LIBNAME)

# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
#		dev/test_tree.c
DEV_TREE_C = $(DEV_DIR)/test_tree.c
DEV_TREE_OBJ = $(BUILD_DIR)/test_tree.o
DEV_TREE_EXE = $(BUILD_DIR)/test_tree
$(DEV_TREE_OBJ): $(DEV_TREE_C)
	$(COMPILER) $(CFLAGS) $(IFLAGS) -c $(DEV_TREE_C) -o $(DEV_TREE_OBJ)
$(DEV_TREE_EXE): $(DEV_TREE_OBJ)
	$(COMPILER) $(CFLAGS) -o $(DEV_TREE_EXE) $(DEV_TREE_OBJ) \
	$(LFLAGS) -l$(LIBNAME)