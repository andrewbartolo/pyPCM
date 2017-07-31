#!/usr/bin/env python
import pyPCM

ctr = 0

pyPCM.roi_begin()

for i in xrange(9999999):
     ctr = 99 - i

print("foobar")
pyPCM.roi_end()

ipc = pyPCM.getIPC()
averageFreq = pyPCM.getAverageFrequency()
relativeFreq = pyPCM.getActiveRelativeFrequency()
aaFreq = pyPCM.getActiveAverageFrequency()
nomFreq = pyPCM.getNominalFrequency()
numCores = pyPCM.getNumCores()
numOnlineCores = pyPCM.getNumOnlineCores()
numSockets = pyPCM.getNumSockets()
cycles = pyPCM.getCycles()
jPEU = pyPCM.getJoulesPerEnergyUnit()
eu = pyPCM.getConsumedEnergyUnits()
dramEU = pyPCM.getDRAMConsumedEnergyUnits()
l2h = pyPCM.getL2CacheHits()
l2m = pyPCM.getL2CacheMisses()

print("ctr: " + str(ctr))
print("ipc: " + str(ipc))
print("averageFreq: " + str(averageFreq))
print("RelativeFreq: " + str(relativeFreq))
print("Active average freq: " + str(aaFreq))
print("Nominal freq: " + str(nomFreq))
print("NumCores: " + str(numCores))
print("NumOnlineCores: " + str(numOnlineCores))
print("NumSockets: " + str(numSockets))
print("cycles: " + str(cycles))
print("J/energy unit: " + str(jPEU))
print("EUs: " + str(eu))
print("DRAM EUs: " + str(dramEU))
print("L2 Hits: " + str(l2h))
print("L2 Misses: " + str(l2m))
