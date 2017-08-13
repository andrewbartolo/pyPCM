#include "/file0/bartolo/usr/python2/include/python2.7/Python.h"
#include <cpucounters.h>
#include <stdio.h>  // crude way to print errors
#include <stdexcept>

static char module_docstring[] = "Python hooks for Intel PCM.";

static PCM *m = NULL;

static SystemCounterState systemStartState;
std::vector<SocketCounterState> socketStartStates; // currently never used by Python
std::vector<CoreCounterState> coreStartStates;

static SystemCounterState systemEndState;
std::vector<SocketCounterState> socketEndStates; // currently never used by Python
std::vector<CoreCounterState> coreEndStates;



extern "C" {

// prototypes
static PyObject *roi_begin(PyObject *self, PyObject *args);
static PyObject *roi_end(PyObject *self, PyObject *args);
static PyObject *cleanup(PyObject *self, PyObject *args);


static PyObject *getActiveAverageFrequency(PyObject *self, PyObject *args);
static PyObject *getActiveRelativeFrequency(PyObject *self, PyObject *args);
static PyObject *getAverageFrequency(PyObject *self, PyObject *args);
//static PyObject *getBytesReadFromMC(PyObject *self, PyObject *args);
//static PyObject *getBytesWrittenToMC(PyObject *self, PyObject *args);
static PyObject *getConsumedEnergyUnits(PyObject *self, PyObject *args);
static PyObject *getCycles(PyObject *self, PyObject *args);
static PyObject *getCoreCycles(PyObject *self, PyObject *args);
//static PyObject *getDRAMClocks(PyObject *self, PyObject *args);
static PyObject *getDRAMConsumedEnergyUnits(PyObject *self, PyObject *args);
//static PyObject *getExecUsage(PyObject *self, PyObject *args);
//static PyObject *getInstructionsRetired(PyObject *self, PyObject *args);
static PyObject *getCoreInstructionsRetired(PyObject *self, PyObject *args);
static PyObject *getIPC(PyObject *self, PyObject *args);
static PyObject *getJoulesPerEnergyUnit(PyObject *self, PyObject *args);
static PyObject *getL2CacheHitRatio(PyObject *self, PyObject *args);
static PyObject *getL2CacheHits(PyObject *self, PyObject *args);
static PyObject *getL2CacheMisses(PyObject *self, PyObject *args);
static PyObject *getNominalFrequency(PyObject *self, PyObject *args);
static PyObject *getNumCores(PyObject *self, PyObject *args);
static PyObject *getNumOnlineCores(PyObject *self, PyObject *args);
static PyObject *getNumSockets(PyObject *self, PyObject *args);
static PyObject *getSMT(PyObject *self, PyObject *args);

//static PyObject *getL3CacheHitRatio(PyObject *self, PyObject *args);
static PyObject *getL3CacheHits(PyObject *self, PyObject *args);
//static PyObject *getL3CacheHitsNoSnoop(PyObject *self, PyObject *args);
//static PyObject *getL3CacheHitsSnoop(PyObject *self, PyObject *args);
static PyObject *getL3CacheMisses(PyObject *self, PyObject *args);
//static PyObject *getL3CacheOccupancy(PyObject *self, PyObject *args);
//static PyObject *getL3ScalingFactor(PyObject *self, PyObject *args);
//static PyObject *getMaxIPC(PyObject *self, PyObject *args);


static PyMethodDef module_methods[] = {
    {"roi_begin", roi_begin, METH_VARARGS, module_docstring},
    {"roi_end", roi_end, METH_VARARGS, module_docstring},
    {"cleanup", cleanup, METH_VARARGS, module_docstring},
    {"getActiveAverageFrequency", getActiveAverageFrequency, METH_VARARGS, module_docstring},
    {"getActiveRelativeFrequency", getActiveRelativeFrequency, METH_VARARGS, module_docstring},
    {"getAverageFrequency", getAverageFrequency, METH_VARARGS, module_docstring},
    {"getConsumedEnergyUnits", getConsumedEnergyUnits, METH_VARARGS, module_docstring},
    {"getCycles", getCycles, METH_VARARGS, module_docstring},
    {"getCoreCycles", getCoreCycles, METH_VARARGS, module_docstring},
    {"getDRAMConsumedEnergyUnits", getDRAMConsumedEnergyUnits, METH_VARARGS, module_docstring},
    {"getCoreInstructionsRetired", getCoreInstructionsRetired, METH_VARARGS, module_docstring},
    {"getIPC", getIPC, METH_VARARGS, module_docstring},
    {"getJoulesPerEnergyUnit", getJoulesPerEnergyUnit, METH_VARARGS, module_docstring},
    {"getL2CacheHitRatio", getL2CacheHitRatio, METH_VARARGS, module_docstring},
    {"getL2CacheHits", getL2CacheHits, METH_VARARGS, module_docstring},
    {"getL2CacheMisses", getL2CacheMisses, METH_VARARGS, module_docstring},
    {"getL3CacheHits", getL3CacheHits, METH_VARARGS, module_docstring},
    {"getL3CacheMisses", getL3CacheMisses, METH_VARARGS, module_docstring},
    {"getNominalFrequency", getNominalFrequency, METH_VARARGS, module_docstring},
    {"getNumCores", getNumCores, METH_VARARGS, module_docstring},
    {"getNumOnlineCores", getNumOnlineCores, METH_VARARGS, module_docstring},
    {"getNumSockets", getNumSockets, METH_VARARGS, module_docstring},
    {"getSMT", getSMT, METH_VARARGS, module_docstring},
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

    // force-reset the counters before we begin our critical region
    m->resetPMU();

    if (m->program() != PCM::Success) {
        printf("ERROR: PCM::getInstance() in pyPCM.roi_begin() failed\n");
        printf("Are you running as root?\n");
        return NULL;
    }

    //systemStartState = getSystemCounterState();
    m->getAllCounterStates(systemStartState, socketStartStates, coreStartStates);

    // clear out the old end state, if any (TEST if reentrant?)
    // TODO clear out old vectors too
    systemEndState = systemStartState;
    printf("[PCM ROI begin]\n");
    Py_RETURN_NONE;
}

static PyObject *roi_end(PyObject *self, PyObject *args) {
    //systemEndState = getSystemCounterState();
    m->getAllCounterStates(systemEndState, socketEndStates, coreEndStates);
    printf("[PCM ROI end]\n");
    Py_RETURN_NONE;
}

static PyObject *cleanup(PyObject *self, PyObject *args) {
    if (m == NULL) {
        m = PCM::getInstance();
    }
    m->cleanup();
    Py_RETURN_NONE;
}


static PyObject *getActiveAverageFrequency(PyObject *self, PyObject *args) {
    double activeAverageFrequency = getActiveAverageFrequency(systemStartState, systemEndState);
    return Py_BuildValue("d", activeAverageFrequency);
}

static PyObject *getActiveRelativeFrequency(PyObject *self, PyObject *args) {
    double activeRelativeFrequency = getActiveRelativeFrequency(systemStartState, systemEndState);
    return Py_BuildValue("d", activeRelativeFrequency);
}

static PyObject *getAverageFrequency(PyObject *self, PyObject *args) {
    double averageFrequency = getAverageFrequency(systemStartState, systemEndState);
    return Py_BuildValue("d", averageFrequency);
}

//static PyObject *getBytesReadFromMC(PyObject *self, PyObject *args);
//static PyObject *getBytesWrittenToMC(PyObject *self, PyObject *args);
static PyObject *getConsumedEnergyUnits(PyObject *self, PyObject *args) {
    uint64_t consumedEnergyUnits = getConsumedEnergy(systemStartState, systemEndState);
    return Py_BuildValue("K", consumedEnergyUnits);
}

static PyObject *getCycles(PyObject *self, PyObject *args) {
    uint64_t cycles = getCycles(systemStartState, systemEndState);
    return Py_BuildValue("K", cycles);
}

static PyObject *getCoreCycles(PyObject *self, PyObject *args) {
    uint32_t numCores = m->getNumCores();

    PyObject *pt = PyTuple_New(numCores);
    // TODO a proper Python error return
    if (pt == NULL) return NULL;

    for (uint32_t i = 0; i < numCores; ++i) {
        uint64_t coreCycles = getCycles(coreStartStates[i], coreEndStates[i]);
        PyObject *coreCyclesObj = Py_BuildValue("K", coreCycles);
        PyTuple_SetItem(pt, i, coreCyclesObj);
    }
    return pt;
}

//static PyObject *getDRAMClocks(PyObject *self, PyObject *args);
static PyObject *getDRAMConsumedEnergyUnits(PyObject *self, PyObject *args) {
    uint64_t dramConsumedEnergyUnits = getDRAMConsumedEnergy(systemStartState, systemEndState);
    return Py_BuildValue("K", dramConsumedEnergyUnits);
}

//static PyObject *getExecUsage(PyObject *self, PyObject *args);
//static PyObject *getInstructionsRetired(PyObject *self, PyObject *args);

static PyObject *getCoreInstructionsRetired(PyObject *self, PyObject *args) {
     uint32_t numCores = m->getNumCores();

    PyObject *pt = PyTuple_New(numCores);
    // TODO a proper Python error return
    if (pt == NULL) return NULL;

    for (uint32_t i = 0; i < numCores; ++i) {
        uint64_t coreInstrsRetired = getInstructionsRetired(coreStartStates[i], coreEndStates[i]);
        PyObject *coreInstrsObj = Py_BuildValue("K", coreInstrsRetired);
        PyTuple_SetItem(pt, i, coreInstrsObj);
    }
    return pt;

}

static PyObject *getIPC(PyObject *self, PyObject *args) {
    double ipc = getIPC(systemStartState, systemEndState);
    return Py_BuildValue("d", ipc);
}

static PyObject *getJoulesPerEnergyUnit(PyObject *self, PyObject *args) {
    double joulesPerEnergyUnit = m->getJoulesPerEnergyUnit();
    return Py_BuildValue("d", joulesPerEnergyUnit);
}

static PyObject *getL2CacheHitRatio(PyObject *self, PyObject *args) {
    double l2CacheHitRatio = getL2CacheHitRatio(systemStartState, systemEndState);
    return Py_BuildValue("d", l2CacheHitRatio);
}

static PyObject *getL2CacheHits(PyObject *self, PyObject *args) {
    uint64_t l2CacheHits = getL2CacheHits(systemStartState, systemEndState);
    return Py_BuildValue("K", l2CacheHits);
}

static PyObject *getL2CacheMisses(PyObject *self, PyObject *args) {
    uint64_t l2CacheMisses = getL2CacheMisses(systemStartState, systemEndState);
    return Py_BuildValue("K", l2CacheMisses);
}

static PyObject *getL3CacheHits(PyObject *self, PyObject *args) {
    uint64_t l3CacheHits = getL3CacheHits(systemStartState, systemEndState);
    return Py_BuildValue("K", l3CacheHits);
}

static PyObject *getL3CacheMisses(PyObject *self, PyObject *args) {
    uint64_t l3CacheMisses = getL3CacheMisses(systemStartState, systemEndState);
    return Py_BuildValue("K", l3CacheMisses);
}

static PyObject *getNominalFrequency(PyObject *self, PyObject *args) {
    uint64_t nominalFrequency = m->getNominalFrequency();
    return Py_BuildValue("K", nominalFrequency);
}

static PyObject *getNumCores(PyObject *self, PyObject *args) {
    uint32_t numCores = m->getNumCores();
    return Py_BuildValue("k", numCores);
}

static PyObject *getNumOnlineCores(PyObject *self, PyObject *args) {
    uint32_t numOnlineCores = m->getNumOnlineCores();
    return Py_BuildValue("k", numOnlineCores);
}

static PyObject *getNumSockets(PyObject *self, PyObject *args) {
    uint32_t numSockets = m->getNumSockets();
    return Py_BuildValue("k", numSockets);
}

static PyObject *getSMT(PyObject *self, PyObject *args) {
    bool smtEnabled = m->getSMT();
    if (smtEnabled) {
        Py_RETURN_TRUE;
    }
    else {
        Py_RETURN_FALSE;
    }
}

//static PyObject *getL3CacheHits(PyObject *self, PyObject *args);
//static PyObject *getL3CacheHitsNoSnoop(PyObject *self, PyObject *args);
//static PyObject *getL3CacheHitsSnoop(PyObject *self, PyObject *args);
//static PyObject *getL3CacheMisses(PyObject *self, PyObject *args);
//static PyObject *getL3CacheOccupancy(PyObject *self, PyObject *args);
//static PyObject *getL3ScalingFactor(PyObject *self, PyObject *args);
//static PyObject *getMaxIPC(PyObject *self, PyObject *args);


}
