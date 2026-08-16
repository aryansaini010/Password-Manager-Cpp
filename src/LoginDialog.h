#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include "StorageManager.h"

class LoginDialog : public QDialog {
    Q_OBJECT
public:
    explicit LoginDialog(StorageManager* storage, QWidget* parent = nullptr);
    bool isNewVault() const;

private slots:
    void onLoginClicked();

private:
    StorageManager* m_storage;
    bool m_isNewVault;

    QLineEdit* m_passwordEdit;
    QLineEdit* m_confirmEdit;
    QLabel* m_statusLabel;
    QPushButton* m_loginBtn;
};

#endif
