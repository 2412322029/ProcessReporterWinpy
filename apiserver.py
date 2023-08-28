from flask import Flask, request
import json
key = "123456"
app = Flask(__name__)
data = {}


@app.route('/api/msg', methods=['POST'])
def receive_post():
    global data
    data = request.get_data(as_text=True)
    print(data)
    data = json.loads(data)
    print(data)
    if data["api_key"] == key:
        return 'Data received!\n'
    else:
        return "api key error\n"


@app.route('/api/sendmsg')
def send_post():
    del data["api_key"]
    return data


@app.route('/',)
def index():
    return open("index.html","r")


if __name__ == '__main__':
    app.config['JSON_AS_ASCII'] = False
    app.run(host='0.0.0.0', port=8080)
