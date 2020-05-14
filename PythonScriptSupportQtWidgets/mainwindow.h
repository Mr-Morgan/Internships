#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTextStream>
#include <QString>
#include <QFile>

#include <Python.h>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

enum registerType {rUpper, rTitle, rLower};

inline registerType& operator++(registerType &eRT, int)
{
   const int i = static_cast<int>(eRT) + 1;
   eRT = static_cast<registerType>((i) % 3);
   return eRT;
}//inline registerType& operator++(registerType &eDOW, int)

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    enum dateFlag {dAll, dOnlyDate, dOnlyTime};
    //Функция удаляющая существующие файлы
    void removeFiles(const char *bufferFN = "buffer.buf", const char *stdoutFN = "out.log", const char *stderrFN = "err.log");
    //Функция запускающая Python интерпритатор
    QString runPyScript(QString script, QString buffer = "",
                        const char *bufferFN = "buffer.buf",
                        const char *stdoutFN = "out.log",
                        const char *stderrFN = "err.log");

    //Функция возвращающая текущую дату в зависимости от флага
    QString dateTime(dateFlag flag);
    //Функция возвращающая строку с измененным регистром upper/title/lower
    QString registerChange(QString &buffer);
    //Функция возвращающая строку с заменой табуляции на 4 пробела
    QString fourSpacesInsteadTabs(QString &buffer);

private slots:
    void on_pushButton_clicked();
    void on_dateTimeAllButton_clicked();
    void on_dateTimeOnlyDateButton_clicked();
    void on_dateTimeOnlyTimeButton_clicked();
    void on_UpperLowerTitleButton_clicked();
    void on_fourSpacesButton_clicked();

    void on_openButton_clicked();
    void on_saveButton_clicked();
    void on_interruptButton_clicked();
    void on_checkBox_clicked(bool checked);

private:
    Ui::MainWindow *ui;
    const char *bufFileName = "buffer.buf";
    const char *stdoutFN = "out.log";
    const char *stderrFN = "err.log";
    registerType type = rUpper;
};//class MainWindow : public QMainWindow

#endif // MAINWINDOW_H
