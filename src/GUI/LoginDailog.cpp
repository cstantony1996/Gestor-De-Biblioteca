#include "GUI/LoginDialog.h"
#include <QLineEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QCheckBox>

LoginDialog::LoginDialog(QWidget *parent)
    : QDialog(parent) {

    usernameInput = new QLineEdit(this);
    passwordInput = new QLineEdit(this);
    passwordInput->setEchoMode(QLineEdit::Password);

    rememberCheck = new QCheckBox("Recordar usuario", this);

    loginButton = new QPushButton("Iniciar sesión", this);
    cancelButton = new QPushButton("Cancelar", this);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(new QLabel("Usuario:"));
    layout->addWidget(usernameInput);
    layout->addWidget(new QLabel("Contraseña:"));
    layout->addWidget(passwordInput);
    layout->addWidget(rememberCheck);

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(loginButton);
    buttonLayout->addWidget(cancelButton);

    layout->addLayout(buttonLayout);

    connect(loginButton, &QPushButton::clicked, this, &QDialog::accept);
    connect(cancelButton, &QPushButton::clicked, this, &QDialog::reject);
    
}