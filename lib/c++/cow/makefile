.PHONY : all clean

CFLAGS := -O0 -Wall -I.
CXXFLAGS := -O0 -Wall -I. -g
DIRS := $(shell find . -type d)
FILES := $(foreach dir, $(DIRS), $(wildcard $(dir)/*.c) \
		                         $(wildcard $(dir)/*.cc))
OBJS := $(addsuffix .o, $(basename $(FILES)))
APP := cow

all: $(OBJS)
	$(CXX) $(CFLAGS) $(CPPFLAGS) $(LDFLAGS) $(OBJS) -o $(APP)

clean:
	rm -f $(OBJS) $(APP) $(OBJS:.o=.d)

%.d: %.cc
	$(CXX) $(CXXFLAGS) -MM -MF $@ $<

%.d: %.c
	$(CC) $(CFLAGS) -MM -MF $@ $<

-include $(OBJS:.o=.d)
