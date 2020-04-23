#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>

/*
 *  Cамое длинной слово

ls = buffer.split(" ")
max_len = 0
word = ""
for el in ls:
   world_len = len(el)
   if word_len > max_len:
      max_len = world_len
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

void MainWindow::on_pushButton_clicked()
{
    //Буфер
    QString buffer = "buffer = \"%1\"";
    buffer = buffer.arg(ui->textEdit->toPlainText());

    //Журнал
    QString log = "buffer:\n" + buffer + "\n\n";

    buffer.replace(QString("\n"), QString("\\n"));

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
}//void on_pushButton_clicked();
