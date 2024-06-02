#include <iostream>
#include <arpa/inet.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>


const int BUFFER_SIZE = 1024;  // Definir tamaño del buffer como una constante

// Crea socket del cliente
int crearSocket() {
    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket < 0) {
        std::cerr << "Error: No hay conexión de la red" << std::endl;
        exit(-1);
    }
    std::cout << "Creación exitosa de Socket" << std::endl;
    return clientSocket;
}

// Configura dirección servidor
void configurarDireccionServidor(struct sockaddr_in &serverAddr, int puerto) {
    memset(&serverAddr, '\0', sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(puerto);  // Puerto dinámico
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
}

// Conecta el servidor
void conectarServidor(int clientSocket, struct sockaddr_in &serverAddr) {
    int ret = connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
    if (ret < 0) {
        std::cerr << "Error: No se estableció enlace" << std::endl;
        exit(-1);
    }
    std::cout << "Cliente se ha conectado al servidor" << std::endl;
}

// Recibe y muestra el estado del tablero
bool recibirEstadoTablero(int clientSocket, char buffer[], int bufferSize) {
    memset(buffer, 0, bufferSize);
    int bytesReceived = recv(clientSocket, buffer, bufferSize, 0);
    if (bytesReceived > 0) {
        std::cout << buffer << std::endl;
        // Verificar si el juego ha terminado
        if (strstr(buffer, "Ganaste!!") != nullptr || strstr(buffer, "El servidor ganó :(") != nullptr || strstr(buffer, "Es un empate :P! ") != nullptr) {
            return true;  // Si es, el juego ha terminado
        }
    } else {
        std::cerr << "Error: Conexión perdida" << std::endl;
        exit(-1);
    }
    return false;  // Sino, el juego continúa
}

// Envía la jugada del cliente
void enviarJugada(int clientSocket) {
    std::cout << "Ingresa la columna (1-7): ";
    int col;
    std::cin >> col;
    col -= 1; // Convierte de 1-7 a 0-6
    while (col < 0 || col > 6) {
        std::cout << "Columna no válida. Ingresa un número entre 1-7: ";
        std::cin >> col;
        col -= 1; // Convierte de 1-7 a 0-6
    }
    std::string message = std::to_string(col);
    send(clientSocket, message.c_str(), message.size(), 0);
}

int main() {
    int puerto;
    std::cout << "Ingrese el puerto del servidor: ";
    std::cin >> puerto;

    int clientSocket = crearSocket();

    struct sockaddr_in serverAddr;
    configurarDireccionServidor(serverAddr, puerto);

    conectarServidor(clientSocket, serverAddr);

    char buffer[BUFFER_SIZE];
    while (true) {
        if (recibirEstadoTablero(clientSocket, buffer, BUFFER_SIZE)) {
            break;  // Salir del bucle si el juego ha terminado
        }
        enviarJugada(clientSocket);
    }

    close(clientSocket);
    return 0;
}
