#ifndef SCRIPTSWORKER_H
#define SCRIPTSWORKER_H

#include "scriptsexecutor.h"

#include <QObject>
#include <QMutex>

class ScriptsWorker : public QObject
{
    Q_OBJECT
public:
    explicit ScriptsWorker(QObject *parent = nullptr);
    ~ScriptsWorker();
    QString pyWork(QString script, QString buffer = "",
                const char *bufferFN = "buffer.buf",
                const char *stdoutFN = "out.log",
                const char *stderrFN = "err.log");

private:
    QMutex mutex;
    ScriptsExecutor *executor;
};//class ScriptsWorker : public QObject

#endif // SCRIPTSWORKER_H
