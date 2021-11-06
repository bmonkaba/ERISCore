import serial
import time
## Change this to match your local settings
SERIAL_PORT = 'COM3'
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
        return self.sp.readline()
 
    def run(self):
        while True:
            if not self.input_queue.empty():
                self.writeSerial(self.input_queue.get())         
            while (self.sp.inWaiting() > 750):
                d = self.readSerial()
                try:
                   d = d.decode()
                except:
                   d = "?" * len(d) 
                self.output_queue.put(d)


                
