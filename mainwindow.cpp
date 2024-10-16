#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "settingsdialog.h"

#include <QShortcut>
#include <QIcon>
#include <QLabel>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    loadSettings();

    // hotkeys
    QShortcut *newFileShortcut = new QShortcut(QKeySequence("Ctrl+N"), this);
    connect(newFileShortcut, &QShortcut::activated, this, &MainWindow::on_actionNew_triggered);

    QShortcut *openShortcut = new QShortcut(QKeySequence("Ctrl+O"), this);
    connect(openShortcut, &QShortcut::activated, this, &MainWindow::on_actionOpen_triggered);

    QShortcut *saveShortcut = new QShortcut(QKeySequence("Ctrl+S"), this);
    connect(saveShortcut, &QShortcut::activated, this, &MainWindow::on_actionSave_triggered);

    QShortcut *saveAsShortcut = new QShortcut(QKeySequence("Ctrl+Shift+S"), this);
    connect(saveAsShortcut, &QShortcut::activated, this, &MainWindow::on_actionSave_as_triggered);


    this->setWindowTitle("Text Editor");
    this->setWindowIcon(QIcon(":/image/resources/images/text-file-icon.png"));

    this->setCentralWidget(ui->plainTextEdit);

    statusBar()->showMessage("Ready");
}

MainWindow::~MainWindow()
{
    delete ui;
    delete settingsDialog;
}


void MainWindow::setFileContent(const QString &content) {
    ui->plainTextEdit->setPlainText(content);
}

void MainWindow::setCurrentFile(const QString &fileName) {
    currentFile = fileName;
}

void MainWindow::on_plainTextEdit_textChanged()
{
    QString currentText = ui->plainTextEdit->toPlainText();
    QString title = currentFile.isEmpty() ? "Text Editor" : QFileInfo(currentFile).fileName();

    if (currentText != originalText) {
        if (!title.startsWith("*")) {
            title.prepend("*");
        }
    }
    else if (currentText == originalText) {
        if (title.startsWith("*")) {
            title.remove(0, 1);
        }
    }

    setWindowTitle(title);
}


// File
void MainWindow::on_actionNew_triggered()
{
    ui->plainTextEdit->clear();
    currentFile.clear();
    originalText.clear();
    this->setWindowTitle("Text Editor");
    statusBar()->showMessage("New file");
}

void MainWindow::on_actionOpen_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Open file");
    if (fileName.isEmpty())
        return;

    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::critical(this, "Error", "Failed to open file: " + file.errorString());
        return;
    }

    QTextStream in(&file);
    QString content = in.readAll();
    ui->plainTextEdit->setPlainText(content);
    originalText = content;
    file.close();

    currentFile = fileName;

    setWindowTitle(QFileInfo(fileName).fileName());

    statusBar()->showMessage("Opened " + fileName);
}

void MainWindow::on_actionSave_triggered()
{
    if (currentFile.isEmpty()) {
        on_actionSave_as_triggered();
        return;
    }

    QFile file(currentFile);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::critical(this, "Error", "Failed to save file: " + file.errorString());
        return;
    }

    QTextStream out(&file);
    QString content = ui->plainTextEdit->toPlainText();
    out << content;
    originalText = content;
    file.close();

    setWindowTitle(QFileInfo(currentFile).fileName());

    statusBar()->showMessage("Saved " + currentFile);
}

void MainWindow::on_actionSave_as_triggered()
{
    QString fileName = QFileDialog::getSaveFileName(this, "Save file as", "", "Text Files (*.txt)");
    if (fileName.isEmpty())
        return;

    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::critical(this, "Error", "Failed to save file: " + file.errorString());
        return;
    }

    QTextStream out(&file);
    QString content = ui->plainTextEdit->toPlainText();
    out << content;
    originalText = content;
    file.close();

    currentFile = fileName;

    setWindowTitle(QFileInfo(currentFile).fileName());

    statusBar()->showMessage("Saved " + fileName);
}


// Edit
void MainWindow::on_actionCut_triggered()
{
    ui->plainTextEdit->cut();
}

void MainWindow::on_actionCopy_triggered()
{
    ui->plainTextEdit->copy();
}

void MainWindow::on_actionPaste_triggered()
{
    ui->plainTextEdit->paste();
}

void MainWindow::on_actionSelect_All_triggered()
{
    ui->plainTextEdit->selectAll();
}

void MainWindow::on_actionSettings_triggered()
{
    SettingsDialog settingsDialog(this);

    settingsDialog.setTextSize(ui->plainTextEdit->font().pointSize());
    settingsDialog.setTheme(currentTheme);

    if (settingsDialog.exec() == QDialog::Accepted) {
        int textSize = settingsDialog.getTextSize();
        QString theme = settingsDialog.getTheme();

        applySettings(textSize, theme);
        saveSettings();
    }
}


// Settings
void MainWindow::loadSettings()
{
    QSettings settings("MyCompany", "MyApp");

    QString theme = settings.value("theme", "Light").toString();
    int textSize = settings.value("fontSize", 9).toInt();

    applySettings(textSize, theme);
}

void MainWindow::saveSettings()
{
    QSettings settings("MyCompany", "MyApp");

    settings.setValue("theme", currentTheme);

    int fontSize = ui->plainTextEdit->font().pointSize();
    settings.setValue("fontSize", fontSize);

    qDebug() << "Settings saved. Theme:" << currentTheme << "Font Size:" << fontSize;
}

