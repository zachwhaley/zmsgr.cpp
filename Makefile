CXXFLAGS = -g -Wall -Werror -std=c++14
LDLIBS = -lzmq -lpthread

PRGM  = libzmsgr.so
SRCS := $(wildcard *.cpp)
OBJS := $(SRCS:.cpp=.o)
DEPS := $(OBJS:.o=.d)

.PHONY: all clean samples

all: $(PRGM)

$(PRGM): $(OBJS)
	$(CXX) $(OBJS) $(LDLIBS) -shared -o $@

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -MMD -MP -fPIC -c $< -o $@

clean:
	rm -rf $(OBJS) $(DEPS) $(PRGM) reqrep mtserver

samples: $(PRGM)
	$(CXX) $(CXXFLAGS) -I. -L. $(LDLIBS) -lzmsgr samples/reqrep.cpp -o reqrep
	$(CXX) $(CXXFLAGS) -I. -L. $(LDLIBS) -lzmsgr samples/mtserver.cpp -o mtserver

-include $(DEPS)
