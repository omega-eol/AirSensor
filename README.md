# Repo for the Air Sensor project

System composed of three parts: Arduino Nano micro-controller, ESP8266 WiFi Module (has its own micro-controller on board), and accelerometer - a
place holder for other sensor(s). Arduino will read data from accelerometer and send via EPS module to a API server.

## Repo structure

 1. `/Accelerometer` - Matlab functions to calibrate accelerometer.
 2. `/Code/esp8266_core` - a framework to connect and send POST request to REST API server. Currently, supports the following commands:
	1. CONNECT ssid=<ssid> password=<password> - connect to WPA2 WiFi network
	2. WIFI_STATUS - returns current connection status. Extected value is 3 (Connected)
	3. API host=<host> port=<port> - Sets REST API host and port number
	4. API_INFO - returns current REST API host and port number
	5. POST sensorID=<sensor ID> data=<data> - sends a POST request to the REST API. Data in JSON format.
	6. DEBUG 1 or DEBUG 0 - sets module to the debug mode (or turns it off)
3. `/Code/wifi_acc_sensor` - Receives data from accelerometer and sends data to REST API using ESP8266 WiFi module.

## Dependencies
1. To view the Arduino code you need to download [Arduino IDE](https://www.arduino.cc/en/Main/Software). 
2. Code for REST API could be found [here](https://github.com/omega-eol/sensor_server).