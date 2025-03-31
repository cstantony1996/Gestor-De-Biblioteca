#include "GUI/MainWindow.h"
#include "GUI/BookManager.h"
#include <QMenuBar>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent) {
    setWindowTitle("Gestor de Biblioteca");
    resize(1024, 768);
    setupUI();
}

void MainWindow::setupUI() {
    bookTable = new QTableView(this);
    setCentralWidget(bookTable);

    QMenu *booksMenu = menuBar()->addMenu("&Libros");
    booksMenu->addAction("Gestionar Libros", this, &MainWindow::showBookManager);
    booksMenu->addAction("Actualizar lista", this, &MainWindow::refreshBookList);
}