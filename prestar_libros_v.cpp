#include <windows.h>
#include <string>
#include <vector>
#include <libpq-fe.h>
#include "libros_prestados.h"

using namespace std;

// Variables globales
HWND hNombreLibro, hResultadoBusqueda, hPrestarButton, hBuscarButton;
Biblioteca biblioteca;

// Prototipos de funciones
LRESULT CALLBACK WindowProcedure(HWND, UINT, WPARAM, LPARAM);
void BuscarLibro(HWND hwnd);
void PrestarLibro(HWND hwnd);

int WINAPI wWinMain(HINSTANCE hInst, HINSTANCE hPrevInst, PWSTR lpCmdLine, int nCmdShow) {
    const wchar_t CLASS_NAME[] = L"BibliotecaApp";
    
    WNDCLASSW wc = { };
    wc.lpfnWndProc = WindowProcedure;
    wc.hInstance = hInst;
    wc.lpszClassName = CLASS_NAME;
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    
    if (!RegisterClassW(&wc)) {
        MessageBoxW(NULL, L"Registro de ventana fallido!", L"Error", MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    HWND hwnd = CreateWindowExW(0, CLASS_NAME, L"Sistema de Biblioteca", WS_OVERLAPPEDWINDOW, 
                               CW_USEDEFAULT, CW_USEDEFAULT, 500, 400, 
                               NULL, NULL, hInst, NULL);

    if (!hwnd) {
        MessageBoxW(NULL, L"Creación de ventana fallida!", L"Error", MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    if (!biblioteca.conectarDB("dbname=postgres user=postgres password=Myroot")) {
        MessageBoxW(hwnd, L"Error de conexión a la base de datos", L"Error", MB_ICONERROR | MB_OK);
        return 0;
    }

    MSG msg = { };
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}

LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) {
    switch (msg) {
        case WM_CREATE: {
            // Crear etiqueta
            CreateWindowW(L"STATIC", L"Nombre del libro:", WS_CHILD | WS_VISIBLE,
                        20, 20, 150, 20, hwnd, NULL, NULL, NULL);
            
            // Campo de entrada para el nombre del libro
            hNombreLibro = CreateWindowW(L"EDIT", L"", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
                                      20, 45, 200, 25, hwnd, (HMENU)1, NULL, NULL);
            
            // Botón de búsqueda
            hBuscarButton = CreateWindowW(L"BUTTON", L"Buscar", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                                       230, 45, 80, 25, hwnd, (HMENU)2, NULL, NULL);
            
            // Botón de préstamo
            hPrestarButton = CreateWindowW(L"BUTTON", L"Prestar", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                                        20, 80, 100, 30, hwnd, (HMENU)3, NULL, NULL);
            
            // Área de resultados
            hResultadoBusqueda = CreateWindowW(L"STATIC", L"", WS_CHILD | WS_VISIBLE | SS_LEFT,
                                           20, 120, 450, 60, hwnd, (HMENU)4, NULL, NULL);
            break;
        }

        case WM_COMMAND: {
            switch (LOWORD(wp)) {
                case 2:  // Botón Buscar
                    BuscarLibro(hwnd);
                    break;
                    
                case 3:  // Botón Prestar
                    PrestarLibro(hwnd);
                    break;
            }
            break;
        }

        case WM_DESTROY:
            PostQuitMessage(0);
            break;

        default:
            return DefWindowProcW(hwnd, msg, wp, lp);
    }
    return 0;
}

void BuscarLibro(HWND hwnd) {
    wchar_t buffer[255];
    GetWindowTextW(hNombreLibro, buffer, 255);
    
    // Convertir a string normal para la biblioteca
    string nombreLibro;
    int len = wcslen(buffer) + 1;
    char* mbstr = new char[len];
    wcstombs(mbstr, buffer, len);
    nombreLibro = mbstr;
    delete[] mbstr;

    vector<Libro> libros = biblioteca.buscarLibros(nombreLibro);
    wstring resultado;
    
    if (!libros.empty()) {
        // Convertir el resultado a wide string
        string temp = "Libro encontrado: " + libros[0].nombre + " - " + libros[0].autor + "\nEstado: " + libros[0].estado;
        resultado = wstring(temp.begin(), temp.end());
    } else {
        resultado = L"No se encontró el libro.";
    }
    
    SetWindowTextW(hResultadoBusqueda, resultado.c_str());
}

void PrestarLibro(HWND hwnd) {
    wchar_t buffer[255];
    GetWindowTextW(hNombreLibro, buffer, 255);
    
    // Convertir a string normal
    string nombreLibro;
    int len = wcslen(buffer) + 1;
    char* mbstr = new char[len];
    wcstombs(mbstr, buffer, len);
    nombreLibro = mbstr;
    delete[] mbstr;

    vector<Libro> libros = biblioteca.buscarLibros(nombreLibro);

    if (!libros.empty()) {
        if (libros[0].estado == "disponible") {
            if (biblioteca.prestarLibro(libros[0].id, "usuario1")) {
                MessageBoxW(hwnd, L"Libro prestado correctamente.", L"Éxito", MB_OK);
                
                // Actualizar estado en la interfaz
                wstring nuevoEstado = L"Libro prestado: " + wstring(libros[0].nombre.begin(), libros[0].nombre.end());
                SetWindowTextW(hResultadoBusqueda, nuevoEstado.c_str());
            } else {
                MessageBoxW(hwnd, L"Error al realizar el préstamo.", L"Error", MB_OK);
            }
        } else {
            MessageBoxW(hwnd, L"El libro no está disponible.", L"Error", MB_OK);
        }
    } else {
        MessageBoxW(hwnd, L"Libro no encontrado.", L"Error", MB_OK);
    }
}