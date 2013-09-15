.PHONY : all clean

CFLAGS := -Wall
CXXFLAGS := -Wall -std=c++11
DIRS := $(shell find . -type d)
FILES := $(foreach dir, $(DIRS), $(wildcard $(dir)/*.c) \
		                         $(wildcard $(dir)/*.cc))
OBJS := $(addsuffix .o, $(basename $(FILES)))
APP := flowhack

all: $(OBJS)
	$(CXX) $(CFLAGS) $(CPPFLAGS) $(LDFLAGS) $(OBJS) -o $(APP)

clean:
	rm -f $(OBJS) $(APP) $(OBJS:.o=.d)

%.d: %.cpp
	$(CXX) $(CPPFLAGS) -MM -MF $@ $<

%.d: %.c
	$(CC) $(CFLAGS) -MM -MF $@ $<

-include $(OBJS:.o=.d)
