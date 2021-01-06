from flask import Flask, render_template, request, make_response
import requests
import urllib.parse

app = Flask(__name__)


@app.route('/')
def index():
    return render_template('index.html')


@app.route('/CORSTrampoline/<endpoint>', methods=['GET', 'POST', 'PUT'])
def corsTramp(endpoint):
    url = urllib.parse.urljoin('http://192.168.1.91', endpoint)
    print(request.args)
    print(request.data)
    if request.method == 'GET':
        resp = requests.get(url, params=request.args)
    elif request.method == 'POST':
        resp = requests.post(url, data=request.data)
    elif request.method == 'PUT':
        resp = requests.put(url, data=request.data)
    else:
        raise(AssertionError('invalid request method: ' + request.method))

    out = make_response(resp.content, resp.status_code)
    return out


@app.route('/calibChase', methods=['PUT', 'POST'])
def calibChase():
    url = 'http://192.168.1.91/calibrate'
    for i in range(401):
        requests.put(url, data=str(i))
    return 'done'


@app.route('/putBlob', methods=['PUT'])
def putBlob():
    print(request.mimetype)
    return 'done'


if __name__ == '__main__':
    app.run('0.0.0.0', debug=True, ssl_context='adhoc')
