#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    if (argc > 1) {
        QString filePath = argv[1];
        QFile file(filePath);

        if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream in(&file);
            w.setFileContent(in.readAll());
            w.setCurrentFile(filePath);
            file.close();
        } else {
            QMessageBox::warning(&w, "Error", "Failed to open file passed as argument.");
        }
    }

    w.show();
    return a.exec();
}
