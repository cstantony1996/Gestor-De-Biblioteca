#include "Listar_libros.h"
#include <iostream>
#include <stdexcept>

using namespace std;

ListarLibros::ListarLibros(PGconn *conexion) : conn(conexion) {}

void ListarLibros::mostrarLibrosOrdenados() {
    try {
        if (!conn)
            throw runtime_error("No hay conexión a la base de datos.");

        const char *query = "SELECT titulo, autor, isbn, editorial, año_publicacion, materia FROM libros ORDER BY titulo ASC";
        PGresult *res = PQexec(conn, query);

        if (PQresultStatus(res) != PGRES_TUPLES_OK) {
            PQclear(res);
            throw runtime_error("Error al ejecutar la consulta: " + string(PQerrorMessage(conn)));
        }

        int rows = PQntuples(res);
        if (rows == 0) {
            cout << "No hay libros registrados." << endl;
        } else {
            cout << "Libros ordenados alfabéticamente:" << endl;
            for (int i = 0; i < rows; ++i) {
                cout << "Título: " << PQgetvalue(res, i, 0) << endl;
                cout << "Autor: " << PQgetvalue(res, i, 1) << endl;
                cout << "ISBN: " << PQgetvalue(res, i, 2) << endl;
                cout << "Editorial: " << PQgetvalue(res, i, 3) << endl;
                cout << "Año de publicación: " << PQgetvalue(res, i, 4) << endl;
                cout << "Materia: " << PQgetvalue(res, i, 5) << endl;
                cout << "-------------------------" << endl;
            }
        }

        PQclear(res);
    } catch (const exception &e) {
        cerr << "Error: " << e.what() << endl;
    }
}