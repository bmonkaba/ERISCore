import string
import os
from PIL import Image, ImageDraw,ImageFont
from array import array


  

free_fonts = ['!PaulMaul-b.ttf', \
 '!PaulMaul.ttf', \
 'ArchitectsDaughter.ttf', \
 'Blokletters-Balpen.ttf', \
 'Blokletters-Potlood.ttf', \
 'Blokletters-Viltstift.ttf', \
 'Brizel.ttf', \
 'CamingoCode-Bold.ttf', \
 'CamingoCode-BoldItalic.ttf', \
 'CamingoCode-Italic.ttf', \
 'CamingoCode-Regular.ttf', \
 'Cascadia.ttf', \
 'Dearest.ttf', \
 'Dearest_open.ttf', \
 'Dearest_outline.ttf', \
 'ElliotSix.ttf', \
 'EraserRegular.ttf', \
 'Exotica.ttf', \
 'Flux_Architect_Bold.ttf', \
 'Flux_Architect_Bold_Italic.ttf', \
 'Flux_Architect_Italic.ttf', \
 'Flux_Architect_Regular.ttf', \
 'FreeMono.ttf', \
 'FreeMonoBold.ttf', \
 'FreeMonoBoldOblique.ttf', \
 'FreeMonoOblique.ttf', \
 'FreeSans.ttf', \
 'FreeSansBold.ttf', \
 'FreeSansBoldOblique.ttf', \
 'FreeSansOblique.ttf', \
 'FreeSerif.ttf', \
 'FreeSerifBold.ttf', \
 'FreeSerifBoldItalic.ttf', \
 'FreeSerifItalic.ttf', \
 'GONGN___.ttf', \
 'Gaegu-Bold.ttf', \
 'Gaegu-Light.ttf', \
 'Gaegu-Regular.ttf', \
 'GenzschEtHeyse.ttf', \
 'GenzschEtHeyseAlt.ttf', \
 'GermaniaOne-Regular.ttf', \
 'GloriaHallelujah.ttf', \
 'Hack-Bold.ttf', \
 'Hack-BoldItalic.ttf', \
 'Hack-Italic.ttf', \
 'Hack-Regular.ttf', \
 'IndieFlower.ttf', \
 'JINKY.ttf', \
 'Kalam-Bold.ttf', \
 'Kalam-Light.ttf', \
 'Kalam-Regular.ttf', \
 'Kingthings_Gothique.ttf', \
 'Kingthings_Spikeless.ttf', \
 'Leafy-Glade.ttf', \
 'Leafy_Shade.ttf', \
 'LiberationMono-Bold.ttf', \
 'LiberationMono-BoldItalic.ttf', \
 'LiberationMono-Italic.ttf', \
 'LiberationMono-Regular.ttf', \
 'MountainsofChristmas.ttf', \
 'Note_this.ttf', \
 'OvertheRainbow.ttf', \
 'PermanentMarker.ttf', \
 'PlaytimeOblique.ttf', \
 'PlaytimeWithHotToddies3D.ttf', \
 'RAPSCALL.ttf', \
 'Recursive_VF_1.078.ttf', \
 'Redressed.ttf', \
 'RockSalt.ttf', \
 'Scratch_.ttf', \
 'Snickles.ttf', \
 'SpaceMono-Bold.ttf', \
 'SpaceMono-BoldItalic.ttf', \
 'SpaceMono-Italic.ttf', \
 'SpaceMono-Regular.ttf', \
 'TrashHand.ttf', \
 'TungusFont_Tinet.ttf', \
 'UnifrakturMaguntia.ttf', \
 'UnifrakturMaguntia16.ttf', \
 'UnifrakturMaguntia17.ttf', \
 'UnifrakturMaguntia18.ttf', \
 'UnifrakturMaguntia19.ttf', \
 'UnifrakturMaguntia20.ttf', \
 'UnifrakturMaguntia21.ttf', \
 'Wagnasty.ttf', \
 'belligerent.ttf', \
 'hetilica.ttf', \
 'journal.ttf', \
 'justanotherhand-regular.ttf', \
 'luximb.ttf', \
 'luximbi.ttf', \
 'luximr.ttf', \
 'luximri.ttf', \
 'playtime.ttf', \
 'rothenbg.ttf', \
 'teutonic1.ttf', \
 'teutonic2.ttf', \
 'teutonic3.ttf', \
 'teutonic4.ttf', \
 'zenda.ttf' \
]

TEXT_FONT_SIZE = 72

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

