// VentanasGUI.cpp
#include "VentanasGUI.h"
#include "UserAuth.h" // Asegúrate de que UserAuth.h esté incluido si lo necesitas aquí
#include "Connection.h" // Asegúrate de que Connection.h esté incluido si lo necesitas aquí
#include <windows.h>
#include <string>
#include <memory>
#include <iostream> // Para mensajes de error

// Variables globales (definiciones)
HWND hUsuario = nullptr;
HWND hContraseña = nullptr;
HWND hBotonLogin = nullptr;
HWND hVentanaPrincipal = nullptr;

HWND hTitulo = nullptr;
HWND hAutor = nullptr;
HWND hIsbn = nullptr;
HWND hEditorial = nullptr;
HWND hMateria = nullptr;
HWND hAñoPublicacion = nullptr;
HWND hGuardarLibro = nullptr;
HWND hListarLibros = nullptr;

PGconn* conn = nullptr;
std::unique_ptr<UserAuth> auth = nullptr;
std::unique_ptr<Libro> libro = nullptr;
std::unique_ptr<ListarLibros> listarLibros = nullptr;
HINSTANCE hInst = nullptr; // Se establecerá en WinMain

// --- Funciones para mostrar las ventanas ---

void MostrarLogin(HINSTANCE hInstance) {
    hInst = hInstance; // Guardar la instancia para usarla globalmente si es necesario

    WNDCLASSW wc = {};
    wc.lpfnWndProc = LoginProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = L"LoginWindowClass"; // Nombre de clase para la ventana de login

    if (!RegisterClassW(&wc)) {
        MessageBoxW(nullptr, L"Falló el registro de la clase de ventana Login.", L"Error", MB_OK | MB_ICONERROR);
        return;
    }

    HWND hwnd = CreateWindowExW(
        0, L"LoginWindowClass", L"Formulario de Login", WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 500, 300, nullptr, nullptr, hInstance, nullptr);

    if (hwnd) {
        ShowWindow(hwnd, SW_SHOW);
        UpdateWindow(hwnd);
    } else {
        MessageBoxW(nullptr, L"Falló la creación de la ventana Login.", L"Error", MB_OK | MB_ICONERROR);
    }
}

void MostrarMenuPrincipal(HINSTANCE hInstance) {
    WNDCLASSW wc = {};
    wc.lpfnWndProc = MainProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = L"MainWindowClass"; // Nombre de clase para la ventana principal

    if (!RegisterClassW(&wc)) {
        MessageBoxW(nullptr, L"Falló el registro de la clase de ventana Principal.", L"Error", MB_OK | MB_ICONERROR);
        return;
    }

    hVentanaPrincipal = CreateWindowExW(
        0, L"MainWindowClass", L"Gestor de Biblioteca", WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 800, 600, nullptr, nullptr, hInstance, nullptr);

    if (hVentanaPrincipal) {
        ShowWindow(hVentanaPrincipal, SW_SHOW);
        UpdateWindow(hVentanaPrincipal);
    } else {
        MessageBoxW(nullptr, L"Falló la creación de la ventana Principal.", L"Error", MB_OK | MB_ICONERROR);
    }
}

void MostrarVentanaAgregarLibro() {
    WNDCLASSW wc = {};
    wc.lpfnWndProc = AgregarLibroProc;
    wc.hInstance = hInst; // Usar la instancia guardada
    wc.lpszClassName = L"AgregarLibroWindowClass"; // Nombre de clase

    if (!RegisterClassW(&wc)) {
        MessageBoxW(nullptr, L"Falló el registro de la clase de ventana Agregar Libro.", L"Error", MB_OK | MB_ICONERROR);
        return;
    }

    HWND hwnd = CreateWindowExW(
        0, L"AgregarLibroWindowClass", L"Agregar Nuevo Libro", WS_OVERLAPPEDWINDOW | WS_CHILD,
        CW_USEDEFAULT, CW_USEDEFAULT, 500, 400, hVentanaPrincipal, nullptr, hInst, nullptr);

    if (hwnd) {
        ShowWindow(hwnd, SW_SHOW);
        UpdateWindow(hwnd);
    } else {
        MessageBoxW(nullptr, L"Falló la creación de la ventana Agregar Libro.", L"Error", MB_OK | MB_ICONERROR);
    }
}

void MostrarVentanaListarLibros() {
    WNDCLASSW wc = {};
    wc.lpfnWndProc = ListarLibrosProc;
    wc.hInstance = hInst; // Usar la instancia guardada
    wc.lpszClassName = L"ListarLibrosWindowClass"; // Nombre de clase

    if (!RegisterClassW(&wc)) {
        MessageBoxW(nullptr, L"Falló el registro de la clase de ventana Listar Libros.", L"Error", MB_OK | MB_ICONERROR);
        return;
    }

    HWND hwnd = CreateWindowExW(
        0, L"ListarLibrosWindowClass", L"Listado de Libros", WS_OVERLAPPEDWINDOW | WS_CHILD,
        CW_USEDEFAULT, CW_USEDEFAULT, 600, 500, hVentanaPrincipal, nullptr, hInst, nullptr);

    if (hwnd) {
        ShowWindow(hwnd, SW_SHOW);
        UpdateWindow(hwnd);
    } else {
        MessageBoxW(nullptr, L"Falló la creación de la ventana Listar Libros.", L"Error", MB_OK | MB_ICONERROR);
    }
}

// --- Implementaciones de los procedimientos de ventana (estarán en otros archivos .cpp) ---
LRESULT CALLBACK LoginProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    // Implementación del procedimiento de ventana para el formulario de login
    return DefWindowProcW(hwnd, uMsg, wParam, lParam);
}

LRESULT CALLBACK MainProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    // Implementación del procedimiento de ventana para la ventana principal (menú)
    return DefWindowProcW(hwnd, uMsg, wParam, lParam);
}

LRESULT CALLBACK AgregarLibroProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    // Implementación del procedimiento de ventana para la ventana Agregar Libro
    return DefWindowProcW(hwnd, uMsg, wParam, lParam);
}

LRESULT CALLBACK ListarLibrosProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    // Implementación del procedimiento de ventana para la ventana Listar Libros
    return DefWindowProcW(hwnd, uMsg, wParam, lParam);
}

void IntentarLogin(HWND hwnd) {
    // Esta función probablemente estará en otro archivo (.cpp) donde se maneja la lógica del botón de login
}