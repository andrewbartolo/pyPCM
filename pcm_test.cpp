#include <cpucounters.h>
#include <stdio.h>
#include <iostream>

using namespace std;

int main(int argc, char *argv[]) {

    PCM *m = PCM::getInstance();
    if (m->program() != PCM::Success) return -1;

    SystemCounterState before_sstate = getSystemCounterState();

    // hope this don't get optimized out...
    /*
    size_t sz = 99999999;
    int *foovec = (int *)malloc(sz * sizeof(int));
    for (int i = 0; i < sz; ++i) {
        foovec[i] = 99 - i;
    }
    */

    SystemCounterState after_sstate = getSystemCounterState();

    for (int i = 0; i < 40; ++i) printf("-");
    printf("\n");

    cout << "IPC: " << getIPC(before_sstate, after_sstate) << endl;
    cout << "L3 cache hit ratio: " << getL3CacheHitRatio(before_sstate, after_sstate) << endl;

    printf("hi\n");

    return 0;
}
