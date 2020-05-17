#include "scriptsexecutor.h"

ScriptsExecutor::ScriptsExecutor(QObject *parent) : QObject(parent)
{
}//ScriptsExecutor();

void ScriptsExecutor::removeFiles(const char *bufferFN, const char *stdoutFN, const char *stderrFN)
{
    QFile file(bufferFN);
    file.remove();
    file.setFileName(stdoutFN);
    file.remove();
    file.setFileName(stderrFN);
    file.remove();
}//void removeFiles(const char *bufferFN = "buffer.buf", const char *stdoutFN = "out.log", const char *stderrFN = "err.log");

QString ScriptsExecutor::runPyScript(QString script, QString buffer, const char *bufferFN, const char *stdoutFN, const char *stderrFN)
{
    removeFiles(bufferFN, stdoutFN, stderrFN);
    freopen (stderrFN, "w", stderr);

    if(buffer.length()) {
        if(buffer[buffer.length()-1] == '\n') buffer = buffer.left(buffer.length()-1);
        buffer.replace(QString("\\"), QString("\\\\"));
        buffer.replace(QString("\""), QString("\\\""));
        buffer.replace(QString("\n"), QString("\\n"));
        buffer.replace(QString("\t"), QString("\\t"));
    }//if(buffer.length())

    QStringList list = script.split('\n');
    script = "";
    for (auto i : list)
        if(i[0] != '#') script += i + '\n';
    script = script.left(script.length()-1);

    QString python_code = "import sys\n"
                          "import Buffer\n"
                          "sys.stdout = open('%3', 'w')\n"
                          "buffer = \"%1\"\n%2\n"
                          "Buffer.setBuffer(buffer)\n"
                          "sys.stdout.close()";

    python_code = python_code.arg(buffer).arg(script).arg(stdoutFN);

    //Исполнение строки в Python интерпретаторе
    PyRun_SimpleString(python_code.toStdString().c_str());

    fclose(stderr);

    //Вывод данных измененных Python скриптом из файла
    QString buff = "???";
    QFile file(bufferFN);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream stream(&file);
        buff = stream.readAll();
        file.close();
        return buff;
    } else return "#@!Error: incorrect Python script code.";
}//QString runPyScript(QString script, QString buffer = "", const char *bufferFN = "buffer.buf", const char *stdoutFN = "out.log", const char *stderrFN = "err.log");

void ScriptsExecutor::pyStart()
{
    Py_Initialize();
}//void PyStart();

void ScriptsExecutor::pyStop()
{
    Py_Finalize();
}//void PyStop();
