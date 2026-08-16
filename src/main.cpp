#include <QApplication>
#include "MainWindow.h"
#include "LoginDialog.h"
#include "StorageManager.h"
#include <QStandardPaths>
#include <QDir>
#include <QMessageBox>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    
    // Apply a modern, sleek "Web-like" Dark Theme using Qt Style Sheets (QSS)
    a.setStyleSheet(R"(
        QWidget {
            background-color: #121212;
            color: #e0e0e0;
            font-family: 'Segoe UI', Helvetica, Arial, sans-serif;
            font-size: 14px;
        }
        QPushButton {
            background-color: #1a73e8; /* Google Blue */
            color: white;
            border-radius: 6px;
            padding: 8px 16px;
            border: none;
            font-weight: 600;
        }
        QPushButton:hover {
            background-color: #1557b0;
        }
        QPushButton:disabled {
            background-color: #333333;
            color: #777777;
        }
        QLineEdit {
            background-color: #1e1e1e;
            border: 1px solid #444;
            border-radius: 6px;
            padding: 8px;
            color: white;
        }
        QLineEdit:focus {
            border: 1px solid #1a73e8;
        }
        QTableWidget {
            background-color: #121212;
            alternate-background-color: #1a1a1a;
            gridline-color: #333;
            border: 1px solid #333;
            border-radius: 6px;
        }
        QHeaderView::section {
            background-color: #1e1e1e;
            color: #a0a0a0;
            padding: 8px;
            border: none;
            border-bottom: 1px solid #333;
            font-weight: bold;
        }
    )");
    
    // Create data directory if it doesn't exist
    QString dataDir = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QDir dir(dataDir);
    if (!dir.exists()) {
        dir.mkpath(".");
    }
    
    QString vaultPath = dir.filePath("vault.dat");
    StorageManager storage(vaultPath);
    
    LoginDialog loginDialog(&storage);
    if (loginDialog.exec() != QDialog::Accepted) {
        return 0; // User cancelled login or creation
    }

    MainWindow w(&storage);
    w.show();

    return a.exec();
}
