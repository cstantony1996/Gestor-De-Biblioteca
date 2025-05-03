#ifndef VENTANASGUI_H
#define VENTANASGUI_H

#include <windows.h>
#include <libpq-fe.h>
#include <string>
#include <memory>

// Declaraciones adelantadas
class UserAuth;
class Libro;
class ListarLibros;

// Variables globales
extern HWND hUsuario, hContraseña, hBotonLogin, hVentanaPrincipal;
extern HWND hTitulo, hAutor, hIsbn, hEditorial, hMateria, hAñoPublicacion, hGuardarLibro, hListarLibros;
extern PGconn* conn;
extern std::unique_ptr<UserAuth> auth;
extern std::unique_ptr<Libro> libro;
extern std::unique_ptr<ListarLibros> listarLibros;
extern HINSTANCE hInst;

// Prototipos de funciones
void IntentarLogin(HWND hwnd);
void MostrarLogin(HINSTANCE hInstance);
void MostrarMenuPrincipal(HINSTANCE hInstance);
void MostrarVentanaAgregarLibro();
void MostrarVentanaListarLibros();

// Procedimientos de ventana
LRESULT CALLBACK LoginProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK MainProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK AgregarLibroProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK ListarLibrosProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

#endif