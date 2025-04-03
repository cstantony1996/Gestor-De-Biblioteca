// libros_prestados.cpp
#include "libros_prestados.h"

Biblioteca::Biblioteca() {
    conn = nullptr;
}

Biblioteca::~Biblioteca() {
    if (conn != nullptr) {
        PQfinish(conn);
    }
}

bool Biblioteca::conectarDB(const std::string& conexion) {
    conn = PQconnectdb(conexion.c_str());
    if (PQstatus(conn) != CONNECTION_OK) {
        std::cerr << "Error al conectar a la base de datos: " << PQerrorMessage(conn) << std::endl;
        return false;
    }
    return true;
}

std::vector<Libro> Biblioteca::buscarLibros(const std::string& texto) {
    std::vector<Libro> libros;
    std::string query = "SELECT * FROM libros_prestados WHERE nombre_libro ILIKE '%" + texto + "%' OR autor ILIKE '%" + texto + "%';";
    PGresult *res = PQexec(conn, query.c_str());

    if (PQresultStatus(res) != PGRES_TUPLES_OK) {
        std::cerr << "Error al ejecutar la consulta: " << PQerrorMessage(conn) << std::endl;
        PQclear(res);
        return libros;
    }

    int rows = PQntuples(res);
    for (int i = 0; i < rows; i++) {
        int id = std::stoi(PQgetvalue(res, i, 0));
        std::string nombre = PQgetvalue(res, i, 1);
        std::string autor = PQgetvalue(res, i, 2);
        std::string estado = PQgetvalue(res, i, 5);
        std::string fecha_prestamo = PQgetvalue(res, i, 3);
        std::string fecha_devolucion = PQgetvalue(res, i, 4);

        libros.push_back(Libro(id, nombre, autor, estado, fecha_prestamo, fecha_devolucion));
    }

    PQclear(res);
    return libros;
}

bool Biblioteca::prestarLibro(int id_libro, const std::string& usuario) {
    std::string query = "UPDATE libros_prestados SET estado = 'prestado', fecha_prestamo = CURRENT_DATE, fecha_devolucion = CURRENT_DATE + INTERVAL '14 days' WHERE id = " + std::to_string(id_libro) + " AND estado = 'disponible';";
    PGresult *res = PQexec(conn, query.c_str());

    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        std::cerr << "Error al realizar el préstamo: " << PQerrorMessage(conn) << std::endl;
        PQclear(res);
        return false;
    }

    PQclear(res);
    return true;
}

bool Biblioteca::cambiarEstadoLibro(int id_libro, const std::string& estado) {
    std::string query = "UPDATE libros_prestados SET estado = '" + estado + "' WHERE id = " + std::to_string(id_libro) + ";";
    PGresult *res = PQexec(conn, query.c_str());

    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        std::cerr << "Error al cambiar el estado del libro: " << PQerrorMessage(conn) << std::endl;
        PQclear(res);
        return false;
    }

    PQclear(res);
    return true;
}

void Biblioteca::mostrarDetallesLibro(int id_libro) {
    std::string query = "SELECT * FROM libros_prestados WHERE id = " + std::to_string(id_libro) + ";";
    PGresult *res = PQexec(conn, query.c_str());

    if (PQresultStatus(res) != PGRES_TUPLES_OK) {
        std::cerr << "Error al ejecutar la consulta: " << PQerrorMessage(conn) << std::endl;
        PQclear(res);
        return;
    }

    int rows = PQntuples(res);
    if (rows == 1) {
        std::cout << "ID: " << PQgetvalue(res, 0, 0) << std::endl;
        std::cout << "Nombre del libro: " << PQgetvalue(res, 0, 1) << std::endl;
        std::cout << "Autor: " << PQgetvalue(res, 0, 2) << std::endl;
        std::cout << "Estado: " << PQgetvalue(res, 0, 5) << std::endl;
        std::cout << "Fecha de préstamo: " << PQgetvalue(res, 0, 3) << std::endl;
        std::cout << "Fecha de devolución: " << PQgetvalue(res, 0, 4) << std::endl;
    } else {
        std::cout << "Libro no encontrado." << std::endl;
    }

    PQclear(res);
}
