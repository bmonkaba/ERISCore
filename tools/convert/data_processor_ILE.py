import os
from PIL import Image, ImageDraw,ImageFont
from array import array
import json

  

TEXT_FONT_SIZE = 36

#convert 8 bit r,g,b values into 16 bit 565 format 
def color565(r,g,b):
    return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);

 
#create a 565 packed bitmap stream file 
#from a bin  ppm file exported from gimp
def Gen565Stream(in_file,out_file):
    in_data = array('B')    
    with open(in_file, 'rb') as f:
        #print in_file        
        f.readline() #magic
        l = f.readline() #may contain gimp comment
        if (l[0] == "#"): #if comment get next line (dimensions)
            dim = f.readline() #
        else:               #if not a comment then this line is the dimensions
            dim = l
        #print 
        bits = f.readline() #get bit depth
        #print bits
        dim = dim.decode("utf-8")
        x = int(dim.split(' ')[0])
        y = int(dim.split(' ')[1])
        in_data.fromfile(f,x*y*3)     #read 8 bit (R,G,B) per pixel data in
    out_data = array("H",[0] * x*y)       #create a 16 bit output array
    #print len(in_data), x * y * 3
    for i in range(0,len(in_data),3):
        r = in_data[i]  #R       B
        g = in_data[i + 1] #G    R
        b = in_data[i + 2] #B    G
        index = int(i/3)
        out_data[index] = color565(r,g,b)
    with open(out_file, 'wb') as f:
        f.write('ILE565\n'.encode('utf-8')) #image little endian 565 formated data
        f.write(dim.encode('utf-8'))      #width height dimensions      
        f.write(out_data) #565 packed data 
    #print len(out_data), x * y
    #print str(out_data[1])
    return


def GenTextTag(text,out_file,font_size=64):
    print (text + " " + out_file)
    img = Image.new('RGB', (320, font_size), color = (0, 0, 0))    
    fnt = ImageFont.truetype('C:\\Windows\\FontsFonts\\heavy_data.ttf', font_size)    
    d = ImageDraw.Draw(img)
    d.text((0,0), text, font=fnt, fill=(255,255,255))
    img.save(out_file)
    return
    
if __name__ == "__main__":

    
    #
    # Convert png graphics to ile (inline 565 bitmap encoding)
    #
    dstruct = {}    
    for root, directory, files in os.walk(".\\INPUT\\"):
        #for d in directory:
        for f in files:
            fp = root + "\\" + f
            if fp[-4:] == ".png":
                print (fp)
                ppm = fp[:-4] + ".ppm"
                ile = fp[:-4] + ".ile"
                ile = ile.replace("ezgif-frame","burn")
                #batch convert images with PIL
                if fp.find("\\V\\") > -1:
                    img_index = f.split("_")[1]
                    while(len(img_index) < 3):
                        img_index = "0" + img_index 
                        
                    ile = root + "\\" + img_index + ".ile"
                    size = 320,240
                    if dstruct.has_key(root) == True:
                        dstruct[root] = dstruct[root] + 1
                    else:
                        dstruct.update({root:0})                      
                elif fp.find("\\W\\") > -1:
                    size = 320,240
                elif fp.find("\\M\\") > -1:
                    size = 1664,64
                elif fp.find("\\L\\") > -1:
                    size = 16,16
                elif fp.find("\\T\\") > -1:
                    size = 320,TEXT_FONT_SIZE
                else:
                    size = 320,240
                #print ppm
                #print ile
                #print size
                im = Image.open(fp).convert("RGB")
                #im.thumbnail(size, Image.ANTIALIAS)
                im = im.resize(size, Image.BILINEAR)
                im.save(ppm)
                #convert to custom 565 data stream format
                #Gen565Stream("test.ppm","test.ile")
                Gen565Stream(ppm,ile)
                #now delete the ppm file
                os.remove(ppm)                
                #print "done."
    f = open(".\\INPUT\\vdo_manifest.txt",'w')
    #f.write(json.dumps(dstruct))
    f.write(str(len(dstruct.keys())) + "\n")
    for key in dstruct.keys():
        f.write(key[1:].replace("\\","/") + "\n")
        f.write(str(dstruct[key]) + "\n")
    f.close()
    