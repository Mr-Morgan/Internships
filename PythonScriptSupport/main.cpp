#include <QCoreApplication>
#include <QTextStream>
#include <QString>
#include <QFile>

#include <iostream>
#include <Python.h>
#include "buffer.cpp"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    //Инициализация интерпретатора Python
    Py_Initialize();

    //Буфер
    std::string buf = "???";

    //Заполнение буффера из потока ввода
    std::getline(std::cin, buf);

    //Строка содержащая скрипт
    std::string script = "buffer.title()";

    //Строка содержащая код на языке Python
    std::string python_code = "import Buffer\n"
                              "buffer = \"" + buf + "\"\n"
                              "Buffer.setBuffer(" + script + ")";

    //Исполнение строки в Python интерпретаторе
    PyRun_SimpleString(python_code.c_str());

    //Вывод данных измененных Python скриптом из файла
    QString buff = "???";
    QFile file(bufFileName);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream stream(&file);
        buff = stream.readAll();
        file.close();
    }//if (file.open(QIODevice::ReadOnly | QIODevice::Text))

    std::cout << buff.toStdString() << std::endl;

    //Завершение работы интерпретатора Python
    Py_Finalize();
    Py_Exit(Py_QuietFlag);

    return a.exec();
}//int main(int argc, char *argv[])
