CC = g++
CFLAGS = -std=c++11 -pthread

all: servidor cliente

servidor: servidor.cpp
	$(CC) $(CFLAGS) -o servidor servidor.cpp

cliente: cliente.cpp
	$(CC) $(CFLAGS) -o cliente cliente.cpp

run_servidor: servidor
	./servidor

run_cliente: cliente
	./cliente

clean:
	rm -f servidor cliente