def GenTextTag(font_ttf,text,out_file,font_size):
    kearning_table = []
    kearning_values = []
    print (out_file)
    fnt = ImageFont.truetype(font_ttf, font_size)
    
    #build kerning table test every a + b char combination
    c_list = list(string.digits + string.ascii_letters + string.punctuation)
    #print(c_list)
    for a in c_list:
        for b in c_list:
            #kw, kh = img.textsize(a+b, font=fnt)
            asize = fnt.getsize(a)[0]
            bsize = fnt.getsize(b)[0]
            ab_size = fnt.getsize(a+b)[0]
            kw = (asize+bsize) - ab_size
            kearning_table.append([a+b,int(kw)])
            kearning_values.append(int(kw))
            
    #get the most common kerning value and make it the default
    from collections import Counter
    default = Counter(kearning_values).most_common(1)[0][0]
    
    #print(default)
    #print(Counter(kearning_values).most_common(10))
    
    #
    #reconstruct the table with a default median value and only the deviations
    #
    
    #build an intermediate table excluding the default value
    k = []
    for record in kearning_table:
        if (record[1] != default):
            k.append(record)
    
    #scan the intermediate table to build the constant kerned chars table
    const_kern = []
    for c in c_list:
        char_kern = [] #extracted kerning value container
        char_kern_values = []
        if len(k) > 1:
            for record in k:
                if len(record[0]) != 2:
                    #print(record)
                    pass
                elif(record[0][1] == c): #if the second char in the record matches
                    char_kern.append(record)
                    char_kern_values.append(record[1])
            #get the top ten histogram, if the result is 1 then the kerning is common
            #print(char_kern_values)
            common = Counter(char_kern_values).most_common(10)
            is_common = True
            if len(common) != 1:
                is_common = False
            
            if is_common:
                #in the first record  [0], 
                #of the first char pair list item  [0]
                #replace the first char with * [0]
                #print(char_kern[1])
                try:
                    sl = char_kern[0][0]
                except:
                    print("ERROR")
                    print(char_kern)
                sl = '*' + sl[1]
                char_kern[0][0] = sl
                const_kern.append(char_kern[0])
    
    if len(const_kern) > 0:
        print("constants:" + str(const_kern))
        
    #now with the constants available, remove them from the kerning table
    final = []
    for record in k:
        found = False
        for const_record in const_kern:
            #print(const_record[0][1])
            if const_record[0][1] == record[0][1]:
                found = True
        if not found:
            final.append(record)
    final =  [["default",default]] + const_kern + final
        
    kearning_table =final
    
    #convert the table to a easy to read format
    kearning_table = kearning_table
    
    digraphs = ['th', 'he', 'an', 'in', 'er', 'on', 're', 'ed', 'nd', 'ha', 'at', 'en', 'es', 'of', 'nt', 'ea', 'ti', 'to', 'io', 'le', 'is', 'ou', 'ar', 'as', 'de', 'rt', 've']
    
    #move the most common digraphs to the top of the list
    for i in range(len(kearning_table)):
        if kearning_table[i][0] in digraphs:
            kearning_table.insert(1, kearning_table.pop(i))

    
    #del(fnt)

    #build char start index
    #fnt = ImageFont.truetype(font_ttf, font_size)

    startx = 0
    for t in text:
        startx = startx + fnt.getsize(t)[0]
        
    width = startx
    height = fnt.getsize(string.printable)[1]
    
    img = Image.new('RGB', (width, height+font_size), color = (0, 0, 0))
    d = ImageDraw.Draw(img)
    startx = 0
    start_char_index = []
    for t in text:
        #d.line([(startx,0),(startx,1000),(startx,0)], fill ="green", width = 1)
        d.text((startx,font_size/2), t, font=fnt, fill=(255,255,255))
        start_char_index.append([t,startx,startx + fnt.getsize(t)[0]])
        startx = startx + fnt.getsize(t)[0]
        #d.line([(startx,0),(startx,1000),(startx,0)], fill ="red", width = 1)
    #d.line(xy, fill ="green", width = 1)
    #d.line(xy_end, fill ="yellow", width = 1)
    img.save(out_file)
    del(fnt)
    del(img)
    del(d)



  #save the kearning table
    f = open(out_file[:-4]+".krn",'w')
    #write the start index (width) for each char
    f.write("SPRITE_X_POS_START_END\n")
    for record in start_char_index:
        f.write(record[0] + " " + str(record[1]) + " " + str(record[2]) + "\n")
    #write the kearning table
    f.write("KEARNING\n")
    for record in kearning_table:
        if(record[0].find("\n")==-1) and record[0].find("\r")==-1:
            f.write(record[0] + " " + str(record[1]) + "\n")
    f.write("end\n")
    f.close()

    
    return

