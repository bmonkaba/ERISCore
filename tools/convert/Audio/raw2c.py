import struct
import os
import random
import time
import operator
import numpy as np

#utility functions
def distance(a,b):
    #absolute diff
    d = 0
    for i in xrange(len(a)):
        d += abs(a[i] - b[i])
    return d
    
def wave_sort():
    ## similarity sort
    for i in xrange(0,len(waves_fft)-2):
        dist = 99999999999999999
        closest_index = 0
        for test in xrange(i+1,len(waves_fft)):
            #print i,test,len(waves_fft[i]),len(waves_fft[test]),len(waves_fft[i]),len(waves_fft[test])
            diff = map(operator.sub,waves_fft[i][:1024],waves_fft[test][:1024])
            diff = map(abs,diff)
            diff = sum(diff)
            if  diff < dist: 
                dist = diff                
                closest_index = test    
        #    d = distance(waves_fft[i],waves_fft[test])
        #    if (d < dist):
        #        dist = d
        #        closest_index = test
        w = waves.pop(closest_index)
        waves.insert(i+1,w)
        wf = waves_fft.pop(closest_index)
        waves_fft.insert(i+1,wf)
        if (i % 100): print i / float(len(waves)) * 100, " % complete"

#variables
file_names = []
waves = []
waves_fft = []
w = []
random.seed(time.time())

#open output files
out = open("out.txt","w")
csv = open("out.csv","w")

merge = open("merged.rdb","wb")
batch = open("build.bat","w")

#generate the batch convert file
# -f s16le -acodec pcm_s16le
# ffmpeg -i input -f s16le -c:a pcm_s16le
cmd = """ffmpeg -i ./in/"{IN_WAV}" -f f32le -c:a pcm_s16le ./out/"{OUT_RAW}"\n"""
cmd = """sox -V6 ./in/"{IN_WAV}" -c1 -b16 -e signed-integer -r 44100 -t raw ./out/"{OUT_RAW}"\n"""

#generate the batch processing script
for root, directory, files in os.walk("."):
    #for d in directory:
        for f in files:
            f = f.upper()
            if (f.find(".WAV") > 0): # and random.random() > 0.1):
                batch.write(cmd.replace("{IN_WAV}",f).replace("{OUT_RAW}",f.replace(".WAV",".raw")))
batch.write("pause\n")
batch.close()

#merge all the raw files into a single database file
print "building wave database"
for root, directory, files in os.walk("."):
    #for d in directory:
        for f in files:
            if (f.find(".raw") > 0):
                print root,directory,f
                wave_count = os.stat(root +"\\"+ f).st_size / (2048 * 2)
                print "Merging " +str(wave_count) +" waves from " + f
                for j in xrange(wave_count):
                    file_names.append(f + "," + str(wave_count) + ", "+ str(merge.tell()))
             #append the data into the merged file
                with open(root +"\\"+f, "rb") as raw:
                    merge.write(raw.read())
merge.close()

print "processing wave database..."
with open("merged.rdb", "rb") as f:
    i = 1
    byte = struct.unpack('<h', f.read(2))[0] #>h
    while byte != "":
        #print i
        #float -> w.append(byte/65535.0)
        w.append(byte)
        if i%2048 == 0:
            waves.append(w)
            w = []
        i = i + 1
        if (i%1000000 == 0): print str(i/1000000) + "MB"
        try:
            byte = struct.unpack('<h', f.read(2))[0]
        except:
            break

print "calc fft..."
for w in waves:
    ft = np.fft.rfft(w)
    if len(ft) != 1025: print len(ft)
    waves_fft.append(ft)
    

#sort order of wave tables by min summed differece
print "sorting data..."
wave_sort()
waves.reverse()
#2nd pass test
wave_sort()

#remove waves below a min differece threshold
print "pruning data below the min difference threshold..."
i = 0
prune_count = 0
while i < len(waves)-1:
    d = distance(waves[i],waves[i+1])
    print d
    if (d < 1000000):
        waves.pop(i+1)
        prune_count +=1
    i +=1
print prune_count," waves removed"

#find ranges of similarity to create wave table indicies
print "identifying wave table indexes..."
i = 0
last_dist = 999999999
wt_indexes = []
while i < len(waves)-1:
    d = distance(waves[i],waves[i+1])
    #print abs(d - last_dist)/float(last_dist)
    if (abs(d - last_dist)/float(last_dist) > 1.0):
        wt_indexes.append(i+1)
    i +=1
    last_dist = d

print wt_indexes

#create a second but this time sorted merged database
with open("sortmerged.rdb", "wb") as f:
    for wave in waves:
        f.write(struct.pack('%sh' % len(wave), *wave))



#generate output files
print "generating output files..."

v = []
count = 0
MAX_WAVES = 20000
for i in range(0,len(waves)):
    if (random.random() > (1 - (float(MAX_WAVES)/len(waves)))): #limit the number of output waves
        try:
            csv.write(file_names[i] + "," + str(waves[i]).replace('[','').replace(']','') + "\n")
            v.append("&wt_" + str(i)+"[0]")
#            out.write("const int16_t PROGMEM wt_" + str(i) + "[] = ")
#            out.write(str(waves[i]).replace('[','{').replace(']','}'))
#            out.write(';\n')
            count += 1
        except:
            pass

#out.write("const int16_t* PROGMEM wt[] = {")
#out.write(str(v)[1:-1].replace("'","") + "};\n")
out.write("const int16_t PROGMEM wt_start_index[] = " + str(wt_indexes) + ";")


out.close()
csv.close()

print str(count) + " number of waves exported"

out.close()
