UNAME := $(shell uname)

ifeq ($(UNAME), Linux)
CC = gcc -g
endif
ifeq ($(UNAME), Darwin)
CC = clang -g
endif
ifeq ($(UNAME), FreeBSD)
CC = gcc -g
endif

ifneq ($(shell pkg-config --modversion --silence-errors papi),)
PAPI_CFLAGS := $(shell pkg-config --cflags papi) -DPAPI=1
PAPI_LDLIBS := $(shell pkg-config --libs papi)
else
PAPI_CFLAGS := $(shell pkg-config --cflags papi) -DPAPI=0
$(info Please install libpapi-dev package to enable PMU counters!)
endif

OFLAGS ?= -O2
WFLAGS ?= -Wall -Wextra -Wno-unused -Wunused-result
CFLAGS = -std=gnu11 -march=native $(OFLAGS) $(WFLAGS) $(PAPI_CFLAGS)
LDLIBS = $(PAPI_LDLIBS)

all: $(PROG) # raport.html

$(PROG): $(PROG).o main.o common.o
$(PROG).o: $(PROG).c common.h
main.o: main.c common.h $(PROG).h
common.o: common.c common.h

# requires "markdown" and "gnuplot" packages to be installed
raport.html: raport.md figure.png

%.html: %.md
	markdown $< > $@

%.png: %.gp %.dat
	gnuplot $< > $@

%.eps: %.gp %.dat
	gnuplot $< > $@

clean:
	@rm -vf $(PROG) *.o *.html *.png *~

# vim: ts=8 sw=8
