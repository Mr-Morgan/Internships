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
print(buffer)

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

    m_process = new QProcess(this);

    connect(m_process, SIGNAL(readyReadStandardOutput()), this, SLOT(stdOut()));
    connect(m_process, SIGNAL(readyReadStandardError()),  this, SLOT(stdErr()));
    connect(m_process, SIGNAL(errorOccurred()), this, SLOT(changedButtonEnabled(int)));
    connect(m_process, SIGNAL(finished(int)), this, SLOT(changedButtonEnabled(int)));
    connect(ui->interruptButton, SIGNAL(clicked()), this, SLOT(stop()));

    autoStop = new QTimer(this);
    autoStop->setInterval(ui->spinBox->value()*1000);
    connect(autoStop, &QTimer::timeout, [&] () {
        if (ui->interruptButton->isEnabled()) stop();
    });//connect(...)
}//MainWindow(QWidget *parent = nullptr);

MainWindow::~MainWindow()
{
    delete ui;
}//~MainWindow();

void MainWindow::on_pushButton_clicked()
{
    changedButtonEnabled(0);
    run();
}//void on_pushButton_clicked();

void MainWindow::on_dateTimeAllButton_clicked()
{
    ui->textEdit->scriptRun("Вставка текущей даты и времени");
}//void on_dateTimeAllButton_clicked();

void MainWindow::on_dateTimeOnlyDateButton_clicked()
{
    ui->textEdit->scriptRun("Вставка текущей даты");
}//void on_dateTimeOnlyDateButton_clicked();

void MainWindow::on_dateTimeOnlyTimeButton_clicked()
{
    ui->textEdit->scriptRun("Вставка текущего времени");
}//void on_dateTimeOnlyTimeButton_clicked();

void MainWindow::on_UpperLowerTitleButton_clicked()
{
    ui->textEdit->scriptRun("Смена регистра выделенного фрагмента");
}//void on_UpperLowerTitleButton_clicked();

void MainWindow::on_fourSpacesButton_clicked()
{
    ui->textEdit->scriptRun("Замена табуляции на четыре пробела");
}//void on_fourSpacesButton_clicked();

void MainWindow::on_checkBox_clicked(bool checked)
{
    (checked)? autoStop->start() : autoStop->stop();
}//void on_checkBox_clicked(bool checked);

void MainWindow::stdOut()
{
    QByteArray output = m_process->readAllStandardOutput();
    ui->logEdit->setTextColor(QColor("#8f5902"));
    ui->logEdit->insertPlainText(QString(output));
}//void stdOut();

void MainWindow::stdErr()
{
    QByteArray output = m_process->readAllStandardError();
    ui->logEdit->setTextColor(QColor("#7F0000"));
    ui->logEdit->insertPlainText(QString(output));
}//void stdErr();

void MainWindow::run(QString param)
{
    QFile tmp("tmp.py");
    if (tmp.open(QIODevice::WriteOnly)) {
        QTextStream stream(&tmp);
        stream << ui->textEdit->toPlainText();
        tmp.close();
        m_process->start(QString("python3 %1 tmp.py").arg(param));
    }//if (tmp.open(QIODevice::WriteOnly))
}//void run(QString param);

void MainWindow::stop()
{
    m_process->close();
}//void stop();

void MainWindow::on_spinBox_valueChanged(int arg1)
{
    autoStop->setInterval(arg1*1000);
}//void on_spinBox_valueChanged(int arg1)

void MainWindow::changedButtonEnabled(int exitCode)
{
    ui->interruptButton->setEnabled(!ui->interruptButton->isEnabled());
    ui->pushButton->setEnabled(!ui->pushButton->isEnabled());
    Q_UNUSED(exitCode)
}//void changedButtonEnabled();
