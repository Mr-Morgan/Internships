#ifndef TEXTEDIT_H
#define TEXTEDIT_H

#include <QPlainTextEdit>
#include <QtConcurrent>
#include <QObject>
#include <QAction>

#include "scriptsworker.h"
#include "scriptsbox.h"

class TextEdit : public QPlainTextEdit
{
    Q_OBJECT
public:
    TextEdit(QWidget *parent = nullptr);
    void insertPlainTextSMode(QString text);
    void scriptRun(QString name);

private:
    const char *bufferFN = "buffer.buf";        //либо убрать,
    const char *stdoutFN = "out.log";           //либо глобальные константы,
    const char *stderrFN = "err.log";           //либо константы в namespace
    std::vector<QAction*> actions;
    ScriptsWorker *worker;
    ScriptsBox sBox;

};//class TextEdit : public QPlainTextEdit

#endif // TEXTEDIT_H
