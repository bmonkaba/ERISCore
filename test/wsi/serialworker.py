import serial
import unicodedata
import base64
import lz4.block

SERIAL_BAUDRATE = 1500000
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
        if e != None:
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
            
            try:
                d = d.decode()
              
            except:
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
                
                #print("decode> ")
                #print(d)
                #print(size)
                try:
                    d = lz4.block.decompress(d, uncompressed_size=size)
                except:
                    print("lz4.block.decompress err")
                    print (d)
                #print("decompress> ")
                #print(d)
                else:
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
            except:
                pass
            
                
            



                
