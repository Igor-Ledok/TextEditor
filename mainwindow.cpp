#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionNew_triggered() {
    ui->textEdit->clear();

    currentFile = "";
};

void MainWindow::on_actionOpen_triggered() {
    QString fileName = QFileDialog::getOpenFileName(this, "Open file");
    if (fileName.isEmpty())
        return;

    QFile file (fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        QMessageBox::warning(this,"Error","Failed to open file");
        return;
    }

    QTextStream in(&file);
    QString text = in.readAll();
    file.close();

    ui->textEdit->setText(text);

    currentFile = fileName;
};

void MainWindow::on_actionSave_triggered() {
    if (currentFile.isEmpty()) {
        on_actionSave_as_triggered();
        return;
    }

    QFile file(currentFile);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Error", "Failed to save file.");
        return;
    }

    QTextStream out(&file);
    out << ui->textEdit->toPlainText();
    file.close();
}


void MainWindow::on_actionSave_as_triggered() {
    QString fileName = QFileDialog::getSaveFileName(this, "Save file as", "", "(*)");

    if (fileName.isEmpty()) {
        return;
    }

    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Error", "Failed to save file.");
        return;
    }

    QTextStream out(&file);
    out << ui->textEdit->toPlainText();
    file.close();

    currentFile = fileName;
}




void MainWindow::on_actionCut_triggered() {
    ui->textEdit->cut();
}

void MainWindow::on_actionCopy_triggered() {
    ui->textEdit->copy();
}

void MainWindow::on_actionPaste_triggered() {
    ui->textEdit->paste();
}



void MainWindow::on_actionAbout_the_program_triggered() {
    QMessageBox::about(this, "About the program", "A text editor created with Qt, using C++.");
}



