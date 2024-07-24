from flask import Flask
from flask import request

app = Flask(__name__)
sensor_data_list = []

@app.route('/sensor/data', methods=['POST'])
def post_sensor_data():
    temperature = request.args.get('temperature')
    humidity = request.args.get('humidity')
    subData = {"temperature": temperature, "humidity": humidity}
    sensor_data_list.append(subData)

    return 'Data received successfully!'

@app.route('/sensor/data', methods=['GET'])
def get_sensor_data():
    return sensor_data_list

if __name__ == '__main__':
    app.run(debug=True)
