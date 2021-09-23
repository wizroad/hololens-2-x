from flask import Flask, request
import cv2
import numpy as np
from socket import *
import threading
import struct

app = Flask(__name__)


width = 760
height = 428

imgnum = 0

d_w = 320
d_h = 288
d_headersize = 17
dnum = 0

# UDP server thread - receive IMU sensor stream
def UDPserver(ip, port):
    serverSocket = socket(AF_INET, SOCK_DGRAM)
    serverSocket.bind((ip, port))
    dataformat = 'ffffqffffffffff'
    data_name_list = ('MagZ', 'MagY', 'MagX', 'DeviceID', 'DeviceTime', 'ROTw', 'ROTz', 'ROTy', 'ROTx', 'Az', 'Ay', 'Ax', 'Gz', 'Gy', 'Gx')
    print("server start")
    while 1:
        try:
            data, info = serverSocket.recvfrom(2048)
            print("recv data: %s bytes from %s/%d" % (len(data), info[0], info[1]))
            if(len(data)==64): # IMU sensor stream format length
                sensordata = struct.unpack(dataformat, data)
                #####################################
                # Write your code at here using data
                # Sample code: print IMU sensor stream
                #####################################
                for i in range(len(data_name_list)):
                    print("%s: %f / "%(data_name_list[i], sensordata[i]),end='')
            else:
                print("MSG is not IMU sensor stream")
        except:
            print("UDP packet receive error")
            break

    serverSocket.close()

# Long throw depth stream
@app.route('/DLT', methods=['POST'])
def DLT():
    imgBuffer = request.data
    dt = dt.newbyteorder('>')
    image = np.frombuffer(imgBuffer[d_headersize:], dtype=dt)
    image = image.astype(np.uint16)
    image = image.reshape((d_h, d_w, 1))
    new_image = image

    #####################################
    # Write your code at here using new_image
    # Sample code: write image file
    #####################################
    global dnum
    cv2.imwrite('posted_depth\%d.png'%(dnum), new_image)
    print('posted_depth\%d.png'%dnum + ' saved')
    dnum += 1
    ########################################
    return 'DLT Success'

# RGB image stream
@app.route('/Store', methods=['POST'])
def Store():
    imgBuffer = request.data
    image = np.frombuffer(imgBuffer, dtype=np.uint8)
    image = image.reshape((height, width, 4))
    new_image = image[:, :, :3]

    #####################################
    # Write your code at here using new_image
    # Sample code: write image file
    #####################################
    global imgnum
    cv2.imwrite('posted_images\%d.png'%(imgnum), new_image)
    print('posted_images\%d.png'%imgnum + ' saved')
    imgnum += 1
    ########################################
    return 'Store Success'

if __name__=='__main__':

    server_ip = "192.168.0.3"
    server_udp_port = 9005
    server_http_port = 5000

    t = threading.Thread(target=UDPserver, args=(server_ip, server_udp_port))
    t.start()
    app.run(host=server_ip, port=server_http_port)
