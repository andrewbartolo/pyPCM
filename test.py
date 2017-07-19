#!/usr/bin/env python
import pyPCM

ctr = 0

pyPCM.roi_begin()

for i in xrange(9999):
     ctr = 99 - i

print("foobar")
pyPCM.roi_end()

ipc = pyPCM.getIPC()
averageFreq = pyPCM.getAverageFrequency()
cycles = pyPCM.getCycles()
jPEU = pyPCM.getJoulesPerEnergyUnit()
eu = pyPCM.getConsumedEnergyUnits()
dramEU = pyPCM.getDRAMConsumedEnergyUnits()
print("ctr: " + str(ctr))
print("ipc: " + str(ipc))
print("averageFreq: " + str(averageFreq))
print("cycles: " + str(cycles))
print("J/energy unit: " + str(jPEU))
print("EUs: " + str(eu))
print("DRAM EUs: " + str(dramEU))
