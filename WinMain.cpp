// WinMain.cpp
#include <windows.h>
#include "VentanasGUI.h"
#include "Connection.h"
#include "UserAuth.h"
#include <iostream>

PGconn* conn = nullptr;  // Conexión global a la base de datos

// Declaración de inicialización
void InicializarAplicacion(HINSTANCE hInstance);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    // Inicialización global
    InicializarAplicacion(hInstance);

    // Mostrar la ventana de login
    MostrarLogin(hInstance);

    // Bucle principal de mensajes
    MSG msg;
    while (GetMessageW(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    // Limpieza al cerrar
    if (conn != nullptr) {
        PQfinish(conn);
        conn = nullptr;
    }

    return static_cast<int>(msg.wParam);
}

void InicializarAplicacion(HINSTANCE hInstance) {
    // Conexión a la base de datos
    conn = conectarDB();
    if (!conn) {
        MessageBoxW(nullptr, L"No se pudo conectar a la base de datos.", L"Error", MB_OK | MB_ICONERROR);
        exit(1);  // Salir si no se puede conectar
    }

    // Puedes agregar otras inicializaciones necesarias aquí
    std::cout << "Aplicación inicializada correctamente.\n";
}
