import os
from PIL import Image, ImageDraw,ImageFont
from array import array


  

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
        f.readline() #get bit depth
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
    #print (text + " " + out_file)
    img = Image.new('RGB', (320, font_size), color = (0, 0, 0))    
    fnt = ImageFont.truetype(r"C:\Users\brian\AppData\Local\Microsoft\Windows\Fonts\HEAVY_DATA.TTF", font_size)    
    d = ImageDraw.Draw(img)
    d.text((0,0), text, font=fnt, fill=(255,255,255))
    img.save(out_file)
    return
    
if __name__ == "__main__":

    
    #
    # Convert png graphics to ile (inline 565 bitmap encoding)
    #
    manifest = open("manifest.txt","w") #save a list of converted files
    dstruct = {}    
    for root, directory, files in os.walk(".\\SMART_CONVERT\\"):
        #print(directory)
        for f in files:
            fp = root + "\\" + f
            if fp[-4:] == ".png":
                #print (fp)
                ppm = fp[:-4] + ".ppm"
                ile = fp[:-4] + ".ile"
                #batch convert images with PIL
                #print ppm
               #print ("\t"+ile)
                #print size
                try:
                    im = Image.open(fp).convert("RGB")
                    skip = False
                except:
                    skip = True
                
                if not skip: 
                    width = im.width
                    height = im.height
                    if (width < 64 and height < 64):
                        ratio = 1
                    elif (width < 128 and height < 128):
                        width = int(width/2)
                        height = int(height/2)
                    elif (width < 800 and height < 600):
                        ratio = 240/height
                        width = int(width * ratio)
                        height = int(height * ratio)
                    else:
                        ratio = 0.25
                        width = int(width * ratio)
                        height = int(height * ratio)
                        
                    if ratio != 1:
                        im = im.resize((width,height), Image.BILINEAR)
                    #im.thumbnail(size, Image.ANTIALIAS)
                    #im = im.resize(size, Image.BILINEAR)
                    zoom_out = 1
                    while True:
                        im.save(ppm)
                        Gen565Stream(ppm,ile)
                        manifest.write(ile + "\n")
                        os.remove(ppm)
                        if width > 16:
                            zoom_out = zoom_out * 2
                            ppm = fp[:-4] + "_nz" + str(zoom_out) + ".ppm"
                            ile = fp[:-4] + "_nz" + str(zoom_out) + ".ile"
                            width = int(width/zoom_out)
                            height = int(height/zoom_out)
                            if width < 4:
                                break
                            if height < 4:
                                break
                            im = im.resize((width,height), Image.BILINEAR)
                        else:
                            break
    manifest.close()
                   