#include <windows.h>
#include <commctrl.h>
#include "Listar_libros.h"
#include <iostream>
#include <string>
#include <vector>
#include <libpq-fe.h>
#include <algorithm> // Añadir para usar remove_if

#pragma comment(lib, "comctl32.lib")

// Definir el procedimiento de la ventana
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

// Convertir UTF-8 a ANSI
std::string utf8ToAnsi(const std::string &utf8Str) {
    int wlen = MultiByteToWideChar(CP_UTF8, 0, utf8Str.c_str(), -1, NULL, 0);
    std::wstring wstr(wlen, 0);
    MultiByteToWideChar(CP_UTF8, 0, utf8Str.c_str(), -1, &wstr[0], wlen);

    int alen = WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), -1, NULL, 0, NULL, NULL);
    std::string ansiStr(alen, 0);
    WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), -1, &ansiStr[0], alen, NULL, NULL);

    return ansiStr;
}

// Limpiar y ajustar el texto
std::string limpiarAjustar(const char* texto, int ancho) {
    std::string str(texto);
    str.erase(remove_if(str.begin(), str.end(), [](unsigned char c) {
        return (c < 32 && c != 9);
    }), str.end());

    if (str.length() > static_cast<size_t>(ancho)) {
        return str.substr(0, ancho - 3) + "...";
    }
    return str;
}

// Mostrar los libros en un ListView
void mostrarLibros(HWND hwndListView, PGconn *conn) {
    try {
        if (!conn)
            throw std::runtime_error("No hay conexión a la base de datos.");

        const char *query = R"(
            SELECT 
                titulo,
                autor,
                isbn,
                estado
            FROM libros
            ORDER BY titulo
        )";

        PGresult *res = PQexec(conn, query);

        if (PQresultStatus(res) != PGRES_TUPLES_OK) {
            PQclear(res);
            throw std::runtime_error("Error al ejecutar la consulta: " + std::string(PQerrorMessage(conn)));
        }

        int numRows = PQntuples(res);
        for (int i = 0; i < numRows; ++i) {
            LVITEM lvItem = {0};
            lvItem.mask = LVIF_TEXT;
            lvItem.iItem = i;
            lvItem.iSubItem = 0;

            std::string titulo = limpiarAjustar(utf8ToAnsi(PQgetvalue(res, i, 0)).c_str(), 40);
            lvItem.pszText = const_cast<LPSTR>(titulo.c_str());

            int idx = ListView_InsertItem(hwndListView, &lvItem);

            std::string autor = limpiarAjustar(utf8ToAnsi(PQgetvalue(res, i, 1)).c_str(), 35);
            std::string isbn = limpiarAjustar(utf8ToAnsi(PQgetvalue(res, i, 2)).c_str(), 30);
            std::string estado = limpiarAjustar(utf8ToAnsi(PQgetvalue(res, i, 3)).c_str(), 20);

            ListView_SetItemText(hwndListView, idx, 1, const_cast<LPSTR>(autor.c_str()));
            ListView_SetItemText(hwndListView, idx, 2, const_cast<LPSTR>(isbn.c_str()));
            ListView_SetItemText(hwndListView, idx, 3, const_cast<LPSTR>(estado.c_str()));
        }

        PQclear(res);
    } catch (const std::exception &e) {
        MessageBox(hwndListView, e.what(), "Error", MB_OK | MB_ICONERROR);
    }
}

// Función principal
int main() {
    PGconn *conn = PQconnectdb("user=postgres dbname=postgres password=Myroot");
    if (PQstatus(conn) != CONNECTION_OK) {
        MessageBox(NULL, PQerrorMessage(conn), "Error de conexión", MB_OK | MB_ICONERROR);
        PQfinish(conn);
        return 1;
    }

    const char CLASS_NAME[] = "VentanaListarLibros";
    WNDCLASS wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = GetModuleHandle(NULL);
    wc.lpszClassName = CLASS_NAME;
    RegisterClass(&wc);

    HWND hwnd = CreateWindowEx(
        0, CLASS_NAME, "Listado de Libros",
        WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 800, 600, NULL, NULL, wc.hInstance, NULL);

    if (hwnd == NULL) {
        MessageBox(NULL, "Error al crear la ventana", "Error", MB_OK | MB_ICONERROR);
        return 0;
    }

    HWND hwndListView = CreateWindowEx(
        0, WC_LISTVIEW, "", WS_CHILD | WS_VISIBLE | LVS_REPORT | LVS_SINGLESEL,
        10, 10, 760, 480, hwnd, NULL, wc.hInstance, NULL);

    LVCOLUMN lvColumn = {0};
    lvColumn.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM;
    lvColumn.pszText = const_cast<LPSTR>("Titulo");
    lvColumn.cx = 250;
    ListView_InsertColumn(hwndListView, 0, &lvColumn);

    lvColumn.pszText = const_cast<LPSTR>("Autor");
    lvColumn.cx = 250;
    ListView_InsertColumn(hwndListView, 1, &lvColumn);

    lvColumn.pszText = const_cast<LPSTR>("ISBN");
    lvColumn.cx = 150;
    ListView_InsertColumn(hwndListView, 2, &lvColumn);

    lvColumn.pszText = const_cast<LPSTR>("Estado");
    lvColumn.cx = 100;
    ListView_InsertColumn(hwndListView, 3, &lvColumn);

    mostrarLibros(hwndListView, conn);

    ShowWindow(hwnd, SW_SHOWNORMAL);
    UpdateWindow(hwnd);

    MSG msg = {};
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    PQfinish(conn);
    return 0;
}
