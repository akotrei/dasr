# install unity before testing: https://www.throwtheswitch.org/unity

ifeq ($(origin CC), default)
  CC = gcc
endif

CFLAGS ?= -Wall -O2
override CFLAGS += -I../include
BUILD ?= ../build

TSRC = test_list.c
TOBJ = $(TSRC:.c=.o)
TEXE = $(TOBJ:.o=.x)

.PHONY: runtest
runtest: $(TEXE)
	./$<

.PHONY: clean
clean:
	rm -rf *.o *.x

$(TOBJ): %.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(TEXE): %.x: %.o
	$(CC) $< -o $@ -L$(BUILD) -ldast -lunity