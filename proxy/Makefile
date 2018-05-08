CXX = g++
CPPINCLUDE = -I../um/interface
CPPFLAGS = -std=c++1z
LDFLAGS = -ldl

SRCS = src/main.cpp
OBJS = bin/main.o

all: bin/proxy

bin/proxy: $(OBJS)
	$(CXX) -o bin/proxy $(OBJS) $(LDFLAGS)

$(OBJS): $(SRCS)
	mkdir -p bin
	$(CXX) $(CPPFLAGS) $(CPPINCLUDE) -c $(SRCS) -o $(OBJS)

clean:
	rm -f $(OBJS) bin/proxy
	rm -d bin
