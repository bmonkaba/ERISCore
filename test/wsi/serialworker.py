import serial
import time
## Change this to match your local settings
SERIAL_PORT = 'COM6'
SERIAL_BAUDRATE = 1500000

class SerialProcess():
    def __init__(self, input_queue, output_queue):
        
        self.input_queue = input_queue
        self.output_queue = output_queue
        self.sp = serial.Serial(SERIAL_PORT, SERIAL_BAUDRATE, timeout=0)

    def close(self):
        self.sp.close()
 
    def writeSerial(self, data):
        self.sp.write(data.encode())
        
    def readSerial(self):
        try:
            return self.sp.readline()
        except:
            return ""
                
         
 
    def run(self):
        while True:
            if not self.input_queue.empty():
                d = self.input_queue.get()
                #print ("writing to serial port: "+ d)
                #print("< " + d[:78])
                self.writeSerial(d)         
            
            d = self.readSerial()
            while (len(d)>0):
                try:
                   d = d.decode()
                except:
                   d = "?" * len(d) 
                if (d.find("M ") >= 0): 
                    print("> " + d[:78].strip('\n'))
                self.output_queue.put(d)
                d = self.readSerial()


                
