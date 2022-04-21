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


import sys
sys.dont_write_bytecode = True

import tornado.httpserver
import tornado.ioloop
import tornado.web
import tornado.websocket
import tornado.gen
from tornado.options import define, options

import multiprocessing
import threading
multiprocessing.set_start_method('spawn')

import serialworker

define("port", default=8088, help="run on the given port", type=int)
 
clients = [] 

input_queue = multiprocessing.Queue()
output_queue = multiprocessing.Queue()
vm_input_queue = multiprocessing.Queue()
vm_output_queue = multiprocessing.Queue()
 
class IndexHandler(tornado.web.RequestHandler):
    def get(self):
        self.render('index.html')

class StaticFileHandler(tornado.web.RequestHandler):
    def get(self):
        self.render('main.js')
 
class WebSocketHandler(tornado.websocket.WebSocketHandler):
    def open(self):
        print ('new connection')
        clients.append(self)
        self.write_message("M WebSocket Server: Connected to the serial bridge")
 
    def on_message(self, message):
        #print ('tornado received from client: %s' % json.dumps(message))
        #self.write_message('ack')
        input_queue.put(message)
 
    def on_close(self):
        print ('connection closed')
        clients.remove(self)

## check the queue for pending messages, and rely that to all connected clients
def checkQueue():
    message = ""
    while not output_queue.empty():
        message += output_queue.get()
    if len(message) > 0:
        for c in clients:
            c.write_message(message)
        message = ""
    while not vm_output_queue.empty():
        message += vm_output_queue.get()
    if(len(message) > 0):
        for c in clients:
            #message = "VM "+ message
            c.write_message(message)

if __name__ == '__main__':
    print("Eris Audio System Test Tool Web Server")
    print("warning: this app is running an open access local static webserver and USB serial websocket")
    ###
    ## start the serial worker in background (as a deamon)
    sp = serialworker.SerialProcess('/dev/cu.usbmodem110967101',input_queue, output_queue)
    x = threading.Thread(target=sp.run)
    x.start()

    #connection to the virtual machine
    #sp = serialworker.SerialProcess('COM10',vm_input_queue, vm_output_queue)
    #x = threading.Thread(target=sp.run)
    #x.start()

    #sp.daemon = True
    #sp.start()
    ###"""
    
    tornado.options.parse_command_line()
    app = tornado.web.Application(
        handlers=[
            (r"/", IndexHandler),
            (r"/static/(.*)", tornado.web.StaticFileHandler, {'path':  './'}),
            (r"/ws", WebSocketHandler)
        ],
        debug=True
    )
    httpServer = tornado.httpserver.HTTPServer(app)
    httpServer.listen(options.port)
    print ("Now listening on port:", options.port)

    mainLoop = tornado.ioloop.IOLoop.instance()
    ## adjust the scheduler_interval according to the frames sent by the serial port
    scheduler_interval = 10
    scheduler = tornado.ioloop.PeriodicCallback(checkQueue, scheduler_interval)
    scheduler.start()
    #s_scheduler = tornado.ioloop.PeriodicCallback(sp.run, scheduler_interval)
    #s_scheduler.start()
    mainLoop.start()