def GenTextProof(sprite_font,kern_file,text):
    #generate the visual inspection proof of function evidence
    print(sprite_font)
    print(kern_file)
    print(text)
    #load sprite font
    im = Image.open(sprite_font).convert("RGB")
    #load kerning file
    k = {}
    f = open(kern_file,'r')
    #load the sprite indexes
    sprite_x = {}
    r = f.readline() #SPRITE_X_POS_START_END
    r = f.readline() #first record
    while r.find("KEARNING") == -1:
        sp = r.split(" ")
        if(len(sp)==1):
            sp.append("0")
            sp.append("0\n")
        if(len(sp)==4):
            #this is the space char
            sp.pop(0)
            sp[0] = " "
        #print(sp)   
        sprite_x.update({sp[0]:[int(sp[1]),int(sp[2].strip())]})
        r = f.readline()
        
        
        
        
    #get the default kern value
    default_text, default_value = f.readline()[:-1].split(" ")
    
    k.update({default_text:default_value})
    #and now get the pairs
    for line in f:
        #print(line)
        if line.find("end")==0:
            break
        pair = line[0:2]
        kern = int(line[3:-1])
        k.update({pair:kern})
        
        
    #print(k)
    #walk the string and get the kerning values
    #blt the proof of function string using the loaded kerning values
    #save the evidence
    return

if __name__ == "__main__":
    fonts = []
    # include any selected system installed fonts
    #
    #fonts = [r"C:\Users\brian\AppData\Local\Microsoft\Windows\Fonts\HEAVY_DATA.TTF", \
    #         r"C:\Users\brian\AppData\Local\Microsoft\Windows\Fonts\MajorMonoDisplay-Regular.ttf", \
    #         r"C:\Users\brian\AppData\Local\Microsoft\Windows\Fonts\RobotoMono-VariableFont_wght.ttf", \
    #         r"C:\Users\brian\AppData\Local\Microsoft\Windows\Fonts\RobotoMono-Italic-VariableFont_wght.ttf" \
    #         ]
    
    fonts = fonts + free_fonts
    
    prefix = "D:\\brian\\Dev\\Eris\\ERISCore\\tools\\convert\\TTF_SOURCE\\" 
    fonts = os.listdir(prefix)
    
    for font in fonts:
        if(font.find("C:") == -1):        
            #font = prefix + font 
            font = prefix + font 
        
        #wrangle the font full file path to a friendly consistent file prefix for the output files
        f = font.split("\\")[-1].split(".")[0].replace("_","-")
        path = ".\\FONTS\\" + f
        print(path)
        try:
            os.mkdir(path)
        except:
            #folder prob already exists
            pass
        somekeywords = [string.printable.replace("\n","")]
        
        #create number text tags for the UI
        #for TEXT_FONT_SIZE in range(10,64,2):
        #    for i in range(0,300):    
        #        GenTextTag(font,str(i),".\\NUMBERS\%s_%02d_%02d.png"%(f,TEXT_FONT_SIZE,i),TEXT_FONT_SIZE) 
        
        for akey in somekeywords:
            for font_size in range(8,TEXT_FONT_SIZE+1,1):
                #print( "Generating text tag for " + akey)
                GenTextTag(font,akey,"%s\%02d.png"%(path,font_size),font_size)
                
        #generate proof file
        GenTextProof("%s\%02d.png"%(path,TEXT_FONT_SIZE), "%s\%02d.krn"%(path,TEXT_FONT_SIZE),"The Quick Brown Fox Jumps Over The Lazy Dog. {!1234.56789} #proof_file")
    
    #
    # Convert png graphics to ile (inline 565 bitmap encoding)
    #
    dstruct = {}    
    for root, directory, files in os.walk(".\\FONTS\\"):
        #for d in directory:
        for f in files:
            fp = root + "\\" + f
            if fp[-4:] == ".png":
                print (fp)
                ppm = fp[:-4] + ".ppm"
                ile = fp[:-4] + ".ile"
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
                    size = 64,64
                #print ppm
                #print ile
                #print size
                im = Image.open(fp).convert("RGB")
                #im.thumbnail(size, Image.ANTIALIAS)
                #im = im.resize(size, Image.BILINEAR)
                im.save(ppm)
                #convert to custom 565 data stream format
                #Gen565Stream("test.ppm","test.ile")
                Gen565Stream(ppm,ile)
                #now delete the ppm file
                os.remove(ppm)                
                #print "done."