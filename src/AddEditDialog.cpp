#include "AddEditDialog.h"
#include <QFormLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QMessageBox>
#include <QRandomGenerator>

AddEditDialog::AddEditDialog(QWidget* parent) : QDialog(parent) {
    setWindowTitle("Entry");
    setFixedSize(400, 250);

    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    QFormLayout* formLayout = new QFormLayout();

    m_titleEdit = new QLineEdit(this);
    m_usernameEdit = new QLineEdit(this);
    m_passwordEdit = new QLineEdit(this);

    formLayout->addRow("Website/Title:", m_titleEdit);
    formLayout->addRow("Username:", m_usernameEdit);

    QHBoxLayout* pwdLayout = new QHBoxLayout();
    pwdLayout->addWidget(m_passwordEdit);
    QPushButton* generateBtn = new QPushButton("Generate", this);
    pwdLayout->addWidget(generateBtn);
    formLayout->addRow("Password:", pwdLayout);

    mainLayout->addLayout(formLayout);

    QHBoxLayout* btnLayout = new QHBoxLayout();
    QPushButton* saveBtn = new QPushButton("Save", this);
    QPushButton* cancelBtn = new QPushButton("Cancel", this);
    btnLayout->addStretch();
    btnLayout->addWidget(saveBtn);
    btnLayout->addWidget(cancelBtn);
    
    mainLayout->addLayout(btnLayout);

    connect(generateBtn, &QPushButton::clicked, this, &AddEditDialog::onGenerateClicked);
    connect(saveBtn, &QPushButton::clicked, this, &AddEditDialog::onSaveClicked);
    connect(cancelBtn, &QPushButton::clicked, this, &QDialog::reject);
}

void AddEditDialog::setEntry(const Entry& entry) {
    m_entry = entry;
    m_titleEdit->setText(entry.title);
    m_usernameEdit->setText(entry.username);
    m_passwordEdit->setText(entry.password);
}

Entry AddEditDialog::getEntry() const {
    Entry e = m_entry;
    e.title = m_titleEdit->text();
    e.username = m_usernameEdit->text();
    e.password = m_passwordEdit->text();
    return e;
}

void AddEditDialog::onGenerateClicked() {
    const QString possibleCharacters("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789!@#$%^&*()_+");
    const int passwordLength = 16;
    QString randomString;
    for(int i = 0; i < passwordLength; ++i) {
        int index = QRandomGenerator::global()->generate() % possibleCharacters.length();
        randomString.append(possibleCharacters.at(index));
    }
    m_passwordEdit->setText(randomString);
}

void AddEditDialog::onSaveClicked() {
    if (m_titleEdit->text().isEmpty()) {
        QMessageBox::warning(this, "Error", "Title cannot be empty.");
        return;
    }
    accept();
}
