ifeq ($(origin CC), default)
  CC = gcc
endif

CFLAGS ?= -Wall -O2 -fPIC
override CFLAGS += -I./include
BUILD ?= build

CSRC = src/array.c src/list.c src/map.c src/utils/mem.c src/utils/allocator_std.c
COBJ = $(addprefix $(BUILD)/, $(CSRC:.c=.o))
DEPS = $(COBJ:.o=.d)

.PHONY: all
all: $(BUILD)/libdast.so

.PHONY: clean
clean:
	rm -rf $(COBJ) $(DEPS) $(BUILD)/*.so

$(BUILD)/libdast.so: $(COBJ)
	$(CC) -shared $^ -o $@ $(LDFLAGS)

$(COBJ) : $(BUILD)/%.o : %.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

$(DEPS) : $(BUILD)/%.d : %.c
	@mkdir -p $(@D)
	$(CC) -E $(CFLAGS) $< -MM -MT $(@:.d=.o) > $@

# include dependencies only if we do not use "make clean"
ifeq (0, $(words $(findstring $(MAKECMDGOALS), clean)))
include $(DEPS)
endif