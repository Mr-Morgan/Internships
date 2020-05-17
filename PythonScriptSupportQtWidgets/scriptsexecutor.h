#ifndef SCRIPTSEXECUTOR_H
#define SCRIPTSEXECUTOR_H

#include <QTextStream>
#include <QString>
#include <QFile>

#include <Python.h>
#include <QObject>

class ScriptsExecutor : public QObject
{
    Q_OBJECT
public:
    explicit ScriptsExecutor(QObject *parent = nullptr);

    //Функция удаляющая существующие файлы
    void removeFiles(const char *bufferFN = "buffer.buf", const char *stdoutFN = "out.log", const char *stderrFN = "err.log");
    //Функция запускающая Python интерпритатор
    QString runPyScript(QString script, QString buffer = "",
                        const char *bufferFN = "buffer.buf",
                        const char *stdoutFN = "out.log",
                        const char *stderrFN = "err.log");
    //Инициализация интерпретатора Python
    void pyStart();
    //Завершение работы интерпретатора Python
    void pyStop();
};//ScriptsExecutor : public QObject

#endif // SCRIPTSEXECUTOR_H
