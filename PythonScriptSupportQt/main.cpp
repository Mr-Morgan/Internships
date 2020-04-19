#include <QCoreApplication>
#include <boost/python.hpp>
#include <iostream>

struct PyScript
{
    void set(std::string text) { this->text = text; }
    std::string get() { return text; }
    std::string text;
};//struct PyScript

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    boost::python::object main_module;
    boost::python::object main_namespace;

    //Буфер
    std::string buf = "???";

    //Указатель на объект структуры, поле которого будет заполняться из Python интерпретатора
    PyScript *ps = new PyScript();
    ps->set("???");

    //Инициализация интерпретатора Python
    Py_Initialize();

    //Получаем namespace
    main_module = boost::python::import("__main__");
    main_namespace = main_module.attr("__dict__");

    //Регестрирация структуры с ее методами
    main_namespace["PyScript"] = boost::python::class_<PyScript>("PyScript")
            .def("get", &PyScript::get)
            .def("set", &PyScript::set);

    //Теперь можно использовать эту структуру включая ее методы в интерпритаторе Python

    //Регестрирация указателя на структуру
    main_namespace["bufCpp"] = boost::python::ptr(ps);

    //Заполнение буффера из потока ввода
    std::getline(std::cin, buf);

    //Строка содержащая скрипт
    std::string script = "buffer.title()";

    //Строка содержащая код на языке Python
    std::string python_code = "buffer = \"" + buf + "\"\nbufCpp.set(" + script + ")";

    //Исполнение строки в Python интерпретаторе
    PyRun_SimpleString(python_code.c_str());

    //Вывод данных измененных Python скриптом
    std::cout << ps->get() << std::endl;

    //Возвращение ресурсов системе
    Py_DECREF(main_namespace.ptr());
    Py_DECREF(main_module.ptr());

    //Завершение работы интерпретатора Python
    Py_Finalize();
    Py_Exit(Py_QuietFlag);

    return a.exec();
}//int main(int argc, char *argv[])
