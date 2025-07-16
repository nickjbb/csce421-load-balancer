CC = g++
CFLAGS = -Wall -std=c++17 -g

HEADERS = load_balancer.h webserver.h requests.h

all: loadbalancer
loadbalancer: load_balancer.cpp $(HEADERS)
	$(CC) $(CFLAGS) -o $@ load_balancer.cpp

clean:
	rm -f loadbalancer *.o
