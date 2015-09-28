# Repo for the Air Sensor project

## Repo structure

 1. `/Accelerometer` - Matlab functions to calibrate accelerometer.
 2. `/Code/esp8266_core` - a framework to connect and send POST request to REST API server. Currenly, supports the following commands:
	1. CONNECT ssid=<ssid> password=<password> - connect to WPA2 WiFi network
	2. WIFI_STATUS - returns current connection status. Extected value is 3 (Connected)
	3. API host=<host> port=<port> - Sets REST API host and port number
	4. API_INFO - returns current REST API host and port number
	5. POST sensorID=<sensor ID> data=<data> - sends a POST request to the REST API. Data in JSON format.
	6. DEBUG 1 or DEBUG 0 - sets module to the debug mode (or turns it off)
3. `/Code/wifi_acc_sensor` - Receives data from accelerometer and sends data to REST API using ESP8266 WiFi module.

## Dependencies
To view the Arduino code you need to download (Arduino IDE)[https://www.arduino.cc/en/Main/Software].