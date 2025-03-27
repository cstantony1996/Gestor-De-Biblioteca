#include "Listar_libros.h"
#include <iostream>
#include <stdexcept>
#include <iomanip>

using namespace std;

string limpiarAjustar(const char* texto, int ancho) {
    string str(texto);

    // elimina caracteres no ASCII, espacios extras y caracteres de control
    str.erase(remove_if(str.begin(), str.end(), [](char c) {
        return (c < 32 || c > 126); // caracteres ASCII imprimibles
    }), str.end());

    // trunca si es necesario y añade "..." si se corta
    if (str.length() > static_cast<size_t>(ancho)) {
        return str.substr(0, ancho -3) + "...";
    }
    return str;
}

ListarLibros::ListarLibros(PGconn *conexion) : conn(conexion) {}

void ListarLibros::mostrarLibrosOrdenados() {
    try {
        if (!conn)
            throw runtime_error("No hay conexión a la base de datos.");

        const char *query = R"(
            SELECT 
            l.titulo,
            l.autor,
            l.isbn,
            CASE
                WHEN p.id_prestamo IS NULL THEN 'Disponible'
                ELSE 'Prestado'
            END AS estado
        FROM libros l
        LEFT JOIN prestamos p ON l.id = p.id_libro AND p.fecha_devolucion IS NULL
        ORDER BY l.titulo
        )";

        PGresult *res = PQexec(conn, query);

        if (PQresultStatus(res) != PGRES_TUPLES_OK) {
            PQclear(res);
            throw runtime_error("Error al ejecutar la consulta: " + string(PQerrorMessage(conn)));
        }

        const int anchoTitulo = 40;
        const int anchoAutor = 35;
        const int anchoISBN = 30;
        const int anchoEstado = 20;

        cout << "\nListado de Libros\n";
        cout << string(anchoTitulo + anchoAutor + anchoISBN + anchoEstado, '-') << endl;
        cout << left
             << setw(anchoTitulo) << "Titulo"
             << setw(anchoAutor) << "Autor"
             << setw(anchoISBN) << "ISBN"
             << setw(anchoEstado) << "Estado" << endl;
        cout << string(anchoTitulo + anchoAutor + anchoISBN + anchoEstado, '-') << endl;

        for (int i = 0; i < PQntuples(res); ++i) {
            cout << left
                 << setw(anchoTitulo) << limpiarAjustar(PQgetvalue(res, i, 0), anchoTitulo) // titulo
                 << setw(anchoAutor) << limpiarAjustar(PQgetvalue(res, i, 1), anchoAutor) // autor
                 << setw(anchoISBN) << limpiarAjustar(PQgetvalue(res, i, 2), anchoISBN) // isbn
                 << setw(anchoEstado) << limpiarAjustar(PQgetvalue(res, i, 3), anchoEstado) << endl; // estado
        }

        cout << "\n\n";

        PQclear(res);
    } catch (const exception &e) {
        cerr << "Error: " << e.what() << endl;
    }
}