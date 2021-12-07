import serial
import unicodedata
import base64
import lz4.block

## Change this to match your local settings
SERIAL_PORT = 'COM6'
SERIAL_BAUDRATE = 1500000

class SerialProcess():
    def __init__(self, input_queue, output_queue):
        
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
                #print("b64> ")
                d = base64.b64decode(d,validate=False)
                #print (d)
    
            if (d.find("LZ4") == 0):
                #print("lz4> " + d)
                sp = d.split(" ")
                size = int(sp[1])
                d = ''.join(sp[2:])
                d = base64.b64decode(d,validate=False)
                
                #print("decode> ")
                #print(d)
                #print(size)
                d = lz4.block.decompress(d, uncompressed_size=size)
                #print("decompress> ")
                #print(d)
                d = d.decode()
                
            try:
                if (len(d) > 1):
                   self.output_queue.put(d)
            except:
               d = "?" * len(d) 
               print (d)
            
            if (d.find("M ") == 0 or d.find("LS ") == 0 or d.find("F") == 0):
                d = ''.join(s for s in d if unicodedata.category(s)[0]!="C")
                
            



                
