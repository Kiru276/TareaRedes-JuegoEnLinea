## Tarea de Redes - Juego en Línea con sockets TCP/IP

Este es un juego en línea para dos jugadores que se ejecuta a través de la red utilizando sockets TCP/IP. El juego implementa una versión del popular juego Conecta en Línea, en el que los jugadores intentan conectar cuatro fichas del mismo color en una fila, ya sea horizontal, vertical o diagonalmente.

`servidor.cpp`: Código fuente del servidor que maneja la lógica del juego y la comunicación con los clientes.

`cliente.cpp`: Código fuente del cliente que permite a los usuarios conectarse al servidor y jugar el juego.

`Makefile`: Archivo de make para compilar y ejecutar fácilmente el servidor y el cliente.

`README.md`: Este archivo, que proporciona información sobre el proyecto y cómo utilizarlo.

`Otros archivos`: Archivos necesarios para la compilación y ejecución del proyecto.

### Requisitos del Sistema

* Compilador de C++ compatible con C++11 (por ejemplo, g++)
* Conexión a internet (para jugar en línea)
* Opcional: Git (para clonar el repositorio)

### Instrucciones de Uso

1. Clonar el Repositorio: Si aún no tienes el repositorio, puedes clonarlo usando Git:
```
git clone https://github.com/tu-usuario/TareaRedes-JuegoEnLinea.git
```

2. Compilar el Proyecto: Utiliza el Makefile proporcionado para compilar el servidor y el cliente:

```
make
```
3. Ejecutar el Servidor: Inicia el servidor en una terminal e ingresa el puerto deseado cuando se te solicite:

```
make run_servidor
```
4. Ejecutar el Cliente: Inicia el cliente en otra terminal e ingresa el puerto del servidor al que deseas conectarte:

```
make run_cliente
```

5. Jugar al Juego: Sigue las instrucciones en pantalla para jugar al juego. Los jugadores pueden realizar movimientos alternados hasta que uno de ellos gane o haya un empate.

6. Limpiar: Cuando hayas terminado de jugar, puedes limpiar los archivos generados por la compilación con:

```
make clean
```

### Notas

* Asegúrate de que el puerto especificado en el servidor esté disponible y no esté bloqueado por un firewall.
* Si tienes problemas con la conexión, verifica tu configuración de red y asegúrate de que el servidor esté en ejecución antes de iniciar el cliente.

### ¡Diviértete jugando!
