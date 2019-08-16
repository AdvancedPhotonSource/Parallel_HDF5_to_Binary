#!/APSshare/epd/rh6-x86_64/bin/ipython -pylab
import os
import sys

execfile('convh2imm.py')
#print sys.argv

filebase=sys.argv[1]

print filebase

HDF_List(filebase,3)

exit()

