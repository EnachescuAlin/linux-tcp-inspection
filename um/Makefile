CXX = g++
CPPINCLUDE = -I./include -I./interface
CPPFLAGS = -std=c++1z -fPIC
LDFLAGS = -shared -rdynamic

OBJS = bin/main.o bin/TcpInspection.o bin/Proxy.o bin/Proxies.o

all: dir bin/TcpInspection.so

dir:
	mkdir -p bin

bin/TcpInspection.so: $(OBJS)
	$(CXX) $(LDFLAGS) -o bin/TcpInspection.so $(OBJS) -lpthread

bin/main.o: src/main.cpp
	$(CXX) $(CPPFLAGS) $(CPPINCLUDE) -c $< -o $@

bin/TcpInspection.o: src/TcpInspection.cpp
	$(CXX) $(CPPFLAGS) $(CPPINCLUDE) -c $< -o $@

bin/Proxy.o: src/Proxy.cpp
	$(CXX) $(CPPFLAGS) $(CPPINCLUDE) -c $< -o $@

bin/Proxies.o: src/Proxies.cpp
	$(CXX) $(CPPFLAGS) $(CPPINCLUDE) -c $< -o $@

clean:
	rm -f $(OBJS) bin/TcpInspection.so
	rm -d bin
