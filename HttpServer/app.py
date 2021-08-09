from flask import Flask, request
import cv2
import numpy as np

app = Flask(__name__)

width = 760
height = 428

imgnum = 0

d_w = 320
d_h = 288
d_headersize = 17
dnum = 0

# Long throw depth stream
@app.route('/DLT', methods=['POST'])
def DLT():
    imgBuffer = request.data
    image = np.frombuffer(imgBuffer[d_headersize:], dtype=np.uint16)
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
    app.run(host='192.168.0.3', port=5000)
