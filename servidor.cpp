#include <iostream>
#include <cstdlib>
#include <thread>
#include <algorithm>
#include <ctime>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <vector>
#include <netinet/in.h>
#include <arpa/inet.h>

// Tablero
class Tablero {
public:
    std::vector<std::vector<int>> cuadricula;
    int filas;
    int columnas;

    bool verificarVictoria(int jugador) const {
        
        // Comprobacion victoria vertical
        for (int columna = 0; columna < columnas; ++columna) {
            for (int fila = 0; fila < filas - 3; ++fila) {
                if (cuadricula[fila][columna] == jugador && cuadricula[fila + 1][columna] == jugador &&
                    cuadricula[fila + 2][columna] == jugador && cuadricula[fila + 3][columna] == jugador) {
                    return true;
                }
            }
        }

        // Comprobacion victoria horizontal
        for (int fila = 0; fila < filas; ++fila) {
            for (int columna = 0; columna < columnas - 3; ++columna) {
                if (cuadricula[fila][columna] == jugador && cuadricula[fila][columna + 1] == jugador &&
                    cuadricula[fila][columna + 2] == jugador && cuadricula[fila][columna + 3] == jugador) {
                    return true;
                }
            }
        }

        // Comprobacion victoria diagonal inversa
        for (int fila = 3; fila < filas; ++fila) {
            for (int columna = 0; columna < columnas - 3; ++columna) {
                if (cuadricula[fila][columna] == jugador && cuadricula[fila - 1][columna + 1] == jugador &&
                    cuadricula[fila - 2][columna + 2] == jugador && cuadricula[fila - 3][columna + 3] == jugador) {
                    return true;
                }
            }
        }

        // Comprobacion victoria diagonal 
        for (int fila = 0; fila < filas - 3; ++fila) {
            for (int columna = 0; columna < columnas - 3; ++columna) {
                if (cuadricula[fila][columna] == jugador && cuadricula[fila + 1][columna + 1] == jugador &&
                    cuadricula[fila + 2][columna + 2] == jugador && cuadricula[fila + 3][columna + 3] == jugador) {
                    return true;
                }
            }
        }
        
        return false;
    }

    Tablero() : filas(6), columnas(7) {
        cuadricula = std::vector<std::vector<int>>(filas, std::vector<int>(columnas, 0));
    }

    void realizarMovimiento(int columna, int jugador) {
        for (int fila = filas - 1; fila >= 0; --fila) {
            if (cuadricula[fila][columna] == 0) {
                cuadricula[fila][columna] = jugador;
                break;
            }
        }
    }

    bool esMovimientoValido(int columna) const {
        return columna >= 0 && columna < columnas && cuadricula[0][columna] == 0;
    }
};

// Servidor
class Servidor {
    // Atributos
private:
    std::vector<int> socketsClientes;  
    int descriptorSocket;  
    struct sockaddr_in direccionServidor;  

public:
    // Manejar la comunicación con un cliente
    void manejarCliente(int socketCliente, Tablero tablero) {
        char buffer[1024];
        int contadorMovimientos = 0;
        const int maxMovimientos = 21;  // Máximo de movimientos permitidos
        bool turnoJugador = std::rand() % 2 == 0;  // Turno inicial aleatorio

        while (true) {
            memset(buffer, 0, sizeof(buffer));
            if (turnoJugador) {
                enviarEstadoTablero(socketCliente, tablero); // Envia estado actual del tablero
                int bytesRecibidos = recv(socketCliente, buffer, sizeof(buffer), 0); // Recibir mensaje
                if (bytesRecibidos <= 0) {
                    std::cerr << "Se perdió conexión" << std::endl; // Mensaje de error, conexión perdida
                    close(socketCliente);
                    socketsClientes.erase(std::remove(socketsClientes.begin(), socketsClientes.end(), socketCliente), socketsClientes.end()); // Eliminar cliente
                    break;
                }

                int columna = std::atoi(buffer);
                std::cout << "Cliente se movió en: " << columna + 1 << std::endl;
                if (tablero.esMovimientoValido(columna)) {
                    tablero.realizarMovimiento(columna, 1);
                    contadorMovimientos++;
                    turnoJugador = false;  // Cambio turno a servidor

                    // Si el cliente gano
                    if (tablero.verificarVictoria(1)) {
                        std::string mensajeVictoria = "Ganaste!! (cliente ganó)";
                        send(socketCliente, mensajeVictoria.c_str(), mensajeVictoria.size(), 0);
                        enviarEstadoTablero(socketCliente, tablero);  // Envio tablero final
                        std::cout << "Ganaste!! (cliente ganó)" << std::endl;
                        close(socketCliente);  // Cerrar la conexión
                        socketsClientes.erase(std::remove(socketsClientes.begin(), socketsClientes.end(), socketCliente), socketsClientes.end()); // Eliminar cliente
                        break;
                    }
                } else {
                    std::cerr << "Movimiento inválido del cliente" << std::endl;
                }
            } else {
                int columna = generarMovimientoAleatorio(tablero);
                if (columna != -1) {
                    std::cout << "Servidor se movió en: " << columna + 1 << std::endl;
                    tablero.realizarMovimiento(columna, 2);  // Jugada del servidor
                    contadorMovimientos++;
                    turnoJugador = true;  // Cambiar turno al cliente

                    // Si el servidor gano
                    if (tablero.verificarVictoria(2)) {
                        std::string mensajeDerrota = "El servidor ganó :("; // Mensaje de derrota
                        send(socketCliente, mensajeDerrota.c_str(), mensajeDerrota.size(), 0); // Enviar mensaje
                        enviarEstadoTablero(socketCliente, tablero);  // Enviar tablero final
                        std::cout << "El servidor ganó :(" << std::endl;
                        close(socketCliente);  // Cierra conexión
                        socketsClientes.erase(std::remove(socketsClientes.begin(), socketsClientes.end(), socketCliente), socketsClientes.end()); // Eliminar cliente
                        break;
                    }
                } else {
                    std::cerr << "No hay movimientos válidos para el servidor" << std::endl; // Mensaje de error, movimiento invalido
                }
            }

            // Si se ha alcanzado el número máximo de movimientos
            if (contadorMovimientos >= maxMovimientos) {
                std::string mensajeEmpate = "Es un empate :P!"; // Mensaje de empate
                send(socketCliente, mensajeEmpate.c_str(), mensajeEmpate.size(), 0); // Enviar mensaje
                enviarEstadoTablero(socketCliente, tablero);  // Enviar tablero final
                std::cout << "Es un empate :P!" << std::endl; 
                close(socketCliente);  // Cierra conexión
                socketsClientes.erase(std::remove(socketsClientes.begin(), socketsClientes.end(), socketCliente), socketsClientes.end()); // Eliminar cliente
                break;
            }
        }
    }
    
