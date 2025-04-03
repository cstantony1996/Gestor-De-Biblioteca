// libros_prestados.h
#ifndef LIBROS_PRESTADOS_H
#define LIBROS_PRESTADOS_H

#include <iostream>
#include <string>
#include <vector>
#include <libpq-fe.h>  // Librería de PostgreSQL

class Libro {
public:
    int id;
    std::string nombre;
    std::string autor;
    std::string estado;
    std::string fecha_prestamo;
    std::string fecha_devolucion;

    Libro(int _id, std::string _nombre, std::string _autor, std::string _estado, std::string _fecha_prestamo, std::string _fecha_devolucion)
        : id(_id), nombre(_nombre), autor(_autor), estado(_estado), fecha_prestamo(_fecha_prestamo), fecha_devolucion(_fecha_devolucion) {}
};

class Biblioteca {
private:
    PGconn *conn; // Conexión con PostgreSQL

public:
    Biblioteca();
    ~Biblioteca();
    bool conectarDB(const std::string& conexion);
    std::vector<Libro> buscarLibros(const std::string& texto);
    bool prestarLibro(int id_libro, const std::string& usuario);
    bool cambiarEstadoLibro(int id_libro, const std::string& estado);
    void mostrarDetallesLibro(int id_libro);
};

#endif
