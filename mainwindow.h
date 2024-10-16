#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFile>
#include <QFileDialog>
#include <QTextStream>
#include <QMessageBox>
#include <QPlainTextEdit>
#include <QStatusBar>
#include "settingsdialog.h"
#include <QSettings>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void setFileContent(const QString &content);
    void setCurrentFile(const QString &fileName);
private slots:
    // File
    void on_actionNew_triggered();
    void on_actionOpen_triggered();
    void on_actionSave_triggered();
    void on_actionSave_as_triggered();
    // Edit
    void on_actionCut_triggered();
    void on_actionCopy_triggered();
    void on_actionPaste_triggered();
    void on_actionSelect_All_triggered();
    void on_actionSettings_triggered();
    // Help
    void on_actionAbout_the_program_triggered();
    // Slot for text changes
    void on_plainTextEdit_textChanged();
private:
    Ui::MainWindow *ui;
    QString currentFile;
    QString currentTheme;
    QString originalText;

    SettingsDialog *settingsDialog;

    void loadSettings();
    void saveSettings();
    void applySettings(int textSize, const QString &theme);
    void applyTheme(const QString &theme);
};
#endif // MAINWINDOW_H
