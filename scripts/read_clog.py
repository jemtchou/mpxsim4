#!/usr/bin/env python3
# -*- coding: utf-8 -*-
import sys
import numpy as np

import matplotlib.pyplot as plt

if __name__=='__main__':

    fname = sys.argv[1]

    frames = []

    with open(fname) as f:
        print("Open file",fname)
        px = [] #all pixels in frame
        cpx=[]  
        cl = [] #clusters in frame
        while(True):
             line = f.readline()
             if not line: 
                 break
             
             if("Frame" in line): # new frame
                 s, frameno, s2 = line.split(' ',maxsplit=2)
                 print(len(px),len(cl))
		
#                 frame = np.zeros((256,256),dtype=np.int32)
#                 ic = 0
#                 for c in cl:
#                   ic = ic+1
#                   print("=",c)
#                   for ip in c:
#                      print("x",ip,ic)
#                      print("*",np.int32(ip[0]),np.int32(ip[1]),np.int32(ic))
#                      frame[np.int32(ip[0]),np.int32(ip[1])] = np.int32(ip[2]) 	
#                 frames.append(frame) 	 
#                 print(frame)
#                 input("Press Enter to continue...")

                 print('Frame', frameno)
                 px.clear()
                 cl.clear()
                 continue

             cline = line
             while(True):
              if('[' in cline): # cluster
                 cpx.clear()
                 begin = cline.find('[')
                 end = cline.find(']')
                 if(begin != -1 and end !=-1):
                     pixel = cline[begin:end+1]
                     pixel = pixel[1:-1]
                     (x,y,z) = pixel.split(',')
                     px.append([x,y])
                     cpx.append([x,y,z])
                     cline = cline[end+1:]
              else:
                  cl.append(cpx)
                  break

     # plot frames
#    for iframe in frames[:3]:
#        print(iframe)
#       plt.matshow(iframe, cmap=plt.get_cmap('gray'))
#    plt.show()
             
