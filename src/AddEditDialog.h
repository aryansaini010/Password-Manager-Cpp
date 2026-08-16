#ifndef ADDEDITDIALOG_H
#define ADDEDITDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include "Entry.h"

class AddEditDialog : public QDialog {
    Q_OBJECT
public:
    explicit AddEditDialog(QWidget* parent = nullptr);
    void setEntry(const Entry& entry);
    Entry getEntry() const;

private slots:
    void onGenerateClicked();
    void onSaveClicked();

private:
    Entry m_entry;
    QLineEdit* m_titleEdit;
    QLineEdit* m_usernameEdit;
    QLineEdit* m_passwordEdit;
};

#endif
