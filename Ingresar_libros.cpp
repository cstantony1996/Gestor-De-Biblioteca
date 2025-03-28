#include "Ingresar_libros.h"
#include <iostream>
#include <stdexcept>
#include <cctype>
#include <regex>

using namespace std;

Libro::Libro(PGconn *conexion) : conn(conexion) {}

bool esNumero(const string& str) {
    return all_of(str.begin(), str.end(), ::isdigit);
}

bool LetrasConEspacios(const string& str) {
    return all_of(str.begin(), str.end(), [](char c) {
        return isalpha(c) || c == ' ' || c == '.'; // Permite letras, espacios y puntos
    });
}

bool ValidarTextoGeneral(const string& str) {
    // Permite letras, números, espacios y caracteres comunes en títulos
    for (char c : str) {
        if (!(isalnum(c) || c == ' ' || c == ',' || c == '.' || c == '-')) {
            // Verificar caracteres especiales en español
            switch(static_cast<unsigned char>(c)) {
                case 0xE1: // á
                case 0xE9: // é
                case 0xED: // í
                case 0xF3: // ó
                case 0xFA: // ú
                case 0xF1: // ñ
                case 0xC1: // Á
                case 0xC9: // É
                case 0xCD: // Í
                case 0xD3: // Ó
                case 0xDA: // Ú
                case 0xD1: // Ñ
                    continue;
                default:
                    return false;
            }
        }
    }
    return true;
}

void Libro::ingresarDatos(){
    try {
        // Título: permite letras, números y espacios
        do {
            cout << "Ingrese el título: ";
            getline(cin, titulo);
            if (titulo.empty()) {
                cout << "El título es obligatorio." << endl;
            } else if (!ValidarTextoGeneral(titulo)) {
                cout << "El título contiene caracteres no válidos." << endl;
                titulo.clear();
            }
        } while (titulo.empty() || !ValidarTextoGeneral(titulo));

        // Autor: permite letras y espacios
        do {
            cout << "Ingrese el autor: ";
            getline(cin, autor);
            if (autor.empty()) {
                cout << "El autor es obligatorio." << endl;
            } else if (!LetrasConEspacios(autor)) {
                cout << "El autor solo puede contener letras y espacios." << endl;
                autor.clear();
            }
        } while (autor.empty() || !LetrasConEspacios(autor));

        // ISBN: solo números (sin cambios)
        do {
            cout << "Ingrese el ISBN: ";
            getline(cin, isbn);
            if (isbn.empty()) {
                cout << "El ISBN es obligatorio." << endl;
            } else if (!esNumero(isbn)) {
                cout << "El ISBN solo puede contener números." << endl;
                isbn.clear();
            } else if (isbnExiste()) {
                cout << "El ISBN ya está registrado." << endl;
                isbn.clear();
            }
        } while (isbn.empty() || !esNumero(isbn) || isbnExiste());

        // Editorial: permite letras y espacios
        do {
            cout << "Ingrese la editorial: ";
            getline(cin, editorial);
            if (editorial.empty()) {
                cout << "La editorial es obligatoria." << endl;
            } else if (!LetrasConEspacios(editorial)) {
                cout << "La editorial solo puede contener letras y espacios." << endl;
                editorial.clear();
            }
        } while (editorial.empty() || !LetrasConEspacios(editorial));

        // Año de publicación: solo números (sin cambios)
        do {
            cout << "Ingrese año de publicación: ";
            getline(cin, año_publicacion);
            if (año_publicacion.empty()) {
                cout << "El año de publicación es obligatorio." << endl;
            } else if (!esNumero(año_publicacion)) {
                cout << "El año de publicación solo puede contener números." << endl;
                año_publicacion.clear();
            }
        } while (año_publicacion.empty() || !esNumero(año_publicacion));

        // Materia: permite letras y espacios
        do {
            cout << "Ingrese la materia: ";
            getline(cin, materia);
            if (materia.empty()) {
                cout << "La materia es obligatoria." << endl;
            } else if (!LetrasConEspacios(materia)) {
                cout << "La materia solo puede contener letras y espacios." << endl;
                materia.clear();
            }
        } while (materia.empty() || !LetrasConEspacios(materia));

    } catch (const invalid_argument &e) {
        cerr << "Error: " << e.what() << endl;
        ingresarDatos();
    }
}
 
 bool Libro::isbnExiste() {
 
     const char *query = "SELECT COUNT(*) FROM agregar_libros WHERE isbn = $1";
     const char *paramValues[1] = {isbn.c_str()};
     PGresult *res = PQexecParams(conn, query, 1, NULL, paramValues, NULL, NULL, 0);
 
     if (PQresultStatus(res) != PGRES_TUPLES_OK) {
         cerr << "Error en la consulta: " << PQerrorMessage(conn) << endl;
         PQclear(res);
         throw runtime_error("No se pudo verificar el ISBN.");
     }
 
     bool existe = stoi(PQgetvalue(res, 0, 0)) > 0;
     PQclear(res);
     return existe;
 }
 
 void Libro::guardarEnDB() {
    try {
        if (!conn)
            throw runtime_error("No hay conexión a la base de datos.");

        // 🚀 Verificar si el ISBN ya existe en agregar_libros
        if (isbnExiste()) {
            cout << "⚠️ El ISBN ya está registrado." << endl;
            return;
        }

        const char *paramValues[6] = {titulo.c_str(), autor.c_str(), isbn.c_str(), editorial.c_str(), año_publicacion.c_str(), materia.c_str()};
        
        // 🚀 Insertar en agregar_libros
        const char *insertQuery1 = "INSERT INTO agregar_libros (titulo, autor, isbn, editorial, año_publicacion, materia) VALUES ($1, $2, $3, $4, $5, $6)";
        PGresult *insertRes1 = PQexecParams(conn, insertQuery1, 6, NULL, paramValues, NULL, NULL, 0);
        
        if (PQresultStatus(insertRes1) != PGRES_COMMAND_OK) {
            PQclear(insertRes1);
            throw runtime_error("Error al insertar: " + string(PQerrorMessage(conn)));
        }
        PQclear(insertRes1);

        // 🚀 Insertar en libros
        const char *insertQuery2 = "INSERT INTO libros (titulo, autor, isbn, editorial, año_publicacion, materia) VALUES ($1, $2, $3, $4, $5, $6)";
        PGresult *insertRes2 = PQexecParams(conn, insertQuery2, 6, NULL, paramValues, NULL, NULL, 0);
        
        if (PQresultStatus(insertRes2) != PGRES_COMMAND_OK) {
            PQclear(insertRes2);
            throw runtime_error("Error al insertar: " + string(PQerrorMessage(conn)));
        }
        PQclear(insertRes2);

        cout << "📚 Libro: " << titulo << " ha sido agregado exitosamente." << endl;

    } catch (const exception &e) {
        cerr << "⚠️ Error al insertar datos: " << e.what() << endl;
    }
}