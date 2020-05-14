#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>

/*
 *  Cамое длинной слово

ls = buffer.split(" ")
max_len = 0
word = ""
for el in ls:
   word_len = len(el)
   if word_len > max_len:
      max_len = word_len
      word = el
buffer = word + " (" + str(max_len) + ")"

 *  Число вхождений строки

find_word = "он"
count = buffer.count(find_word)
buffer = buffer + " (Найдено " + str(count) + " повторений слова \"" + find_word + "\")"

 *  Арифметическая прогрессия (вывод через ' ')

buffer = ""
a = 1   #Первый член прогрессии
d = -2  #Разность
n = 10  #Количество членов прогрессии
i = 1
s = a   #Сумма арифметической прогрессии
buffer += str(a) + ' '
while i<n:
    a = a + d
    i+=1
    buffer += str(a) + ' '
s += a
s *= n/2
buffer += '\n' + str(s) + '\n'

*/

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //Инициализация интерпретатора Python
    Py_Initialize();
}//MainWindow(QWidget *parent = nullptr);

MainWindow::~MainWindow()
{
    //Завершение работы интерпретатора Python
    Py_Finalize();
    delete ui;
}//~MainWindow();

void MainWindow::removeFiles(const char *bufferFN, const char *stdoutFN, const char *stderrFN)
{
    QFile file(bufferFN);
    file.remove();
    file.setFileName(stdoutFN);
    file.remove();
    file.setFileName(stderrFN);
    file.remove();
}//void removeFiles(const char *bufferFN = "buffer.buf", const char *stdoutFN = "out.log", const char *stderrFN = "err.log");

QString MainWindow::runPyScript(QString script, QString buffer, const char *bufferFN, const char *stdoutFN, const char *stderrFN)
{
    removeFiles(bufferFN, stdoutFN, stderrFN);

    if(buffer.length())
        if(buffer[buffer.length()-1] == '\n') buffer = buffer.left(buffer.length()-1);

    buffer.replace(QString("\\"), QString("\\\\"));
    buffer.replace(QString("\""), QString("\\\""));
    buffer.replace(QString("\n"), QString("\\n"));
    buffer.replace(QString("\t"), QString("\\t"));

    QStringList list = script.split('\n');
    script = "";
    for (auto i : list)
        if(i[0] != '#') script += i + '\n';
    script = script.left(script.length()-1);

    QString python_code = "import sys\n"
                          "import Buffer\n"
                          "sys.stdout = open('%3', 'w')\n"
                          "sys.stderr = open('%4', 'w')\n"
                          "buffer = \"%1\"\n%2\n"
                          "Buffer.setBuffer(buffer)\n"
                          "sys.stdout.close()\n"
                          "sys.stderr.close()";

    python_code = python_code.arg(buffer).arg(script).arg(stdoutFN).arg(stderrFN);

    //Исполнение строки в Python интерпретаторе
    PyRun_SimpleString(python_code.toStdString().c_str());

    //Вывод данных измененных Python скриптом из файла
    QString buff = "???";
    QFile file(bufferFN);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream stream(&file);
        buff = stream.readAll();
        file.close();
        return buff;
    } else return "#@!Error in Python script code.";
}//QString runPyScript(QString script, QString buffer = "", const char *bufferFN = "buffer.buf", const char *stdoutFN = "out.log", const char *stderrFN = "err.log");

QString MainWindow::dateTime(MainWindow::dateFlag flag)
{
    //Строка содержащая скрипт
    QString script = "import datetime\n"
                     "buffer = datetime.datetime.today().strftime(\"%1\")";
    QString arg = "???";

    switch (flag) {
    case dAll: arg = "%m.%d.%Y %H:%M:%S";
        break;
    case dOnlyDate: arg = "%m.%d.%Y";
        break;
    case dOnlyTime: arg = "%H:%M:%S";
        break;
    }//switch (flag)
    script = script.arg(arg);

    return runPyScript(script);
}//QString dateTime(dateFlag flag);

