#include "scriptsworker.h"

ScriptsWorker::ScriptsWorker(QObject *parent) : QObject(parent)
{
    executor = new ScriptsExecutor(this);
    executor->pyStart();
}//explicit ScriptsWorker(QObject *parent = nullptr);

ScriptsWorker::~ScriptsWorker()
{
    executor->pyStop();
}//~ScriptsWorker();

QString ScriptsWorker::pyWork(QString script, QString buffer, const char *bufferFN, const char *stdoutFN, const char *stderrFN)
{
    mutex.lock();
    QString result = executor->runPyScript(script, buffer, bufferFN, stdoutFN, stderrFN);
    mutex.unlock();
    return result;
}//void pyWork(QString script, QString buffer = "", const char *bufferFN = "buffer.buf", const char *stdoutFN = "out.log", const char *stderrFN = "err.log");

