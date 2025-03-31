#include "Connection.h"
#include "Ingresar_libros.h"
#include "Listar_libros.h"
#include <iostream>
 
using namespace std;

int main() {
    try {
        PGconn *conn = conectarDB();
        if (!conn)
            throw runtime_error("No se pudo conectar a la base de datos.");

        int opcion;
        do {
            cout << "1. Ingresar libro" << endl;
            cout << "2. Listar libros" << endl;
            cout << "3. Salir" << endl;
            cout << "Opción: ";
            cin >> opcion;
            cin.ignore();

            if (opcion == 1) {
                Libro libro(conn);
                libro.ingresarDatos();
                libro.guardarEnDB();
            } else if (opcion == 2) {
                ListarLibros ListarLibros(conn);
                ListarLibros.mostrarLibrosOrdenados();
            } else if (opcion != 3) {
                cout << "Opcion no valida" << endl;
            }
        } while (opcion != 3);

        PQfinish(conn);
    } catch (const exception &e) {
        cerr << "Error: " << e.what() << endl;
    }

    cout << "Presione Enter para salir...";
    cin.get();
    return 0;

}