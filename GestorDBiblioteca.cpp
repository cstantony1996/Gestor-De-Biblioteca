#include "Connection.h"
#include "Ingresar_libros.h"    // Incluimos la declaración de la función Agregar_libros
#include <iostream>
#include <stdexcept>

using namespace std;

void menuUsuario(PGconn *conn) {
    int opcion;
    bool salir = false;

    while (!salir) {
        cout << "\n--- Menú Usuario ---" << endl;
        cout << "1. Ingresar libro" << endl;
        cout << "2. Listar libros" << endl;
        cout << "3. Prestar libro" << endl;
        cout << "4. Devolver libro" << endl;
        cout << "5. Salir" << endl;
        cout << "Seleccione una opción: ";
        cin >> opcion;

        switch (opcion) {
            case 1: {
                cout << "Opción para Ingresar libros seleccionada..." << endl;
                // Llamamos a la función Agregar_libros para agregar el libro
                Libro libro(conn);
                libro.ingresarDatos();  // Llamamos al método que permite ingresar el libro
                libro.guardarEnDB();
                break;
            }
            case 2: {
                // Aquí puedes llamar a la función para listar los libros
                cout << "Opción para listar libros seleccionada..." << endl;
                break;
            }
            case 3:
                // Aquí puedes llamar a la función para prestar un libro
                cout << "Opción para prestar libro seleccionada..." << endl;
                break;
            case 4:
                // Aquí puedes llamar a la función para devolver un libro
                cout << "Opción para devolver libro seleccionada..." << endl;
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

void menuAdministrador(PGconn *conn) {
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
            cout << "Opción para Ingresar libros seleccionada..." << endl;
                // Llamamos a la función Agregar_libros para agregar el libro
            //Ingresar_libros();  // Llamamos a la función que agregará el libro
                break;
            case 2:
                // Aquí puedes llamar a la función para listar los libros
                cout << "Opción para listar libros seleccionada..." << endl;
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

    // Conectar a la base de datos
    PGconn *conn = nullptr;
    try {
        conn = conectarDB();  // Inicializa la conexión a la base de datos

        if (!conn) {
            throw runtime_error("No se pudo establecer conexión con la base de datos.");
        }
        cout << "Conexión a la base de datos establecida exitosamente." << endl;

        // Menú Principal
        while (!salir) {
            cout << "\n--- Menú Principal ---" << endl;
            cout << "1. Usuario" << endl;
            cout << "2. Administrador" << endl;
            cout << "Seleccione una opción: ";
            cin >> opcion;

            switch (opcion) {
                case 1:
                    menuUsuario(conn);  // Llama al menú de usuario
                    break;
                case 2:
                    menuAdministrador(conn);  // Llama al menú de administrador
                    break;
                default:
                    cout << "Opción no válida, intente nuevamente." << endl;
                    break;
            }

            // Preguntamos si el usuario quiere salir
            cout << "¿Desea salir? (1. Sí, 2. No): ";
            int opcionSalir;
            cin >> opcionSalir;
            if (opcionSalir == 1) {
                salir = true;
            }
        }

        // Después de salir del menú principal, cerramos la conexión a la base de datos
        PQfinish(conn);
        cout << "¡Gracias por usar el sistema de biblioteca!" << endl;

    } catch (const exception &e) {
        cerr << "Error crítico: " << e.what() << endl;
    }

    return 0;
}
