#Copyright 2022 Brian Monkaba
#Permission is hereby granted, free of charge, to any person obtaining a copy 
#of this software and associated documentation files (the "Software"), to deal 
#in the Software without restriction, including without limitation the rights 
#to use, copy, modify, merge, publish, distribute, sublicense, and/or sell 
#copies of the Software, and to permit persons to whom the Software is 
#furnished to do so, subject to the following conditions: The above copyright 
#notice and this permission notice shall be included in all copies or 
#substantial portions of the Software. 
#THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, 
#EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF 
#MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. 
#IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, 
#DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR 
#OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE 
#USE OR OTHER DEALINGS IN THE SOFTWARE.
###


import serial
import unicodedata
import base64
import lz4.block

SERIAL_BAUDRATE = 5500000
SERIAL_PORT = ''

class SerialProcess():
    def __init__(self, serial_port, input_queue, output_queue):
        SERIAL_PORT = serial_port
        self.input_queue = input_queue
        self.output_queue = output_queue
        self.sp = serial.Serial(SERIAL_PORT, SERIAL_BAUDRATE, timeout=5000)

    def close(self):
        self.sp.close()
 
    def writeSerial(self, data):
        e = data.encode()
        if e != None and len(e) > 1:
            self.sp.write(e)
        
    def readSerial(self):
        try:
            return self.sp.readline()
        except:
            return ""
                
         
 #lz4.block.decompress(base64.b64decode("gExaNF9FTkQN"),8)
 #Out[35]: b'LZ4_END\r'
 
    def run(self):
        while True:
            if not self.input_queue.empty():
                d = self.input_queue.get()
                #print ("writing to serial port: "+ d)
                print("< " + d[:78].strip('\n'))
                self.writeSerial(d)         
            
            d = self.readSerial()
            #print(d)
            
            try:
                msg_len = len(d)
                d = d.decode()
              
            except:
                msg_len = 1
                if(len(d) > 0):
                    print("First ring decode failure on MSG:")
                    print (d)
                    d = ""
            
            
            if (d.find("LZ4") == 0):
                #print("lz4> " + d)
                sp = d.split(" ")
                try:
                    size = int(sp[1])
                except:
                    self.sp.close()
                    self.sp = serial.Serial(SERIAL_PORT, SERIAL_BAUDRATE, timeout=5000)
                    
                d = ''.join(sp[2:])
                try:
                    d = base64.b64decode(d,validate=False)
                except:
                    print("base64.b64decode err")
                    break
                if (msg_len/size)> 1.0:
                    print("decode> ")
                    print(d[:16])
                    print(size)
                    print(msg_len)
                    print(msg_len/size)
                try:
                    d = lz4.block.decompress(d, uncompressed_size=size)
                except:
                    print("lz4.block.decompress err")
                    print (d)
                else:
                    #print("decompress> ")
                    #print(d)
                    try:
                        d = d.decode()
                    except:
                        print("WRN: bad lz4 block")
                        print(d)
                
            try:
                if (len(d) > 1 ):#and isinstance(d,str)
                   self.output_queue.put(d)
            except:
               d = "?" * len(d) 
               print (d)
            
            try:
                if (d.find("M ") == 0 or d.find("LS ") == 0 or d.find("F") == 0):
                    d = ''.join(s for s in d if unicodedata.category(s)[0]!="C")
                    #print (d)
            except:
                pass
            
                
            



                
