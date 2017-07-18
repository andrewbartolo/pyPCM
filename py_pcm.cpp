#include "/file0/bartolo/usr/python2/include/python2.7/Python.h"
#include <cpucounters.h>

static char module_docstring[] = "Python hooks for Intel PCM.";

static double globval = 0.0;
static PCM *m = NULL;
static SystemCounterState startState;
static SystemCounterState endState;

// prototypes
static PyObject *roi_begin(PyObject *self, PyObject *args);
static PyObject *roi_end(PyObject *self, PyObject *args);

static PyMethodDef module_methods[] = {
    {"roi_begin", roi_begin, METH_VARARGS, module_docstring},
    {"roi_end", roi_end, METH_VARARGS, module_docstring},
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
    //zsim_roi_begin();
    m = PCM::getInstance();
    // build and return -1 if failed
    startState = getSystemCounterState();
    // clear out the old end state, if any (TEST if reentrant?)
    endState = startState;
    globval = 69.999;
    Py_RETURN_NONE;
}

static PyObject *roi_end(PyObject *self, PyObject *args) {
    //zsim_roi_end();
    //Py_RETURN_NONE;
    endState = getSystemCounterState();
    double ipc = getIPC(startState, endState);
    return Py_BuildValue("d", ipc);
}
