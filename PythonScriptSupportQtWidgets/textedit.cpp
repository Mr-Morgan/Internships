#include "textedit.h"

TextEdit::TextEdit(QWidget *parent):QPlainTextEdit(parent)
{
    worker = new ScriptsWorker(this);
}//TextEdit(QPlainTextEdit *parent = nullptr);

void TextEdit::insertPlainTextSMode(QString text)
{
    QTextCursor cursor = textCursor();
    if(cursor.selectedText().length()) {
        int32_t start = cursor.selectionStart();
        insertPlainText(text);
        cursor.setPosition(start);
        cursor.movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor, text.length());
        setTextCursor(cursor);
    } else return;
}//void insertPlainTextSMode(QString text);

void TextEdit::scriptRun(QString name)
{
    //проверка существования скрипта с таким именем
    if (sBox.getNames().indexOf(name) == -1) return;
    std::string mode = sBox.getInputMode(name).toStdString();
    QString buf = "";

    if (mode[0] == 'P') {
        if (mode[1] == 'S') {
            buf = textCursor().selectedText();
//            QString t = sBox.getScript(name);
            insertPlainTextSMode(QtConcurrent::run(worker, &ScriptsWorker::pyWork, sBox.getScript(name), buf, bufferFN, stdoutFN, stderrFN));
        } else {
            if (mode[1] == 'B') buf = toPlainText();
            insertPlainText(QtConcurrent::run(worker, &ScriptsWorker::pyWork, sBox.getScript(name), buf, bufferFN, stdoutFN, stderrFN));
        }//if (mode[1] == 'S')
    } else return;
}//void scriptRun(QString name);
