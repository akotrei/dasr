# you need to install unity before testing: https://www.throwtheswitch.org/unity

ifeq ($(origin CC), default)
  CC = gcc
endif

CFLAGS ?= -Wall -O2
override CFLAGS += -I../include
BUILD ?= ../build

TSRC = test_array.c test_list.c test_set.c test_tree.c
TOBJ = $(TSRC:.c=.o)
TEXE = $(TOBJ:.o=.x)

$(info $(TEXE))

.PHONY: runtest
runtest: $(TEXE)
	$(filter-out &&END, $(foreach f, $^, ./$(f) &&)END)

.PHONY: clean
clean:
	rm -rf *.o *.x

$(TOBJ): %.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(TEXE): %.x: %.o
	$(CC) $< -o $@ -L$(BUILD) -ldast -lunity