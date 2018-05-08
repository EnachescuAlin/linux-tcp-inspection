CXX = g++
CPPINCLUDE = -I./include -I./interface
CPPFLAGS = -std=c++1z -fPIC
LDFLAGS = -shared

SRCS = src/main.cpp
OBJS = bin/main.o

all: bin/TcpInspection.so

bin/TcpInspection.so: $(OBJS)
	$(CXX) $(LDFLAGS) -o bin/TcpInspection.so $(OBJS)

$(OBJS): $(SRCS)
	mkdir -p bin
	$(CXX) $(CPPFLAGS) $(CPPINCLUDE) -c $(SRCS) -o $(OBJS)

clean:
	rm -f $(OBJS) bin/TcpInspection.so
	rm -d bin
