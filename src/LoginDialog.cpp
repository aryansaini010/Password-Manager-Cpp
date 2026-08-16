#include "LoginDialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>

LoginDialog::LoginDialog(StorageManager* storage, QWidget* parent)
    : QDialog(parent), m_storage(storage)
{
    m_isNewVault = !m_storage->vaultExists();
    
    setWindowTitle(m_isNewVault ? "Create Master Password" : "Login to Vault");
    setFixedSize(350, 200);

    QVBoxLayout* layout = new QVBoxLayout(this);

    QLabel* instruction = new QLabel(m_isNewVault ? 
        "Welcome! Create a master password for your new vault." : 
        "Welcome back! Enter your master password.");
    instruction->setWordWrap(true);
    layout->addWidget(instruction);

    m_passwordEdit = new QLineEdit(this);
    m_passwordEdit->setEchoMode(QLineEdit::Password);
    m_passwordEdit->setPlaceholderText("Master Password");
    layout->addWidget(m_passwordEdit);

    if (m_isNewVault) {
        m_confirmEdit = new QLineEdit(this);
        m_confirmEdit->setEchoMode(QLineEdit::Password);
        m_confirmEdit->setPlaceholderText("Confirm Master Password");
        layout->addWidget(m_confirmEdit);
    } else {
        m_confirmEdit = nullptr;
    }

    m_statusLabel = new QLabel(this);
    m_statusLabel->setStyleSheet("color: red;");
    layout->addWidget(m_statusLabel);

    m_loginBtn = new QPushButton(m_isNewVault ? "Create Vault" : "Unlock", this);
    layout->addWidget(m_loginBtn, 0, Qt::AlignRight);

    connect(m_loginBtn, &QPushButton::clicked, this, &LoginDialog::onLoginClicked);
    connect(m_passwordEdit, &QLineEdit::returnPressed, this, &LoginDialog::onLoginClicked);
    if (m_confirmEdit) {
        connect(m_confirmEdit, &QLineEdit::returnPressed, this, &LoginDialog::onLoginClicked);
    }
}

bool LoginDialog::isNewVault() const {
    return m_isNewVault;
}

void LoginDialog::onLoginClicked() {
    QString pwd = m_passwordEdit->text();
    if (pwd.isEmpty()) {
        m_statusLabel->setText("Password cannot be empty.");
        return;
    }

    if (m_isNewVault) {
        if (pwd != m_confirmEdit->text()) {
            m_statusLabel->setText("Passwords do not match.");
            return;
        }
        if (m_storage->setupVault(pwd)) {
            accept();
        } else {
            m_statusLabel->setText("Failed to create vault.");
        }
    } else {
        if (m_storage->unlockVault(pwd)) {
            accept();
        } else {
            m_statusLabel->setText("Incorrect password or corrupted vault.");
            m_passwordEdit->clear();
        }
    }
}
