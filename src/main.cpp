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