void MainWindow::applySettings(int textSize, const QString &theme)
{
    applyTheme(theme);

    QString fontStyle = QString("font-size: %1pt;").arg(textSize);
    ui->plainTextEdit->setStyleSheet(fontStyle);
    ui->plainTextEdit->update();
}

void MainWindow::applyTheme(const QString &theme)
{
    if (theme == "Dark") {
        qApp->setStyleSheet(
            "QPlainTextEdit {background-color: #2e2e2e; color: white; border-color: gray; border: 2px;}"
            "QMenuBar {background-color: #3e3e3e; color: white;}"
            "QMenuBar::item:selected {background-color: #2e2e2e; color: white;}"
            "QStatusBar {background-color: #3e3e3e; color: white;}"
            "QMenu {background-color: #2e2e2e; color: white;}"
            "QMenu::item:selected {background-color: #3e3e3e;}"
            "QScrollBar:vertical {border: 1px solid #3e3e3e; background: #2e2e2e; width: 12px;}"
            "QScrollBar:horizontal {border: 1px solid #3e3e3e; background: #2e2e2e; height: 12px;}"
            "QScrollBar::handle:vertical {background: #4e4e4e; min-height: 20px;}"
            "QScrollBar::handle:horizontal {background: #4e4e4e; min-width: 20px;}"
            "QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical {background: #2e2e2e; border: 1px solid #3e3e3e;}"
            "QScrollBar::add-line:horizontal, QScrollBar::sub-line:horizontal {background: #2e2e2e; border: 1px solid #3e3e3e;}"
            "QScrollBar::up-arrow:vertical, QScrollBar::down-arrow:vertical, QScrollBar::left-arrow:horizontal, QScrollBar::right-arrow:horizontal {background: #2e2e2e;}"
            "QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical, QScrollBar::add-page:horizontal, QScrollBar::sub-page:horizontal {background: #2e2e2e;}"
            "QDialog { background-color: #2e2e2e; color: white; }"

            "QLabel { color: white; }"
            "QSpinBox, QComboBox { background-color: #3e3e3e; color: white; border: 1px solid #555; }"
            "QDialogButtonBox { background-color: #2e2e2e; }"
            "QPushButton { background-color: #4e4e4e; color: white; border: 1px solid #555; }"
            "QPushButton:hover { background-color: #5e5e5e; }"
            );
        currentTheme = "Dark";
    } else {
        qApp->setStyleSheet("QPlainTextEdit { background-color: white; border-color: gray; border: 2px;} "
            "QMenuBar { background-color: #f0f0f0; color: black;} "
            "QMenuBar::item:selected { background-color: #e0e0e0; color: black; } "
            "QStatusBar { background-color: #f0f0f0;} "
            "QMenu { background-color: white; color: black; } "
            "QMenu::item:selected { background-color: #e0e0e0; }"
            "QScrollBar:vertical {border: 1px solid #c0c0c0; background: #f0f0f0; width: 12px;}"
            "QScrollBar:horizontal {border: 1px solid #c0c0c0; background: #f0f0f0; height: 12px;}"
            "QScrollBar::handle:vertical {background: #c0c0c0; min-height: 20px;}"
            "QScrollBar::handle:horizontal {background: #c0c0c0; min-width: 20px;}"
            "QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical {background: #f0f0f0; border: 1px solid #c0c0c0;}"
            "QScrollBar::add-line:horizontal, QScrollBar::sub-line:horizontal {background: #f0f0f0; border: 1px solid #c0c0c0;}"
            "QScrollBar::up-arrow:vertical, QScrollBar::down-arrow:vertical, QScrollBar::left-arrow:horizontal, QScrollBar::right-arrow:horizontal {background: #f0f0f0;}"
            "QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical, QScrollBar::add-page:horizontal, QScrollBar::sub-page:horizontal {background: #f0f0f0;}"

            "QDialog { background-color: white; color: black; }"
            "QLabel { color: black; }"
            "QSpinBox, QComboBox { background-color: white; color: black; border: 1px solid #ccc; }"
            "QDialogButtonBox { background-color: white; }"
            "QPushButton { background-color: #e0e0e0; color: black; border: 1px solid #ccc; }"
            "QPushButton:hover { background-color: #d0d0d0; }"
            );
        currentTheme = "Light";
    }
}


// Help
void MainWindow::on_actionAbout_the_program_triggered()
{
     QDialog aboutDialog(this);
    aboutDialog.setWindowTitle("About the program");
    aboutDialog.setFixedSize(400, 300);

    QLabel *label = new QLabel(&aboutDialog);
    label->setTextFormat(Qt::RichText);
    label->setText(
        "<h2>Text Editor Application</h2>"
        "<p><b>1) This is a simple text editor designed for basic text editing tasks, "
        "allowing you to create, open, and save text files, with support for customizable themes "
        "and font sizes.</b></p>"
        "<p><b>2) Built using Qt and C++.</b></p>"
        "<p><b>3) Hotkeys:</b></p>"
        "<ul>"
        "<li><b>Control+N:</b> New File</li>"
        "<li><b>Control+O:</b> Open File</li>"
        "<li><b>Control+S:</b> Save File</li>"
        "<li><b>Control+Shift+S:</b> Save As</li>"
        "<li><b>Control+Q:</b> Quit</li>"
        "</ul>"
        "<p><b>4) Developed by <a href=\"https://github.com/Igor-Ledok\">Igor Ledok</a>.</b></p>"
    );
    label->setOpenExternalLinks(true);
    label->setWordWrap(true);

    QVBoxLayout layout;
    layout.addWidget(label);
    aboutDialog.setLayout(&layout);

    aboutDialog.exec();
}
