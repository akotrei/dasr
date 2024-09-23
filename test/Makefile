# you need to install unity before testing: https://www.throwtheswitch.org/unity

ifeq ($(origin CC), default)
  CC = gcc
endif

CFLAGS ?= -Wall -O2
override CFLAGS += -I../include
BUILD ?= ../build

TSRC = test_list.c test_array.c
TOBJ = $(TSRC:.c=.o)
TEXE = $(TOBJ:.o=.x)

$(info $(TEXE))

.PHONY: runtest
runtest: $(TEXE)
	$(foreach f, $^, ./$(f);)

.PHONY: clean
clean:
	rm -rf *.o *.x

$(TOBJ): %.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(TEXE): %.x: %.o
	$(CC) $< -o $@ -L$(BUILD) -ldast -lunity