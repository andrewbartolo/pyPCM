#!/usr/bin/env python
import pyPCM

pyPCM.roi_begin()

ctr = 0
for i in xrange(9999):
     ctr = 99 - i


pyPCM.roi_end()

ipc = pyPCM.getIPC()
print("ctr: " + str(ctr))
print("ipc: " + str(ipc))
