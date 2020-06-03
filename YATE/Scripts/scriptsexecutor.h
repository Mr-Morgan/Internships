#ifndef SCRIPTSEXECUTOR_H
#define SCRIPTSEXECUTOR_H

#include <QTextStream>
#include <QObject>
#include <QString>
#include <QFile>

#include <Python.h>

class ScriptsExecutor : public QObject
{
    Q_OBJECT
public:
    explicit ScriptsExecutor(QObject *parent = nullptr);

    //Функция удаляющая существующие файлы
    void removeFiles();
    //Функция запускающая Python интерпритатор
    QString runPyScript(QString script, QString buffer = "");
    //Инициализация интерпретатора Python
    void pyStart();
    //Завершение работы интерпретатора Python
    void pyStop();
private:
    const char *bufferFN = "tmp/buffer.buf";
    const char *stdoutFN = "tmp/out.log";
    const char *stderrFN = "tmp/err.log";
};//ScriptsExecutor : public QObject

#endif // SCRIPTSEXECUTOR_H
