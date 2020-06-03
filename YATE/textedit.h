#ifndef TEXTEDIT_H
#define TEXTEDIT_H

#include <QCloseEvent>
#include <QTextEdit>        //---------------??????
#include <QPainter>
#include <QTextBlock>
#include <QFile>
#include <QMessageBox>
#include <QTextStream>
#include <QGuiApplication>
#include <QFileDialog>
//#include <QObject>
#include <QAction>
#include <QtConcurrent>
#include <QPlainTextEdit>

#include "MenuBar/menubar.h"
#include "buffer.h"
#include "highlighter.h"
#include "settingsloader.h"
#include "Style/appearance.h"
#include "Scripts/scriptsworker.h"
#include "Scripts/scriptsbox.h"

QT_BEGIN_NAMESPACE
class QPaintEvent;
class QResizeEvent;
class QSize;
class QWidget;
QT_END_NAMESPACE

class LineNumberArea;
class Highlighter;
class Appearance;

class TextEdit : public QPlainTextEdit
{
    Q_OBJECT
public:
    TextEdit(Appearance *appearance = nullptr, Buffer *buffer = nullptr);
    ~TextEdit();
    //**************** lineNumberArea *******************************
    void lineNumberAreaPaintEvent(QPaintEvent *event);
    int lineNumberAreaWidth();
    //TODO добавить в настройки программы

    //***************************************************************
    //*********************** ScriptSupport *************************
    void insertPlainTextSMode(QString text);
    void scriptRun(QString name);
    void updateActions();
//    void


    Buffer *buffer() const;
    void setBuffer(Buffer *buffer);
    Highlighter * highlighter() const;
    bool saveFile();
    bool saveAsFile();

protected:
    //**************** lineNumberArea *******************************
    void resizeEvent(QResizeEvent *event) override;
    //***************************************************************
     void closeEvent(QCloseEvent *event) override;

private:
    Buffer *m_buffer;
    Highlighter * m_highlighter;
    Appearance *m_appearance;
    //**************** lineNumberArea *******************************
    QWidget *lineNumberArea;
    //TODO Добавить в стиль оформления
    const int lineNumberAreaPadding = 15;
    bool VisibleLineNumber;
    //*********************** ScriptSupport *************************
    QList<QAction*> actions;
    ScriptsWorker *worker;
    ScriptsBox sBox;

private slots:
    //**************** lineNumberArea *******************************
    void updateLineNumberAreaWidth();
    void highlightCurrentLine();
    void updateLineNumberArea(const QRect &rect, int numPixelsScrolledVertically);
    //***************************************************************
    void newFile();
    void loadFile();
    bool maybeSave();
    void documentWasModified();
    void showErrorMsg(const QString &err);

    //*********************** ScriptSupport *************************
    void runAction();


public slots:

    void  VisibleLine(bool status){
        VisibleLineNumber = status;
        updateLineNumberAreaWidth();
       }
    bool getVisibleLine(){
           return VisibleLineNumber;
       }
signals:
    void deleteBuffer(Buffer*);
};//class TextEdit : public QPlainTextEdit
//***************************************************************
class LineNumberArea : public QWidget
{
public:
    LineNumberArea(TextEdit *editor) : QWidget(editor), codeEditor(editor)
    {}

    QSize sizeHint() const override
    {
        return QSize(codeEditor->lineNumberAreaWidth(), 0);
    }

protected:
    void paintEvent(QPaintEvent *event) override
    {
        codeEditor->lineNumberAreaPaintEvent(event);
    }

private:
    TextEdit *codeEditor;
};

#endif // TEXTEDIT_H
