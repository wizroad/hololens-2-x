from flask import Flask, request
import cv2
import numpy as np

app = Flask(__name__)

width = 760
height = 428

imgnum = 0

@app.route('/')
def hello():
    return 'Hello, world'

@app.route('/method', methods=['GET','POST'])
def method():
    if request.method == 'GET':
        return 'communicate with GET'
    else:
        return 'communicate with POST'

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