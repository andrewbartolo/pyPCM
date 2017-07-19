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


static PyObject *getActiveAverageFrequency(PyObject *self, PyObject *args);
static PyObject *getActiveRelativeFrequency(PyObject *self, PyObject *args);
static PyObject *getAverageFrequency(PyObject *self, PyObject *args);
//static PyObject *getBytesReadFromMC(PyObject *self, PyObject *args);
//static PyObject *getBytesWrittenToMC(PyObject *self, PyObject *args);
static PyObject *getConsumedEnergyUnits(PyObject *self, PyObject *args);
static PyObject *getCycles(PyObject *self, PyObject *args);
//static PyObject *getDRAMClocks(PyObject *self, PyObject *args);
static PyObject *getDRAMConsumedEnergyUnits(PyObject *self, PyObject *args);
//static PyObject *getExecUsage(PyObject *self, PyObject *args);
//static PyObject *getInstructionsRetired(PyObject *self, PyObject *args);
static PyObject *getIPC(PyObject *self, PyObject *args);
static PyObject *getJoulesPerEnergyUnit(PyObject *self, PyObject *args);
//static PyObject *getL2CacheHitRatio(PyObject *self, PyObject *args);
//static PyObject *getL2CacheHits(PyObject *self, PyObject *args);
//static PyObject *getL2CacheMisses(PyObject *self, PyObject *args);
//static PyObject *getL3CacheHitRatio(PyObject *self, PyObject *args);
//static PyObject *getL3CacheHits(PyObject *self, PyObject *args);
//static PyObject *getL3CacheHitsNoSnoop(PyObject *self, PyObject *args);
//static PyObject *getL3CacheHitsSnoop(PyObject *self, PyObject *args);
//static PyObject *getL3CacheMisses(PyObject *self, PyObject *args);
//static PyObject *getL3CacheOccupancy(PyObject *self, PyObject *args);
//static PyObject *getL3ScalingFactor(PyObject *self, PyObject *args);
//static PyObject *getMaxIPC(PyObject *self, PyObject *args);


static PyMethodDef module_methods[] = {
    {"roi_begin", roi_begin, METH_VARARGS, module_docstring},
    {"roi_end", roi_end, METH_VARARGS, module_docstring},
    {"getActiveAverageFrequency", getActiveAverageFrequency, METH_VARARGS, module_docstring},
    {"getActiveRelativeFrequency", getActiveRelativeFrequency, METH_VARARGS, module_docstring},
    {"getAverageFrequency", getAverageFrequency, METH_VARARGS, module_docstring},
    {"getConsumedEnergyUnits", getConsumedEnergyUnits, METH_VARARGS, module_docstring},
    {"getCycles", getCycles, METH_VARARGS, module_docstring},
    {"getDRAMConsumedEnergyUnits", getDRAMConsumedEnergyUnits, METH_VARARGS, module_docstring},
    {"getIPC", getIPC, METH_VARARGS, module_docstring},
    {"getJoulesPerEnergyUnit", getJoulesPerEnergyUnit, METH_VARARGS, module_docstring},
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
        printf("Are you running as root?\n");
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



static PyObject *getActiveAverageFrequency(PyObject *self, PyObject *args) {
    double activeAverageFrequency = getActiveAverageFrequency(startState, endState);
    return Py_BuildValue("d", activeAverageFrequency);
}

static PyObject *getActiveRelativeFrequency(PyObject *self, PyObject *args) {
    double activeRelativeFrequency = getActiveRelativeFrequency(startState, endState);
    return Py_BuildValue("d", activeRelativeFrequency);
}

static PyObject *getAverageFrequency(PyObject *self, PyObject *args) {
    double averageFrequency = getAverageFrequency(startState, endState);
    return Py_BuildValue("d", averageFrequency);
}

//static PyObject *getBytesReadFromMC(PyObject *self, PyObject *args);
//static PyObject *getBytesWrittenToMC(PyObject *self, PyObject *args);
static PyObject *getConsumedEnergyUnits(PyObject *self, PyObject *args) {
    uint64_t consumedEnergyUnits = getConsumedEnergy(startState, endState);
    return Py_BuildValue("l", consumedEnergyUnits);
}
static PyObject *getCycles(PyObject *self, PyObject *args) {
    uint64_t cycles = getCycles(startState, endState);
    return Py_BuildValue("l", cycles);
}

//static PyObject *getDRAMClocks(PyObject *self, PyObject *args);
static PyObject *getDRAMConsumedEnergyUnits(PyObject *self, PyObject *args) {
    uint64_t dramConsumedEnergyUnits = getDRAMConsumedEnergy(startState, endState);
    return Py_BuildValue("l", dramConsumedEnergyUnits);
}

//static PyObject *getExecUsage(PyObject *self, PyObject *args);
//static PyObject *getInstructionsRetired(PyObject *self, PyObject *args);
static PyObject *getIPC(PyObject *self, PyObject *args) {
    double ipc = getIPC(startState, endState);
    return Py_BuildValue("d", ipc);
}

static PyObject *getJoulesPerEnergyUnit(PyObject *self, PyObject *args) {
    double joulesPerEnergyUnit = m->getJoulesPerEnergyUnit();
    return Py_BuildValue("d", joulesPerEnergyUnit);
}

//static PyObject *getL2CacheHitRatio(PyObject *self, PyObject *args);
//static PyObject *getL2CacheHits(PyObject *self, PyObject *args);
//static PyObject *getL2CacheMisses(PyObject *self, PyObject *args);
//static PyObject *getL3CacheHitRatio(PyObject *self, PyObject *args);
//static PyObject *getL3CacheHits(PyObject *self, PyObject *args);
//static PyObject *getL3CacheHitsNoSnoop(PyObject *self, PyObject *args);
//static PyObject *getL3CacheHitsSnoop(PyObject *self, PyObject *args);
//static PyObject *getL3CacheMisses(PyObject *self, PyObject *args);
//static PyObject *getL3CacheOccupancy(PyObject *self, PyObject *args);
//static PyObject *getL3ScalingFactor(PyObject *self, PyObject *args);
//static PyObject *getMaxIPC(PyObject *self, PyObject *args);


}
