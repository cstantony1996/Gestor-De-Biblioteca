#include <windows.h>
#include <tchar.h>
#include <string>
#include "Connection.h" // Incluir el archivo de conexión
#include "UserAuth.h"

using namespace std;

// Declaración de la función de ventana
LRESULT CALLBACK WindowProcedure(HWND, UINT, WPARAM, LPARAM);

// Crear instancia de UserAuth
UserAuth* userAuth = nullptr;

// Función para crear el formulario de login
HWND hEditUser, hEditPass, hButtonLogin, hButtonClear;

// La entrada principal
int main()
{
    // Conectar a la base de datos usando la función conectarDB()
    PGconn* conn = conectarDB();

    if (conn == nullptr) {
        MessageBox(NULL, _T("No se pudo conectar a la base de datos."), _T("Error"), MB_OK | MB_ICONERROR);
        return 0;
    }

    // Crear la instancia de UserAuth con la conexión a la base de datos
    userAuth = new UserAuth(conn);

    // Definir una clase de ventana
    WNDCLASS wndClass = {};
    wndClass.lpfnWndProc = WindowProcedure;
    wndClass.hInstance = GetModuleHandle(NULL);
    wndClass.lpszClassName = _T("LoginWindowClass");

    // Registrar la clase de la ventana
    if (!RegisterClass(&wndClass))
    {
        PQfinish(conn);  // Cerrar la conexión antes de salir si la ventana no se registra
        return 0;
    }

    // Crear la ventana
    HWND hwnd = CreateWindowEx(
        0, _T("LoginWindowClass"), _T("Formulario de Login"), WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 500, 300, NULL, NULL, NULL, NULL);

    // Verificar si la ventana fue creada correctamente
    if (!hwnd)
    {
        PQfinish(conn);  // Cerrar la conexión antes de salir si la ventana no se crea
        return 0;
    }

    // Crear los controles del formulario
    CreateWindow(_T("STATIC"), _T("Usuario:"), WS_VISIBLE | WS_CHILD, 50, 50, 100, 20, hwnd, NULL, NULL, NULL);
    hEditUser = CreateWindow(_T("EDIT"), _T(""), WS_VISIBLE | WS_CHILD | WS_BORDER, 150, 50, 200, 20, hwnd, NULL, NULL, NULL);

    CreateWindow(_T("STATIC"), _T("Contraseña:"), WS_VISIBLE | WS_CHILD, 50, 100, 100, 20, hwnd, NULL, NULL, NULL);
    hEditPass = CreateWindow(_T("EDIT"), _T(""), WS_VISIBLE | WS_CHILD | WS_BORDER | ES_PASSWORD, 150, 100, 200, 20, hwnd, NULL, NULL, NULL);

    hButtonLogin = CreateWindow(_T("BUTTON"), _T("Iniciar sesión"), WS_VISIBLE | WS_CHILD, 150, 150, 100, 30, hwnd, (HMENU)1, NULL, NULL);
    hButtonClear = CreateWindow(_T("BUTTON"), _T("Limpiar"), WS_VISIBLE | WS_CHILD, 260, 150, 100, 30, hwnd, (HMENU)2, NULL, NULL);

    // Mostrar la ventana
    ShowWindow(hwnd, SW_SHOW);
    UpdateWindow(hwnd);

    // Bucle de mensajes
    MSG msg = {};
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    // Limpiar y cerrar la conexión a la base de datos
    delete userAuth;
    PQfinish(conn);

    return 0;
}

// Procedimiento de la ventana para manejar mensajes
LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
{
    switch (msg)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;

    case WM_COMMAND:
        if (LOWORD(wp) == 1)
        { // Si el botón "Iniciar sesión" es presionado
            int lenUser = GetWindowTextLength(hEditUser) + 1;
            int lenPass = GetWindowTextLength(hEditPass) + 1;

            TCHAR* username = new TCHAR[lenUser];
            TCHAR* password = new TCHAR[lenPass];

            GetWindowText(hEditUser, username, lenUser);
            GetWindowText(hEditPass, password, lenPass);

            // Convertir TCHAR a std::string para pasar a UserAuth
            string userStr = string(username);
            string passStr = string(password);

            // Llamar a la función login de UserAuth
            if (userAuth->login(userStr, passStr))
            {
                MessageBox(hwnd, _T("Login exitoso!"), _T("Exito"), MB_OK | MB_ICONINFORMATION);
            }
            else
            {
                MessageBox(hwnd, _T("Credenciales inválidas"), _T("Error"), MB_OK | MB_ICONERROR);
            }

            // Limpiar la memoria
            delete[] username;
            delete[] password;
        }
        else if (LOWORD(wp) == 2)
        { // Botón "Limpiar"
            SetWindowText(hEditUser, _T(""));
            SetWindowText(hEditPass, _T(""));
        }
        return 0;

    default:
        return DefWindowProc(hwnd, msg, wp, lp);
    }
}
