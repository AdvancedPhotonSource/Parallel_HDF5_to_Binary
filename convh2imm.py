
import h5py
import sys
import numpy 
import os



def listkeys(pathstr,hitem,level,maxlevel):
    
    print ' '
    
    print "%s "%(pathstr)
    
    print "    %s"%(type(hitem))
    print "    Attributes:"
    
    try:
        klist = hitem.attrs.keys()
    
    	for k in klist:
	    print "    %s \t\t %s"%(k,hitem.attrs[k])
    
    except:
        pass
	
	
    if level==maxlevel:
        return
    
    try:
    
    	klist=hitem.keys()
	
	for k in klist:
	    strx=pathstr+k
	    strx=strx+'/'
	    listkeys(strx,hitem[k],level+1,maxlevel)
	
	
    except:
        pass;
    




def HDF_List(filename,maxlevel):
	hdffile_r = h5py.File(filename,'r')


	listkeys('/',hdffile_r,0,maxlevel)
	    
	hdffile_r.close()



def HDF_SetNames(filename):
    hdffile_r = h5py.File(filename,'r')

    klist= hdffile_r['entry'].keys()

    dlist=[]
    for k in klist:
	if k[0:4]=='data':
	    image_nr_high = hdffile_r['entry'][k].attrs['image_nr_high']
	    image_nr_low = hdffile_r['entry'][k].attrs['image_nr_low']
	    setnum=int(k[5:])
	    nimgs=1 + image_nr_high - image_nr_low
	    dlist.append( (k,setnum,image_nr_low,image_nr_high,nimgs ) )



    hdffile_r.close()


    return(dlist)

def HDF_ImgSpecs(filename):
	hdffile_r = h5py.File(filename,'r')
	
	
	nimages=hdffile_r['entry']['instrument']['detector']['detectorSpecific']['nimages'][0]
	
	sizex=hdffile_r['entry']['instrument']['detector']['detectorSpecific']['x_pixels_in_detector'][0]
	sizey=hdffile_r['entry']['instrument']['detector']['detectorSpecific']['y_pixels_in_detector'][0]
	
	ans={'nimages':nimages , 'sizex':sizex , 'sizey':sizey}
	
	hdffile_r.close()
	return(ans)
	
	
	

def HDF_2IMM(filebase,setnumber):
	filename = '%s%s'%(filebase,'_master.h5')
	specs=HDF_ImgSpecs(filename)

	maximages = specs['nimages']
	dsets=HDF_SetNames(filename)
	#sort in order of first image in each set
 	dsets = sorted(dsets,key=lambda x:x[2] )
	
	
	# this block is if we wish to convert one dataset in the h5 file.
	if setnumber>=0:
	    #make sure the dataset is in the file
  	    notfound = 1;	
	    for dset in dsets:
   	        snum=dset[1]

	        if snum==setnumber:
		  notfound=0;
		  nimages=dset[4]

	    if notfound==1:
	        print "Non extant dataset"
	        return

	    #convert 1 data set
	    datafile = '%s_data_%06d.h5'%(filebase,setnumber)
	    immfile = '%s_d%06d'%(filebase,setnumber)
	    #command = './h52imm %s %s data %d %d %d 1 0 wb %d 1'%(datafile,immfile,nimages,specs['sizex'],specs['sizey'],nimages,)
	    command = './h52immsh %s %s %d %d %d %s %d %d'%(datafile,immfile,nimages,specs['sizex'],specs['sizey'],'wb',nimages,1)
	    print command
	    os.system(command)
	    return
	
	#here if we are converting the whole file all datasets
	fileperm='wb'
	for dset in dsets:
	    snum=dset[1]
	    nimages=dset[4]
	    firstimg = dset[2] + 1
	    
	    #convert 1 data set
	    datafile = '%s_data_%06d.h5'%(filebase,snum)
	    immfile = '%s'%(filebase)
	    #command = './h52imm %s %s data %d %d %d 1 0 %s %d %d'%(datafile,immfile,nimages,specs['sizex'],specs['sizey'],fileperm,maximages,firstimg)
	    command = './h52immsh %s %s %d %d %d %s %d %d'%(datafile,immfile,nimages,specs['sizex'],specs['sizey'],fileperm,maximages,firstimg)

	    print command
	    os.system(command)
	    fileperm='ab'
	    
	  
	
	
	
	
	
