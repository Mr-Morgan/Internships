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

QString ScriptsWorker::pyWork(QString script, QString buffer)
{
    mutex.lock();
    QString result = executor->runPyScript(script, buffer);
    mutex.unlock();
    return result;
}//void pyWork(QString script, QString buffer = "");

