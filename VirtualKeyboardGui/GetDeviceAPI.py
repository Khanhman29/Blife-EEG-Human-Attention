import requests
import json

BASE_URL = 'http://192.168.0.4:3123'
data = json.dumps(requests.get(BASE_URL + "/desvc").json()).encode('utf8');
f = open("DeviceList.txt", "wb")
f.write(data)
f.close()