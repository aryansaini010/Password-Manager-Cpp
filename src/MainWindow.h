#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidget>
#include <QPushButton>
#include "StorageManager.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(StorageManager* storage, QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onAddClicked();
    void onEditClicked();
    void onDeleteClicked();
    void onCopyClicked();
    void onToggleVisibilityClicked();
    void onTableSelectionChanged();

private:
    StorageManager* m_storage;
    QTableWidget* m_table;
    QPushButton* m_editBtn;
    QPushButton* m_deleteBtn;
    QPushButton* m_copyBtn;
    QPushButton* m_toggleVisibilityBtn;
    
    bool m_passwordsVisible;
    
    void setupUi();
    void loadData();
    void updateButtons();
};

#endif
