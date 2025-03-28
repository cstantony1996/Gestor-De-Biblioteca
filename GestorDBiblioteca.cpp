#include "Connection.h"
#include "Ingresar_libros.h"
#include "UserAuth.h"
#include <iostream>
#include <limits>
 
 using namespace std;
 
 int main() {
     try {
         PGconn *conn = conectarDB();
         if (!conn) {
             throw runtime_error("No se pudo establecer conexión con la base de datos.");
         }
         
         UserAuth auth(conn);

         auth.showAuthMenu();

         if (!auth.isAuthenticated()) {
            cout << "⚠️ No se pudo iniciar sesión." << endl;
            PQfinish(conn);
            return 0;
        }
             
         cout << "\n════════ SISTEMA DE GESTIÓN DE LIBROS ════════" << endl;
         Libro libro(conn);
         libro.ingresarDatos();
         libro.guardarEnDB();
 
         PQfinish(conn);

     } catch (const exception &e) {
         cerr << "Error crítico: " << e.what() << endl;
     }
 
     cout << "\nPresiona Enter para salir..." << endl;
     cin.ignore(numeric_limits<streamsize>::max(), '\n');
     cin.get();

     return 0;
 }