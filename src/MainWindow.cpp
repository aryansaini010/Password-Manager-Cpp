#include "MainWindow.h"
#include "AddEditDialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QMessageBox>
#include <QApplication>
#include <QClipboard>

MainWindow::MainWindow(StorageManager* storage, QWidget *parent)
    : QMainWindow(parent), m_storage(storage), m_passwordsVisible(false)
{
    setupUi();
    loadData();
}

MainWindow::~MainWindow() {}

void MainWindow::setupUi() {
    setWindowTitle("Password Manager");
    resize(800, 600);

    QWidget* centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    QVBoxLayout* mainLayout = new QVBoxLayout(centralWidget);

    QHBoxLayout* topLayout = new QHBoxLayout();
    QPushButton* addBtn = new QPushButton("Add Entry", this);
    m_editBtn = new QPushButton("Edit", this);
    m_deleteBtn = new QPushButton("Delete", this);
    m_copyBtn = new QPushButton("Copy Password", this);
    m_toggleVisibilityBtn = new QPushButton("Show Passwords", this);
    
    topLayout->addWidget(addBtn);
    topLayout->addWidget(m_editBtn);
    topLayout->addWidget(m_deleteBtn);
    topLayout->addWidget(m_copyBtn);
    topLayout->addStretch();
    topLayout->addWidget(m_toggleVisibilityBtn);
    
    mainLayout->addLayout(topLayout);

    m_table = new QTableWidget(0, 4, this);
    m_table->setHorizontalHeaderLabels({"ID", "Website", "Username", "Password"});
    m_table->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_table->setSelectionMode(QAbstractItemView::SingleSelection);
    m_table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    m_table->hideColumn(0); // Hide ID column
    mainLayout->addWidget(m_table);

    connect(addBtn, &QPushButton::clicked, this, &MainWindow::onAddClicked);
    connect(m_editBtn, &QPushButton::clicked, this, &MainWindow::onEditClicked);
    connect(m_deleteBtn, &QPushButton::clicked, this, &MainWindow::onDeleteClicked);
    connect(m_copyBtn, &QPushButton::clicked, this, &MainWindow::onCopyClicked);
    connect(m_toggleVisibilityBtn, &QPushButton::clicked, this, &MainWindow::onToggleVisibilityClicked);
    connect(m_table, &QTableWidget::itemSelectionChanged, this, &MainWindow::onTableSelectionChanged);

    updateButtons();
}

void MainWindow::loadData() {
    m_table->setRowCount(0);
    QList<Entry> entries = m_storage->getEntries();
    
    for (const Entry& e : entries) {
        int row = m_table->rowCount();
        m_table->insertRow(row);
        
        m_table->setItem(row, 0, new QTableWidgetItem(e.id));
        m_table->setItem(row, 1, new QTableWidgetItem(e.title));
        m_table->setItem(row, 2, new QTableWidgetItem(e.username));
        
        QString displayPwd = m_passwordsVisible ? e.password : "********";
        QTableWidgetItem* pwdItem = new QTableWidgetItem(displayPwd);
        pwdItem->setData(Qt::UserRole, e.password); // Store actual password in user data
        m_table->setItem(row, 3, pwdItem);
    }
}

void MainWindow::updateButtons() {
    bool hasSelection = m_table->currentRow() >= 0;
    m_editBtn->setEnabled(hasSelection);
    m_deleteBtn->setEnabled(hasSelection);
    m_copyBtn->setEnabled(hasSelection);
}

void MainWindow::onTableSelectionChanged() {
    updateButtons();
}

void MainWindow::onAddClicked() {
    AddEditDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        m_storage->addEntry(dialog.getEntry());
        loadData();
    }
}

void MainWindow::onEditClicked() {
    int row = m_table->currentRow();
    if (row < 0) return;

    QString id = m_table->item(row, 0)->text();
    QList<Entry> entries = m_storage->getEntries();
    Entry entryToEdit;
    for (const Entry& e : entries) {
        if (e.id == id) {
            entryToEdit = e;
            break;
        }
    }

    AddEditDialog dialog(this);
    dialog.setEntry(entryToEdit);
    if (dialog.exec() == QDialog::Accepted) {
        m_storage->updateEntry(dialog.getEntry());
        loadData();
    }
}

void MainWindow::onDeleteClicked() {
    int row = m_table->currentRow();
    if (row < 0) return;

    if (QMessageBox::question(this, "Delete", "Are you sure you want to delete this entry?") == QMessageBox::Yes) {
        QString id = m_table->item(row, 0)->text();
        m_storage->deleteEntry(id);
        loadData();
    }
}

void MainWindow::onCopyClicked() {
    int row = m_table->currentRow();
    if (row < 0) return;

    QString pwd = m_table->item(row, 3)->data(Qt::UserRole).toString();
    QApplication::clipboard()->setText(pwd);
}

void MainWindow::onToggleVisibilityClicked() {
    m_passwordsVisible = !m_passwordsVisible;
    m_toggleVisibilityBtn->setText(m_passwordsVisible ? "Hide Passwords" : "Show Passwords");
    
    for (int i = 0; i < m_table->rowCount(); ++i) {
        QString pwd = m_table->item(i, 3)->data(Qt::UserRole).toString();
        m_table->item(i, 3)->setText(m_passwordsVisible ? pwd : "********");
    }
}
