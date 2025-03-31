#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableView>

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    
private slots:
    void showBookManager();
    void refreshBookList();
    
private:
    void setupUI();
    QTableView *bookTable;
};
#endif