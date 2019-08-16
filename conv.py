#!/APSshare/epd/rh6-x86_64/bin/ipython -pylab
import os
import sys

execfile('convh2imm.py')
#print sys.argv

filebase=sys.argv[1]
dset=int(sys.argv[2])

HDF_2IMM(filebase,dset)

exit()

