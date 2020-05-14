#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    sList.clear();
    readPythonScriptDB();
    for (auto i : sList)  {
        QListWidgetItem* item = new QListWidgetItem(i.first);
        item->setFlags(Qt::ItemIsEditable | Qt::ItemIsEnabled | Qt::ItemIsSelectable);
        ui->list->addItem(item);
    }//for (auto i : sList)

    ui->label_3->setVisible(false);
    ui->lineEdit->setVisible(false);
}//MainWindow(QWidget *parent = nullptr);

MainWindow::~MainWindow()
{
    writePythonScriptDB();
    delete ui;
}//~MainWindow();

bool MainWindow::readPythonScriptDB()
{
    QFile file(pyScritpDBFileName);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream s(&file);
        QString tmp = s.readLine();
        if (tmp != "#@MPSDB") return false;
        QString name = "??", script = "??";
        for (tmp = s.readLine(); !s.atEnd(); tmp = s.readLine()) {
            if (tmp == "#@N") name = s.readLine();
            if (tmp == "#@{") {
                script = "";
                for (tmp = s.readLine(); ; tmp = s.readLine()) {
                    if (tmp != "#@}") script += tmp + '\n';
                    else {
                        sList[name] = script;
                        break;
                    }//if (tmp != "#@}")
                }//for (tmp = s.readLine(); ; tmp = s.readLine())
            }//if (tmp == "#@{")
        }//for (tmp = s.readLine(); !s.atEnd(); tmp = s.readLine())
        file.close();
    }//if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    return true;
}//bool readPythonScritpDB();

bool MainWindow::writePythonScriptDB() const
{
    QFile file(pyScritpDBFileName);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream s(&file);
        s << "#@MPSDB\n";
        for (auto i : sList)
            s << "#@N\n" << i.first << "\n#@{\n" << i.second << "\n#@}\n";
        file.close();
    }//if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    return true;
}//bool writePythonScritpDB() const;

void MainWindow::changePythonScript(QString name)
{
    sList[name] = ui->text->toPlainText();
}//bool changePythonScritp(QString name);

void MainWindow::on_newButton_clicked()
{
    ui->label_3->setVisible(true);
    ui->lineEdit->setVisible(true);
}//void on_newButton_clicked();

void MainWindow::on_saveButton_clicked()
{
    changePythonScript(ui->list->currentItem()->text());
}//void on_saveButton_clicked();

void MainWindow::on_list_clicked(const QModelIndex &index)
{
    (sList.count(index.data().toString()))? ui->text->setPlainText(sList[index.data().toString()])
                                          : ui->text->setPlainText("");
}//void on_list_clicked(const QModelIndex &index);

void MainWindow::on_text_textChanged()
{
    changePythonScript(ui->list->currentItem()->text());
}//void on_text_textChanged();

void MainWindow::on_lineEdit_returnPressed()
{
    ui->label_3->setVisible(false);
    ui->lineEdit->setVisible(false);
    QListWidgetItem* item = new QListWidgetItem(ui->lineEdit->text());
    item->setFlags(Qt::ItemIsEditable | Qt::ItemIsEnabled | Qt::ItemIsSelectable);
    ui->list->addItem(item);
}//void on_lineEdit_returnPressed();

void MainWindow::on_deleteButton_clicked()
{
    sList.erase(ui->list->currentItem()->text());
    if(QListWidgetItem* item = ui->list->currentItem())
        delete item;
}//void on_deleteButton_clicked();

void MainWindow::on_list_doubleClicked(const QModelIndex &index)
{
    tmp_key = index.data().toString();
}//void on_list_doubleClicked(const QModelIndex &index);

void MainWindow::on_list_itemChanged(QListWidgetItem *item)
{
    auto it = sList.find(tmp_key);
    if (it != sList.cend())
        if (sList.try_emplace(ui->list->currentItem()->text(), it->second).second)
            sList.erase(it);
    Q_UNUSED(item)
}//void on_list_itemChanged(QListWidgetItem *item);
