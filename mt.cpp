#include <cpucounters.h>
#include <pthread.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>


using namespace std;

static long nThreads = 0;
static long nIters = 0;

static SystemCounterState systemStartState;
std::vector<SocketCounterState> socketStartStates; // currently never used by Python
std::vector<CoreCounterState> coreStartStates;

static SystemCounterState systemEndState;
std::vector<SocketCounterState> socketEndStates; // currently never used by Python
std::vector<CoreCounterState> coreEndStates;


void die(int errorCode) {
    printf("usage: mt <num_threads> <num_iterations>\n");
    exit(errorCode);
}

void workerFn(void *input) {
    volatile uint64_t ctr = 0;
    for (uint64_t i = 0; i < nIters; ++i) {
        ++ctr;
    }
    printf("Worker %i done (count = %zu).\n", (long)input, ctr);
}

int main(int argc, char *argv[]) {
    if (argc != 3) die(0);

    nThreads = atol(argv[1]);
    nIters = atol(argv[2]);

    if (nThreads <= 0 || nIters <= 0) die(0);

    pthread_t threads[nThreads];

    PCM *m = PCM::getInstance();
    m->resetPMU();
    if (m->program() != PCM::Success) return -1;

    //SystemCounterState before_sstate = getSystemCounterState();
    m->getAllCounterStates(systemStartState, socketStartStates, coreStartStates);


    for (long i = 0; i < nThreads; ++i) {
        int ret = pthread_create(&threads[i], NULL, (void* (*)(void*))workerFn, (void *)i);
        if (ret) die(1);
    }

    for (long i = 0; i < nThreads; ++i) {
        pthread_join(threads[i], NULL);
    }

    //SystemCounterState after_sstate = getSystemCounterState();
    m->getAllCounterStates(systemEndState, socketEndStates, coreEndStates);

    for (int i = 0; i < 40; ++i) printf("-");
    printf("\n");

    cout << "IPC: " << getIPC(systemStartState, systemEndState) << endl;
    cout << "L3 cache hit ratio: " << getL3CacheHitRatio(systemStartState, systemEndState) << endl;
    cout << "Core 0 cycles: " << getCycles(coreStartStates[0], coreEndStates[0]) << endl;
    cout << "Core 0 instrs retired: " << getInstructionsRetired(coreStartStates[0], coreEndStates[0]) << endl;


    printf("All workers complete.\n");
    pthread_exit(NULL);

    return 0;
}
