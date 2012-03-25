.PHONY : all clean

CFLAGS := -Wall -I.
CXXFLAGS := -Wall -I.
CC := g++
APPS := esctest

all: $(APPS)

esctest : esctest.o esc.o

clean:
	@rm -vf *o $(APPS)

%.d: %.cc
	$(CXX) $(CXXFLAGS) -MM -MF $@ $<

%.d: %.c
	$(CC) $(CFLAGS) -MM -MF $@ $<

-include $(OBJS:.o=.d)
