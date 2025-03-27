#ifndef LISTAR_LIBROS_H
#define LISTAR_LIBROS_H

#include <libpq-fe.h>
#include <iostream>
#include <vector>
#include <algorithm>

class ListarLibros {
private:
    PGconn *conn;

public:
    ListarLibros(PGconn *conexion);
    void mostrarLibrosOrdenados();
};

#endif