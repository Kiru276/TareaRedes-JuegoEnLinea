#include <iostream>
#include <vector>

using namespace std;

const int FILAS = 6;
const int COLUMNAS = 7;
const char VACIO = '.';
const char JUGADOR1 = 'X';
const char JUGADOR2 = 'O';
const int MAX_MOVIMIENTOS = 21;

void inicializarTablero(vector<vector<char>>& tablero) {
    for (int i = 0; i < FILAS; ++i) {
        for (int j = 0; j < COLUMNAS; ++j) {
            tablero[i][j] = VACIO;
        }
    }
}

void imprimirTablero(const vector<vector<char>>& tablero) {
    // Imprimir números de columna
    cout << "  ";
    for (int j = 0; j < COLUMNAS; ++j) {
        cout << j + 1 << " ";
    }
    cout << endl;

    // Imprimir filas con números de fila
    for (int i = 0; i < FILAS; ++i) {
        cout << i + 1 << " "; // Imprimir número de fila
        for (int j = 0; j < COLUMNAS; ++j) {
            cout << tablero[i][j] << " ";
        }
        cout << endl;
    }
}

bool soltarFicha(vector<vector<char>>& tablero, int columna, char jugador) {
    columna--; // Ajustar la columna para que coincida con el índice de la matriz
    if (columna < 0 || columna >= COLUMNAS || tablero[0][columna] != VACIO) {
        return false; // Jugada inválida
    }

    for (int i = FILAS - 1; i >= 0; --i) {
        if (tablero[i][columna] == VACIO) {
            tablero[i][columna] = jugador;
            return true;
        }
    }

    return false; // Esto no debería ocurrir
}

bool verificarVictoria(const vector<vector<char>>& tablero, char jugador) {
    // Verificar horizontal
    for (int i = 0; i < FILAS; ++i) {
        for (int j = 0; j < COLUMNAS - 3; ++j) {
            if (tablero[i][j] == jugador && tablero[i][j + 1] == jugador && tablero[i][j + 2] == jugador && tablero[i][j + 3] == jugador) {
                return true;
            }
        }
    }

    // Verificar vertical
    for (int i = 0; i < FILAS - 3; ++i) {
        for (int j = 0; j < COLUMNAS; ++j) {
            if (tablero[i][j] == jugador && tablero[i + 1][j] == jugador && tablero[i + 2][j] == jugador && tablero[i + 3][j] == jugador) {
                return true;
            }
        }
    }

    // Verificar diagonal (arriba-izquierda a abajo-derecha)
    for (int i = 0; i < FILAS - 3; ++i) {
        for (int j = 0; j < COLUMNAS - 3; ++j) {
            if (tablero[i][j] == jugador && tablero[i + 1][j + 1] == jugador && tablero[i + 2][j + 2] == jugador && tablero[i + 3][j + 3] == jugador) {
                return true;
            }
        }
    }

    // Verificar diagonal (abajo-izquierda a arriba-derecha)
    for (int i = 3; i < FILAS; ++i) {
        for (int j = 0; j < COLUMNAS - 3; ++j) {
            if (tablero[i][j] == jugador && tablero[i - 1][j + 1] == jugador && tablero[i - 2][j + 2] == jugador && tablero[i - 3][j + 3] == jugador) {
                return true;
            }
        }
    }

    return false;
}

int main() {
    vector<vector<char>> tablero(FILAS, vector<char>(COLUMNAS));
    inicializarTablero(tablero);

    char jugadorActual = JUGADOR1;
    int movimientos = 0;
    while (movimientos < MAX_MOVIMIENTOS) {
        imprimirTablero(tablero);
        int columna;
        cout << "Jugador " << jugadorActual << ", ingrese la columna (1-7): ";
        cin >> columna;

        if (soltarFicha(tablero, columna, jugadorActual)) {
            movimientos++;
            if (verificarVictoria(tablero, jugadorActual)) {
                imprimirTablero(tablero);
                cout << "¡Jugador " << jugadorActual << " gana!" << endl;
                return 0;
            }

            // Cambiar de jugador
            jugadorActual = (jugadorActual == JUGADOR1) ? JUGADOR2 : JUGADOR1;
        } else {
            cout << "Jugada inválida. Inténtelo de nuevo." << endl;
        }
    }

    imprimirTablero(tablero);
    cout << "¡Es un empate! Se han alcanzado los " << MAX_MOVIMIENTOS << " movimientos." << endl;
    return 0;
}
