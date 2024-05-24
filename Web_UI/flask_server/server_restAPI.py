from os import urandom
import json
from typing_extensions import dataclass_transform
from flask import Flask, jsonify, request, render_template, send_file
from flask_cors import CORS 
from flask_sse import sse
import requests


app = Flask(__name__) 
#app.config["REDIS_URL"]="redis://127.0.0.1"
#app.register_blueprint(sse,url_prefix ='/stream')

CORS(app,origins='*')
stored_data = None
# Sample data (can be replaced with your own data source)
tasks = [
    {
        'id': 1,
        'title': 'ON/Off',
        'description': 'This is task 1.',
        'done': False
    },
    {
        'id': 2,
        'title': 'Timer',
        'description': 'This is task 2.',
        'done': False
    },
	    {
        'id': 3,
        'title': 'Calender',
        'description': 'This is task 1.',
        'done': False
    },	    
     
]
@app.route('/test',methods=['POST'])
def get_test():
    # Generate or fetch the data to send to JavaScripto
    data = request.json

    print(data)
    data = {"message": "Hello from Flask!"}
    return jsonify(data)
@app.route('/get_data')
def get_data():
    # generate or fetch the data to send to javascript
    data = {"message": "hello from flask!"}
    return jsonify(data)


@app.route("/upload_root")
def upload_root():
    firmware_file = './bin_OTA/root/firmware.bin' 
    return send_file(firmware_file, as_attachment=True) 

@app.route("/upload_plug")
def upload_plug():
    firmware_file = './bin_OTA/plug/firmware.bin' 
    return send_file(firmware_file, as_attachment=True) 


esp_data = {}
def handle_Powerdata(data):
    global esp_data
    esp_data =data 
    sse.publish(data,type='data')

# Route to get all tasks
def ONoff_callback(data, IP):
	#url ="http://10.0.0.6/ONoff"
	#192.168.1.43
    url = "http://192.168.1.21"+IP
    print(url) 
    response = requests.post(url,json=data,timeout=3000)
    if (response.status_code == 200 ):
        print("data send ") 
        return True 
    else :
        print ("error")
        return False
    return False
@app.route('/')
def handle_rout():
    return render_template("index.html")

@app.route('/api/tasks', methods=['GET'])
def get_tasks():
    return jsonify({'tasks': tasks})

@app.route('/ONoff',methods=['POST'])
def handel_onOff():
    data = request.json
    v = json.loads(data)
    value_1 = v.pop("1")
    ONoff_callback(value_1, "/ONoff")
    print( value_1 )
    return jsonify({"message":"data received"})
@app.route('/allowed_devcies',methods=['POST'])
def handel_allowed_devcies():
    data = request.json
    ONoff_callback(data, "/allowed_devcies")
    print(data )
    return jsonify({"message":"data received"})

@app.route('/data', methods=['POST'])
def handle_data():
	global stored_data
	print(request.method,"aaa")
	if request.method == 'POST':
		stored_data = request.json
		print(stored_data) 
		return jsonify({"mg":"done"}), 200

# Route to get a specific task by its ID
@app.route('/dataaa', methods=['POST'])
def handle_dataaa():
	global stored_data 
	print(request.method,"aaa")
	if request.method == 'POST':
		if (stored_data):
			return stored_data
		else:  
			return jsonify({"mg":"waiting"}), 200

@app.route('/init', methods=['POST'])
def handle_init():
	print(request.method,"aaa")
	print (request.json) 
	return jsonify({"msg":"correct"}),200





if __name__ == '__main__':
    app.run(debug=True,host='0.0.0.0')

