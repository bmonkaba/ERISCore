import serial

## Change this to match your local settings
SERIAL_PORT = 'COM3'
SERIAL_BAUDRATE = 1500000

class SerialProcess():
 
    def __init__(self, input_queue, output_queue):
        self.input_queue = input_queue
        self.output_queue = output_queue
        self.sp = serial.Serial(SERIAL_PORT, SERIAL_BAUDRATE, timeout=1)

    def close(self):
        self.sp.close()
 
    def writeSerial(self, data):
        self.sp.write(data.encode())
        
    def readSerial(self):
        return self.sp.readline()
 
    def run(self):
        self.sp.flushInput()
        while True:
            if not self.input_queue.empty():
                data = self.input_queue.get()
                # send it to the serial device
                self.writeSerial(data)
                print ("writing to serial: " + data)

            if (self.sp.inWaiting() > 0):
                data = self.readSerial()
                #print ("reading from serial: " + str(data))
                # send it back to tornado
                self.output_queue.put(data)
