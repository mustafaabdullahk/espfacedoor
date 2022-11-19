import face_recognition
from flask import Flask, jsonify, request, redirect
import cv2
import numpy as np


ALLOWED_EXTENSIONS = {'png', 'jpg', 'jpeg', 'gif'}

app = Flask(__name__)


def allowed_file(filename):
    return '.' in filename and \
           filename.rsplit('.', 1)[1].lower() in ALLOWED_EXTENSIONS

from PIL import Image
from io import BytesIO
import json
import os

ENROLLS_DIR='enrolls'
# Get face encodings for any faces in the uploaded image
known_face_encodings = []

def checkAndCreate():
    isExists = os.path.isdir('enrolls')
    if isExists:
        return
    else:
        os.mkdir('enrolls')

def syncEnrolledFace():
    dir_list = os.listdir(ENROLLS_DIR)
    for file in dir_list:
        # Load the uploaded image file
        img = face_recognition.load_image_file(ENROLLS_DIR + '/' + file)
        # Get face encodings for any faces in the uploaded image
        face_encodings = face_recognition.face_encodings(img)
        known_face_encodings.append(face_encodings)

@app.route('/enroll', methods=['POST'])
def enroll_face():
    result = {
        "face_found_in_image": 0,
        "is_picture_of_obama": 0
    }
    print('-------------------')
    data = json.loads(request.get_data())
    print(data['face_id'])
    # print(request.get_data())
    f = open(f"enrolls/{data['face_id']}", "a")
    f.write(str(data['face_enroll']))
    f.close()
    print(result)
    return jsonify(result)

@app.route('/recognize', methods=['POST'])
def recognize():
    # im = Image.open(BytesIO(request.get_data()))
    data_len = len(request.get_data())
    if data_len < 10:
        return 'image not found'

    data = json.loads(request.get_data())
    print(data['image'])
    res = detect_faces_in_image(BytesIO(data['image']))
    return res


@app.route('/', methods=['GET', 'POST'])
def upload_image():
    # Check if a valid image file was uploaded
    im = Image.open(BytesIO(request.get_data()))
    # im.save('result.jpg')
    res = detect_faces_in_image(BytesIO(request.get_data()))
    print(f'result {res}')
    return res

def detect_faces_in_image(file_stream):
    # Load the uploaded image file
    img = face_recognition.load_image_file(file_stream)
    # Get face encodings for any faces in the uploaded image
    unknown_face_encodings = face_recognition.face_encodings(img)

    face_found = False
    is_recognize = False

    if len(unknown_face_encodings) > 0:
        face_found = True
        # See if the first face in the uploaded image matches the known face of Obama
        for face in known_face_encodings:
            match_results = face_recognition.compare_faces(face[0], unknown_face_encodings[0])
            if match_results[0]:
                is_recognize = True

    # Return the result as json
    result = {
        "face_found_in_image": face_found,
        "face_recognized": is_recognize
    }
    print(result)
    return jsonify(result)

if __name__ == "__main__":
    checkAndCreate()
    syncEnrolledFace()
    app.run(host='0.0.0.0', port=8001, debug=True)