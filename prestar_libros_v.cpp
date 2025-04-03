#include <windows.h>
#include <string>
#include "libros_prestados.h"
#include <tchar.h>  // Asegúrate de que este archivo esté disponible y correcto

// Declaración de variables globales
HWND hNombreLibro, hResultadoBusqueda, hPrestarButton;
Biblioteca biblioteca;

// Función para manejar los eventos de la ventana
LRESULT CALLBACK WindowProcedure(HWND, UINT, WPARAM, LPARAM);

// Función principal que crea la ventana
int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR lpCmdLine, int nCmdShow) {
    const char CLASS_NAME[] = "BibliotecaApp";
    
    WNDCLASS wc = { };
    wc.lpfnWndProc = WindowProcedure;  // Función que maneja los mensajes de la ventana
    wc.hInstance = hInst;
    wc.lpszClassName = CLASS_NAME;
    
    if (!RegisterClass(&wc)) {
        MessageBox(NULL, "Window Registration Failed!", "Error", MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    // Crear la ventana
    HWND hwnd = CreateWindowEx(0, CLASS_NAME, "Sistema de Biblioteca", WS_OVERLAPPEDWINDOW, 
                               CW_USEDEFAULT, CW_USEDEFAULT, 500, 400, 
                               NULL, NULL, hInst, NULL);

    if (!hwnd) {
        MessageBox(NULL, "Window Creation Failed!", "Error", MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    // Mostrar la ventana
    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    // Conectar a la base de datos PostgreSQL
    if (!biblioteca.conectarDB("dbname=library user=postgres password=yourpassword")) {
        MessageBox(hwnd, "Error de conexión a la base de datos", "Error", MB_ICONERROR | MB_OK);
        return 0;
    }

    // Bucle de mensajes
    MSG msg = { };
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}

// Función de procedimiento de la ventana
LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) {
    switch (msg) {
        case WM_CREATE:
            // Crear controles
            hNombreLibro = CreateWindowEx(0, "EDIT", "", WS_CHILD | WS_VISIBLE | WS_BORDER, 
                                          50, 50, 200, 25, hwnd, (HMENU)1, NULL, NULL);
            hPrestarButton = CreateWindowEx(0, "BUTTON", "Prestar", WS_CHILD | WS_VISIBLE, 
                                            50, 100, 100, 30, hwnd, (HMENU)2, NULL, NULL);
            hResultadoBusqueda = CreateWindowEx(0, "STATIC", "", WS_CHILD | WS_VISIBLE, 
                                                50, 150, 400, 25, hwnd, (HMENU)3, NULL, NULL);
            break;

        case WM_COMMAND:
            if (LOWORD(wp) == 1) {  // Buscar libro cuando se ingresa texto en el campo
                char buffer[255];
                GetWindowText(hNombreLibro, buffer, sizeof(buffer));
                std::string textoBusqueda = buffer;

                std::vector<Libro> libros = biblioteca.buscarLibros(textoBusqueda);
                if (!libros.empty()) {
                    std::string resultado = "Libro encontrado: " + libros[0].nombre + " - " + libros[0].autor;
                    SetWindowText(hResultadoBusqueda, resultado.c_str());
                } else {
                    SetWindowText(hResultadoBusqueda, "No se encontró el libro.");
                }
            }

            if (LOWORD(wp) == 2) {  // Realizar el préstamo cuando se presiona el botón
                char buffer[255];
                GetWindowText(hNombreLibro, buffer, sizeof(buffer));
                std::string nombreLibro = buffer;
                
                std::vector<Libro> libros = biblioteca.buscarLibros(nombreLibro);

                if (!libros.empty() && libros[0].estado == "disponible") {
                    if (biblioteca.prestarLibro(libros[0].id, "usuario1")) {
                        MessageBox(hwnd, "Libro prestado correctamente.", "Éxito", MB_OK);
                    } else {
                        MessageBox(hwnd, "Error al realizar el préstamo.", "Error", MB_OK);
                    }
                } else {
                    MessageBox(hwnd, "El libro ya está prestado o no existe.", "Error", MB_OK);
                }
            }

            break;

        case WM_DESTROY:
            PostQuitMessage(0);
            break;

        default:
            return DefWindowProc(hwnd, msg, wp, lp);
    }

    return 0;
}
