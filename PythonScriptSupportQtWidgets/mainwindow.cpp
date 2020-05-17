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

i = 1
while i:
    print(str(i))
    i+=1
*/

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    worker = new ScriptsWorker(this);
    connect(this, SIGNAL(stopAllPyScripts()), worker, SLOT(pyStop()));
}//MainWindow(QWidget *parent = nullptr);

MainWindow::~MainWindow()
{
    delete ui;
}//~MainWindow();

void MainWindow::on_pushButton_clicked()
{
    ui->resultEdit->setPlainText(QtConcurrent::run(worker, &ScriptsWorker::pyWork, ui->pyScriptEdit->toPlainText(),
                                                   ui->textEdit->toPlainText(), bufferFN, stdoutFN, stderrFN));
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
{//Добавить в скрипты с флагом D
    QString script = "import datetime\n"
                     "buffer = datetime.datetime.today().strftime(\"%m.%d.%Y %H:%M:%S\")";
    ui->textEdit->insertPlainText(QtConcurrent::run(worker, &ScriptsWorker::pyWork, script, QString(""), bufferFN, stdoutFN, stderrFN));
}//void on_dateTimeAllButton_clicked();

void MainWindow::on_dateTimeOnlyDateButton_clicked()
{//Добавить в скрипты с флагом D
    QString script = "import datetime\n"
                     "buffer = datetime.datetime.today().strftime(\"%m.%d.%Y\")";
    ui->textEdit->insertPlainText(QtConcurrent::run(worker, &ScriptsWorker::pyWork, script, QString(""), bufferFN, stdoutFN, stderrFN));
}//void on_dateTimeOnlyDateButton_clicked();

void MainWindow::on_dateTimeOnlyTimeButton_clicked()
{//Добавить в скрипты с флагом D
    QString script = "import datetime\n"
                     "buffer = datetime.datetime.today().strftime(\"%H:%M:%S\")";
    ui->textEdit->insertPlainText(QtConcurrent::run(worker, &ScriptsWorker::pyWork, script, QString(""), bufferFN, stdoutFN, stderrFN));
}//void on_dateTimeOnlyTimeButton_clicked();

void MainWindow::on_UpperLowerTitleButton_clicked()
{//Добавить в скрипты с флагом S
    //Получаем буффер из выделенного текста
    QTextCursor cursor = ui->textEdit->textCursor();
    QString buffer = cursor.selectedText();
    if(buffer.length()) {
        int32_t start = cursor.selectionStart(),
                end = cursor.selectionEnd();
        QString text = ui->textEdit->toPlainText();

        //Строка содержащая скрипт
        QString script = "if buffer.isupper(): buffer = buffer.title()\n"
                         "elif (buffer.istitle()): buffer = buffer.lower()\n"
                         "else : buffer = buffer.upper()";
        buffer = QtConcurrent::run(worker, &ScriptsWorker::pyWork, script, buffer, bufferFN, stdoutFN, stderrFN);
        text = text.left(start) + buffer + text.right(text.length() - end);
        ui->textEdit->setPlainText(text);

        //Возвращаем выделение
        cursor.setPosition(start);
        cursor.movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor, end - start);
        ui->textEdit->setTextCursor(cursor);
    } else return;
}//void on_UpperLowerTitleButton_clicked();

void MainWindow::on_fourSpacesButton_clicked()
{//Добавить в скрипты с флагом S
    //Получаем буффер из выделенного текста
    QTextCursor cursor = ui->textEdit->textCursor();
    QString buffer = cursor.selectedText();
    if(buffer.length()) {
        int32_t start = cursor.selectionStart(),
                end = cursor.selectionEnd();

        QString text = ui->textEdit->toPlainText();
        //Строка содержащая скрипт
        QString script = "buffer = buffer.replace(\'\\t\', \'    \')";
        buffer = QtConcurrent::run(worker, &ScriptsWorker::pyWork, script, buffer, bufferFN, stdoutFN, stderrFN);
        text = text.left(start) + buffer + text.right(text.length() - end);
        ui->textEdit->setPlainText(text);

        //Возвращаем выделение
        cursor.setPosition(start);
        cursor.movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor, end - start);
        ui->textEdit->setTextCursor(cursor);
    } else return;
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
    emit stopAllPyScripts();
}//void on_interruptButton_clicked();

void MainWindow::on_checkBox_clicked(bool checked)
{
    ui->interruptButton->setEnabled(!checked);
}//void on_checkBox_clicked(bool checked);
