
CPPFLAGS = -Wall -Werror -O2
# enable this for debugging
#CPPFLAGS = -Wall -g
CXX = g++ -std=c++11
# enable this on Mac OS X
#CXX = g++-4.2

LDFLAGS =
READTEST_SRCS = readrecord.cpp fraction.cpp
DB_SRCS = fraction.cpp interactive.cpp
READTEST_OBJS = $(READTEST_SRCS:.cpp=.o)
DB_OBJS = $(DB_SRCS:.cpp=.o)
PROGS = db 

default : db

db : $(DB_OBJS)
	$(CXX) -o $@ $(DB_OBJS) $(LDFLAGS)


# The dependencies below make use of make's default rules,
# under which a .o automatically depends on its .c and
# the action taken uses the $(CXX) and $(CFLAGS) variables.
# These lines describe a few extra dependencies involved.

depend:: Makefile.dependencies $(DB_SRCS) $(HDRS)

Makefile.dependencies:: $(DB_SRCS) $(READTEST_SRCS) $(HDRS)
	$(CXX) $(CPPFLAGS) -MM $(DB_SRCS) $(READTEST_SRCS) > Makefile.dependencies

-include Makefile.dependencies

clean : 
	rm -f *.o a.out core $(PROGS) Makefile.dependencies
