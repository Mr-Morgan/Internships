#ifndef BUFFER_H
#define BUFFER_H

#include <Python.h>
#include <fstream>

/*
 * Python модуль содержащий один метод
 * setString() сохраняет строку в файл
 * "buffer.buf".
*/

const char *bufFileName = "buffer.buf";

static PyObject *setString(PyObject * self, PyObject* args)
{
    char * toPrint;
    if(!PyArg_ParseTuple(args, "s", &toPrint)) return NULL;

    std::ofstream out(bufFileName);
    out << toPrint;
    out.close();

    return Py_BuildValue("s", toPrint);
}//static PyObject *setString(PyObject * self, PyObject* args)

static PyMethodDef bufferMethod[] = {
    {"setBuffer", (PyCFunction)setString, METH_VARARGS, "Returns the buffer changed by the script"},
    {NULL, NULL, 0, NULL}
};//static PyMethodDef bufferMethod[]

static struct PyModuleDef Buffer =
{
    PyModuleDef_HEAD_INIT,
    "Buffer", /* name of module */
    "usage: Buffer.setString(lstSortableItems, comboSize)\n", /* module documentation, may be NULL */
    -1,   /* size of per-interpreter state of the module, or -1 if the module keeps state in global variables. */
    bufferMethod
};//static struct PyModuleDef Buffer

PyMODINIT_FUNC PyInit_Buffer(void)
{
    return PyModule_Create(&Buffer);
}//PyMODINIT_FUNC PyInit_Buffer(void)

#endif // BUFFER_H
