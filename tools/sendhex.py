#!/usr/bin/env python

import asyncio
import websockets
import os
import time


filename = ".\\.pio\\build\\teensy41\\firmware.hex"


async def hello():
    uri = "ws://75.118.234.229:3271" #8765
    async with websockets.connect(uri) as websocket:
        f = open(filename,'r') #\.pio\build\teensy41\firmware.hex
        firmware_hex = f.read()
        await websocket.send(firmware_hex)
        #print("> Uploading..")
        response = await websocket.recv()
        if response == firmware_hex:
            print(time.ctime() + " > FILE UPLOAD OK")
        else:
            print(time.ctime() + " > NOT OK")


print ("SendHex Remote Transfer Utility")
print ("Monitoring " + filename + " for changes\n\n")

last_timestamp = 0
while 1:
    time.sleep(2)
    try:
        timestamp = os.stat(filename).st_mtime
        if last_timestamp - timestamp != 0:
            print(time.ctime() + " > Sending updated file to the remote target")
            asyncio.get_event_loop().run_until_complete(hello())
            last_timestamp = timestamp
    except:
        pass
    