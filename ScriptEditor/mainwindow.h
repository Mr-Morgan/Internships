#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTextStream>
#include <QString>
#include <QFile>
#include <map>

#include <QListWidgetItem>
#include <QVariant>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    bool readPythonScriptDB();
    bool writePythonScriptDB() const;
    void changePythonScript(QString name);

private slots:
    void on_newButton_clicked();
    void on_saveButton_clicked();
    void on_list_clicked(const QModelIndex &index);
    void on_text_textChanged();
    void on_lineEdit_returnPressed();
    void on_deleteButton_clicked();
    void on_list_doubleClicked(const QModelIndex &index);
    void on_list_itemChanged(QListWidgetItem *item);

private:
    Ui::MainWindow *ui;
    std::map<QString,QString> sList;
    const QString pyScritpDBFileName = "pysdb.mpsdb";
    QString tmp_key = "???";
};//class MainWindow : public QMainWindow
#endif // MAINWINDOW_H
