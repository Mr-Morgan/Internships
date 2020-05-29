#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtConcurrent>
#include <QMainWindow>
#include <QTextStream>
#include <QProcess>
#include <QString>
#include <QFile>

#include <Python.h>

#include "scriptsworker.h"
#include "scriptsbox.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();
    void on_dateTimeAllButton_clicked();
    void on_dateTimeOnlyDateButton_clicked();
    void on_dateTimeOnlyTimeButton_clicked();
    void on_UpperLowerTitleButton_clicked();
    void on_fourSpacesButton_clicked();

    void on_checkBox_clicked(bool checked);

    void stdOut();
    void stdErr();
    void run(QString param = "");
    void stop();

    void on_spinBox_valueChanged(int arg1);

    void changedButtonEnabled(int exitCode);

private:
    const char *bufferFN = "buffer.buf";
    const char *stdoutFN = "out.log";
    const char *stderrFN = "err.log";
//    ScriptsWorker *worker;
    QProcess *m_process;
    Ui::MainWindow *ui;
    QTimer *autoStop;
};//class MainWindow : public QMainWindow

#endif // MAINWINDOW_H
