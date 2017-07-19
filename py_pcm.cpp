#include "/file0/bartolo/usr/python2/include/python2.7/Python.h"
#include <cpucounters.h>
#include <stdio.h>  // crude way to print errors

static char module_docstring[] = "Python hooks for Intel PCM.";

static PCM *m = NULL;
static SystemCounterState startState;
static SystemCounterState endState;

extern "C" {

// prototypes
static PyObject *roi_begin(PyObject *self, PyObject *args);
static PyObject *roi_end(PyObject *self, PyObject *args);
static PyObject *getIPC(PyObject *self, PyObject *args);

static PyMethodDef module_methods[] = {
    {"roi_begin", roi_begin, METH_VARARGS, module_docstring},
    {"roi_end", roi_end, METH_VARARGS, module_docstring},
    {"getIPC", getIPC, METH_VARARGS, module_docstring},
    {NULL, NULL, 0, NULL} // just a sentinel
};


PyMODINIT_FUNC initpyPCM(void)
{
    /*
    PyObject *module;
    static struct PyModuleDef moduledef = {
        PyModuleDef_HEAD_INIT,
        "pyPCM",
        module_docstring,
        -1,
        module_methods,
        NULL,
        NULL,
        NULL,
        NULL
    };
    module = PyModule_Create(&moduledef);
    if (!module) return NULL;
    return module;
    */
    (void) Py_InitModule("pyPCM", module_methods);
}

static PyObject *roi_begin(PyObject *self, PyObject *args) {
    m = PCM::getInstance();
    // TODO return an actual PyError if failed
    // TODO garbage-collect m?
    if (m->program() != PCM::Success) {
        printf("ERROR: PCM::getInstance() in pyPCM.roi_begin() failed\n");
        return NULL;
    }
    startState = getSystemCounterState();
    // clear out the old end state, if any (TEST if reentrant?)
    endState = startState;
    printf("[PCM ROI begin]\n");
    Py_RETURN_NONE;
}

static PyObject *roi_end(PyObject *self, PyObject *args) {
    endState = getSystemCounterState();
    printf("[PCM ROI end]\n");
    Py_RETURN_NONE;
}

static PyObject *getIPC(PyObject *self, PyObject *args) {
    double ipc = getIPC(startState, endState);
    return Py_BuildValue("d", ipc);
}

}
