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

QString MainWindow::dateTime(MainWindow::dateFlag flag) const
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

    //Строка содержащая код на языке Python
    QString python_code = "import Buffer\n" + script + "\n" + "Buffer.setBuffer(buffer)";

    //Исполнение строки в Python интерпретаторе
    PyRun_SimpleString(python_code.toStdString().c_str());

    //Вывод данных измененных Python скриптом из файла
    QString buff = "???";
    QFile file(bufFileName);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream stream(&file);
        buff = stream.readAll();
        file.close();
    }//if (file.open(QIODevice::ReadOnly | QIODevice::Text))

    return buff;
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

        //Строка содержащая код на языке Python
        QString python_code = "import Buffer\nbuffer = \"%1\"\n" + script + "\n" + "Buffer.setBuffer(buffer)";

        //Исполнение строки в Python интерпретаторе
        PyRun_SimpleString(python_code.arg(buffer).toStdString().c_str());

        //Вывод данных измененных Python скриптом из файла
        QString buff = "???";
        QFile file(bufFileName);
        if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream stream(&file);
            buff = stream.readAll();
            file.close();
        }//if (file.open(QIODevice::ReadOnly | QIODevice::Text))

        type++;
        return buff;
    } else return buffer;
}//QString registerChange(QString &buffer) const;

QString MainWindow::fourSpacesInsteadTabs(QString &buffer)
{
    if(buffer.length()) {
        //Строка содержащая скрипт
        QString script = "buffer = buffer.replace(\'\\t\', \'    \')";

        //Строка содержащая код на языке Python
        QString python_code = "import Buffer\nbuffer = \"%1\"\n" + script + "\n" + "Buffer.setBuffer(buffer)";

        //Исполнение строки в Python интерпретаторе
        PyRun_SimpleString(python_code.arg(buffer).toStdString().c_str());

        //Вывод данных измененных Python скриптом из файла
        QString buff = "???";
        QFile file(bufFileName);
        if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream stream(&file);
            buff = stream.readAll();
            file.close();
        }//if (file.open(QIODevice::ReadOnly | QIODevice::Text))

        return buff;
    } else return buffer;
}//QString fourSpacesInsteadTabs(QString &buffer);

void MainWindow::on_pushButton_clicked()
{
    //Буфер
    QString buffer = "buffer = \"%1\"";
    QString arg = ui->textEdit->toPlainText();

    if(arg.length()) {

        if(arg[arg.length()-1] == '\n') arg = arg.left(arg.length()-1);

        //Журнал
        QString log = "buffer:\n" + buffer + "\n\n";

        arg.replace(QString("\\"), QString("\\\\"));
        arg.replace(QString("\""), QString("\\\""));
        arg.replace(QString("\n"), QString("\\n"));
        arg.replace(QString("\t"), QString("\\t"));

        buffer = buffer.arg(arg);

        //Строка содержащая скрипт
        QString script = ui->pyScriptEdit->toPlainText();
        QStringList list = script.split('\n');

        log += "script:\n" + script + "\n\n";

        script = "";
        for (auto i : list)
            if(i[0] != '#') script += i + '\n';
        script = script.left(script.length()-1);

        //Строка содержащая код на языке Python
        QString python_code = "import Buffer\n" + buffer + "\n" + script + "\n" + "Buffer.setBuffer(buffer)";

        log += "buffer:\n" + buffer + "\n\n";
        log += "script:\n" + script + "\n\n";
        log += "python_code:\n" + python_code + "\n\n";

        //Исполнение строки в Python интерпретаторе
        PyRun_SimpleString(python_code.toStdString().c_str());

        //Вывод данных измененных Python скриптом из файла
        QString buff = "???";
        QFile file(bufFileName);
        if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream stream(&file);
            buff = stream.readAll();
            file.close();
        }//if (file.open(QIODevice::ReadOnly | QIODevice::Text))

        ui->resultEdit->setPlainText(buff);
        ui->logEdit->setTextColor(QColor("#8f5902"));
        ui->logEdit->setText(log);
    }//if(arg.length())
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

    buffer.replace(QString("\\"), QString("\\\\"));
    buffer.replace(QString("\""), QString("\\\""));
    buffer.replace(QString("\n"), QString("\\n"));
    buffer.replace(QString("\t"), QString("\\t"));

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

    buffer.replace(QString("\\"), QString("\\\\"));
    buffer.replace(QString("\""), QString("\\\""));
    buffer.replace(QString("\n"), QString("\\n"));
    buffer.replace(QString("\t"), QString("\\t"));

    QString text = ui->textEdit->toPlainText();

    text = text.left(start) + fourSpacesInsteadTabs(buffer) + text.right(text.length() - end);

    ui->textEdit->setPlainText(text);

    //Возвращаем выделение
    cursor.setPosition(start);
    cursor.movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor, end - start);
    ui->textEdit->setTextCursor(cursor);
}//void on_fourSpacesButton_clicked();