QString MainWindow::registerChange(QString &buffer)
{
    if(buffer.length()) {
        //Строка содержащая скрипт
        QString script = "???";

        switch (type) {
        case rUpper: script = "buffer = buffer.upper()";
            break;
        case rTitle: script = "buffer = buffer.title()";
            break;
        case rLower: script = "buffer = buffer.lower()";
            break;
        }//switch (type)

        type++;
        return runPyScript(script, buffer);
    } else return buffer;
}//QString registerChange(QString &buffer) const;

QString MainWindow::fourSpacesInsteadTabs(QString &buffer)
{
    if(buffer.length()) {
        //Строка содержащая скрипт
        QString script = "buffer = buffer.replace(\'\\t\', \'    \')";
        return runPyScript(script, buffer);
    } else return buffer;
}//QString fourSpacesInsteadTabs(QString &buffer);

void MainWindow::on_pushButton_clicked()
{
        ui->resultEdit->setPlainText(runPyScript(ui->pyScriptEdit->toPlainText(), ui->textEdit->toPlainText()));
        QString log = "log";
        QFile file(stderrFN);
        if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            ui->logEdit->setTextColor(QColor("#7F0000"));
            QTextStream stream(&file);
            log = stream.readAll();
            ui->logEdit->setText(log);
            file.close();
        }//if (file.open(QIODevice::ReadOnly | QIODevice::Text))
        file.setFileName(stdoutFN);
        if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            ui->logEdit->setTextColor(QColor("#8f5902"));
            QTextStream stream(&file);
            log = stream.readAll();
            ui->logEdit->insertPlainText(log);
            file.close();
        }//if (file.open(QIODevice::ReadOnly | QIODevice::Text))
}//void on_pushButton_clicked();

void MainWindow::on_dateTimeAllButton_clicked()
{
    ui->textEdit->insertPlainText(dateTime(dateFlag::dAll));
}//void on_dateTimeAllButton_clicked();

void MainWindow::on_dateTimeOnlyDateButton_clicked()
{
    ui->textEdit->insertPlainText(dateTime(dateFlag::dOnlyDate));
}//void on_dateTimeOnlyDateButton_clicked();

void MainWindow::on_dateTimeOnlyTimeButton_clicked()
{
    ui->textEdit->insertPlainText(dateTime(dateFlag::dOnlyTime));
}//void on_dateTimeOnlyTimeButton_clicked();

void MainWindow::on_UpperLowerTitleButton_clicked()
{
    //Получаем буффер из выделенного текста
    QTextCursor cursor = ui->textEdit->textCursor();
    QString buffer = cursor.selectedText();
    int32_t start = cursor.selectionStart(),
            end = cursor.selectionEnd();

    QString text = ui->textEdit->toPlainText();

    text = text.left(start) + registerChange(buffer) + text.right(text.length() - end);

    ui->textEdit->setPlainText(text);

    //Возвращаем выделение
    cursor.setPosition(start);
    cursor.movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor, end - start);
    ui->textEdit->setTextCursor(cursor);

}//void on_UpperLowerTitleButton_clicked();

void MainWindow::on_fourSpacesButton_clicked()
{
    //Получаем буффер из выделенного текста
    QTextCursor cursor = ui->textEdit->textCursor();
    QString buffer = cursor.selectedText();
    int32_t start = cursor.selectionStart(),
            end = cursor.selectionEnd();

    QString text = ui->textEdit->toPlainText();

    text = text.left(start) + fourSpacesInsteadTabs(buffer) + text.right(text.length() - end);

    ui->textEdit->setPlainText(text);

    //Возвращаем выделение
    cursor.setPosition(start);
    cursor.movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor, end - start);
    ui->textEdit->setTextCursor(cursor);
}//void on_fourSpacesButton_clicked();

void MainWindow::on_openButton_clicked()
{
    std::map<QString,QString> m;
}//void on_openButton_clicked();

void MainWindow::on_saveButton_clicked()
{

}//void on_saveButton_clicked();

void MainWindow::on_interruptButton_clicked()
{
    Py_Finalize();
    Py_Initialize();
}//void on_interruptButton_clicked();

void MainWindow::on_checkBox_clicked(bool checked)
{
   ui->interruptButton->setEnabled(!checked);
}//void on_checkBox_clicked(bool checked);