    // Constructor del servidor
    Servidor(int puerto) {
        // Generador de números randoms
        std::srand(std::time(nullptr));

        // Crea el socket del servidor
        descriptorSocket = socket(AF_INET, SOCK_STREAM, 0);
        if (descriptorSocket < 0) {
            std::cerr << "Error: No hay conexión de la red" << std::endl;
            exit(1);
        }
        std::cout << "Creación exitosa de Socket" << std::endl;

        // Maneja la dirección del servidor
        memset(&direccionServidor, '\0', sizeof(direccionServidor)); // Limpia la estructura
        direccionServidor.sin_family = AF_INET; 
        direccionServidor.sin_port = htons(puerto); // Puerto dinámico
        direccionServidor.sin_addr.s_addr = inet_addr("127.0.0.1"); 

        // Ancla socket del servidor a la dirección
        int ret = bind(descriptorSocket, (struct sockaddr*)&direccionServidor, sizeof(direccionServidor));
        if (ret < 0) {
            std::cerr << "Error: No se estableció enlace" << std::endl;
            exit(1);
        }
        std::cout << "Se ha enlazado en el puerto " << puerto << std::endl;

        // Poner el servidor en modo de escucha
        if (listen(descriptorSocket, 5) == 0) { // Escucha hasta 5 conexiones
            std::cout << "Conectado..." << std::endl; 
        } else {
            std::cerr << "Error: Problema en la conexión" << std::endl;
        }
    }

    // Controla conexiones de del juego y cliente
    void manejarConexiones() {
        while (true) {
            int socketCliente = accept(descriptorSocket, nullptr, nullptr);
            if (socketCliente < 0) { // Si no se puede conectar
                std::cerr << "Error de conexión" << std::endl;
                continue;
            }
            socketsClientes.push_back(socketCliente); // Agregar cliente a la lista
            std::cout << "Cliente se ha conectado" << std::endl; // Mensaje de conexión

            // Nuevo tablero para cliente
            Tablero tablero;

            // Hilo para manejo del cliente
            std::thread(&Servidor::manejarCliente, this, socketCliente, tablero).detach();
        }
    }

    // Realiza jugada aleatoria para servidor
    int generarMovimientoAleatorio(const Tablero& tablero) { // Movimiento aleatorio
        std::vector<int> movimientosValidos; // Movimientos validos
        for (int columna = 0; columna < tablero.columnas; ++columna) {
            if (tablero.esMovimientoValido(columna)) {
                movimientosValidos.push_back(columna);
            }
        }
        if (!movimientosValidos.empty()) { // Si hay movimientos validos
            int indiceAleatorio = std::rand() % movimientosValidos.size();
            return movimientosValidos[indiceAleatorio];
        }
        return -1;  // Sin movimientos válidos
    }

    // Estado del tablero a los consola cliente
    void enviarEstadoTablero(int socketCliente, const Tablero& tablero) {
        std::string estadoTablero = "\n\n---------Juego: Intenta conectar 4-----------\n"; // Mensaje de juego
        for (int fila = 0; fila < tablero.filas; ++fila) {
            estadoTablero += std::to_string(fila + 1) + " "; // Numeros de fila
            for (int columna = 0; columna < tablero.columnas; ++columna) {
                int celda = tablero.cuadricula[fila][columna]; // Celdas del tablero
                estadoTablero += (celda == 0 ? "." : (celda == 1 ? "C" : "S")); // Celdas vacias, cliente o servidor
                estadoTablero += " ";
            }
            estadoTablero += "\n";
        }
        estadoTablero += "  1 2 3 4 5 6 7\n";
        send(socketCliente, estadoTablero.c_str(), estadoTablero.size(), 0); // Enviar mensaje
        std::cout << estadoTablero;
    }
};

int main() {
    int puerto;
    std::cout << "Ingrese el puerto para el servidor: ";
    std::cin >> puerto;

    Servidor servidor(puerto);  // Crear el servidor con el puerto ingresado
    servidor.manejarConexiones();  // Manejar las conexiones de los clientes

    return 0;
}
