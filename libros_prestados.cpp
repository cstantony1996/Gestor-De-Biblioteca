// libros_prestados.cpp
#include "libros_prestados.h"

using namespace std;

Biblioteca::Biblioteca() {
    conn = nullptr;
}

Biblioteca::~Biblioteca() {
    if (conn != nullptr) {
        PQfinish(conn);
    }
}

bool Biblioteca::conectarDB(const string& conexion) {
    conn = PQconnectdb(conexion.c_str());
    if (PQstatus(conn) != CONNECTION_OK) {
        cerr << "Error al conectar a la base de datos: " << PQerrorMessage(conn) << endl;
        return false;
    }
    return true;
}

vector<Libro> Biblioteca::buscarLibros(const string& texto) {
    vector<Libro> libros;
    string query = "SELECT libros_prestados.id, libros.titulo AS nombre_libro, libros.autor, "
                    "libros_prestados.fecha_prestamo, libros_prestados.fecha_devolucion, libros_prestados.estado "
                    "FROM libros_prestados "
                    "JOIN libros ON libros_prestados.id_libro = libros.id "
                    "WHERE libros.titulo ILIKE '%" + texto + "%' OR libros.autor ILIKE '%" + texto + "%';";

    PGresult *res = PQexec(conn, query.c_str());

    if (PQresultStatus(res) != PGRES_TUPLES_OK) {
        cerr << "Error al ejecutar la consulta: " << PQerrorMessage(conn) << endl;
        PQclear(res);
        return libros;
    }

    int rows = PQntuples(res);
    for (int i = 0; i < rows; i++) {
        int id = stoi(PQgetvalue(res, i, 0));
        string nombre = PQgetvalue(res, i, 1);
        string autor = PQgetvalue(res, i, 2);
        string estado = PQgetvalue(res, i, 5);
        string fecha_prestamo = PQgetvalue(res, i, 3);
        string fecha_devolucion = PQgetvalue(res, i, 4);

        libros.push_back(Libro(id, nombre, autor, estado, fecha_prestamo, fecha_devolucion));
    }

    PQclear(res);
    return libros;
}

bool Biblioteca::prestarLibro(int id_libro, const string& usuario) {
    // Verificar si el libro está disponible antes de prestarlo
    string checkQuery = "SELECT estado FROM libros WHERE id = " + to_string(id_libro) + ";";
    PGresult *res = PQexec(conn, checkQuery.c_str());

    if (PQresultStatus(res) != PGRES_TUPLES_OK) {
        cerr << "Error al verificar disponibilidad del libro: " << PQerrorMessage(conn) << endl;
        PQclear(res);
        return false;
    }

    string estado = PQgetvalue(res, 0, 0);
    PQclear(res);

    if (estado != "Disponible") {
        cerr << "El libro ya está prestado o no existe." << endl;
        return false;
    }

    // Si el libro está disponible, se actualiza su estado en la tabla libros
    string updateQuery = "UPDATE libros SET estado = 'prestado' WHERE id = " + to_string(id_libro) + ";";
    res = PQexec(conn, updateQuery.c_str());

    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        cerr << "Error al actualizar estado del libro: " << PQerrorMessage(conn) << endl;
        PQclear(res);
        return false;
    }
    PQclear(res);

    // Registrar el préstamo en la tabla libros_prestados
    string insertQuery = "INSERT INTO libros_prestados (id_libro, fecha_prestamo, fecha_devolucion, estado) "
                              "VALUES (" + to_string(id_libro) + ", CURRENT_DATE, CURRENT_DATE + INTERVAL '14 days', 'prestado');";
    res = PQexec(conn, insertQuery.c_str());

    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        cerr << "Error al registrar el préstamo: " << PQerrorMessage(conn) << endl;
        PQclear(res);
        return false;
    }

    PQclear(res);
    return true;
}

bool Biblioteca::cambiarEstadoLibro(int id_libro, const string& estado) {
    string query = "UPDATE libros_prestados SET estado = '" + estado + "' WHERE id = " + to_string(id_libro) + ";";
    PGresult *res = PQexec(conn, query.c_str());

    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        cerr << "Error al cambiar el estado del libro: " << PQerrorMessage(conn) << endl;
        PQclear(res);
        return false;
    }

    PQclear(res);
    return true;
}

void Biblioteca::mostrarDetallesLibro(int id_libro) {
    string query = "SELECT * FROM libros_prestados WHERE id = " + to_string(id_libro) + ";";
    PGresult *res = PQexec(conn, query.c_str());

    if (PQresultStatus(res) != PGRES_TUPLES_OK) {
        cerr << "Error al ejecutar la consulta: " << PQerrorMessage(conn) << endl;
        PQclear(res);
        return;
    }

    int rows = PQntuples(res);
    if (rows == 1) {
        cout << "ID: " << PQgetvalue(res, 0, 0) << endl;
        cout << "Nombre del libro: " << PQgetvalue(res, 0, 1) << endl;
        cout << "Autor: " << PQgetvalue(res, 0, 2) << endl;
        cout << "Estado: " << PQgetvalue(res, 0, 5) << endl;
        cout << "Fecha de préstamo: " << PQgetvalue(res, 0, 3) << endl;
        cout << "Fecha de devolución: " << PQgetvalue(res, 0, 4) << endl;
    } else {
        cout << "Libro no encontrado." << endl;
    }

    PQclear(res);
}
