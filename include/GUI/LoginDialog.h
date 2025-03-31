#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>
#include <QString>
#include <QLineEdit>
#include <QCheckBox>
#include <QPushButton>


class LoginDialog : public QDialog {

    Q_OBJECT

public:
    explicit LoginDialog(QWidget *parent = nullptr);

    QString getUsername() const;
    QString getPassword() const;
    bool rememberMe() const;

private:

    QLineEdit *usernameInput;
    QLineEdit *passwordInput;
    QCheckBox *rememberCheck;
    QPushButton *loginButton;
    QPushButton *cancelButton;

};

#endif // LOGINDIALOG_H