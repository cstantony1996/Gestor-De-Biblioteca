#include <iostream>
#include "biblioteca.h"  // Cabecera donde se declaran las funciones

using namespace std;

// Menú Usuario
void menuUsuario() {
    int opcion;
    bool salir = false;

    while (!salir) {
        cout << "\n--- Menú Usuario ---" << endl;
        cout << "1. Agregar libro" << endl;
        cout << "2. Listar libros" << endl;
        cout << "3. Prestar libro" << endl;
        cout << "4. Devolver libro" << endl;
        cout << "5. Salir" << endl;
        cout << "Seleccione una opción: ";
        cin >> opcion;

        switch (opcion) {
            case 1:
                agregarLibro();
                break;
            case 2:
                listarLibros();
                break;
            case 3:
                prestarLibro();
                break;
            case 4:
                devolverLibro();
                break;
            case 5:
                salir = true;
                break;
            default:
                cout << "Opción no válida, intente nuevamente." << endl;
                break;
        }
    }
}

// Menú Administrador
void menuAdministrador() {
    int opcion;
    bool salir = false;

    while (!salir) {
        cout << "\n--- Menú Administrador ---" << endl;
        cout << "1. Agregar libro" << endl;
        cout << "2. Listar libros" << endl;
        cout << "Seleccione una opción: ";
        cin >> opcion;

        switch (opcion) {
            case 1:
                agregarLibro();
                break;
            case 2:
                listarLibros();
                break;
            default:
                cout << "Opción no válida, intente nuevamente." << endl;
                break;
        }
    }
}

// Menú Principal
int main() {
    int opcion;
    bool salir = false;

    while (!salir) {
        cout << "\n--- Menú Principal ---" << endl;
        cout << "1. Usuario" << endl;
        cout << "2. Administrador" << endl;
        cout << "Seleccione una opción: ";
        cin >> opcion;

        switch (opcion) {
            case 1:
                menuUsuario();
                break;
            case 2:
                menuAdministrador();
                break;
            default:
                cout << "Opción no válida, intente nuevamente." << endl;
                break;
        }

        cout << "¿Desea salir? (1. Sí, 2. No): ";
        int opcionSalir;
        cin >> opcionSalir;
        if (opcionSalir == 1) {
            salir = true;
        }
    }

    cout << "¡Gracias por usar el sistema de biblioteca!" << endl;
    return 0;
}
