.PHONY : all clean

CFLAGS := -Wall -I.
CXXFLAGS := -Wall -I.
APPS := test-timeout

all: $(APPS)

test-timeout : test-timeout.o timeout.o

clean:
	@rm -vf *o $(APPS)
