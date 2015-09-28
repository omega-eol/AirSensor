#include <SoftwareSerial.h>

// soft serial port to communicate with ESP8266 WiFi module
SoftwareSerial esp8266(7,8);

// Debug mode
boolean debug = true;

// WiFi network
const String ssid     = "Elochka_2.4GHz";
const String password = "Kukareku";
boolean wifi_connected = false;

// Remote server
const String serverIP = "192.168.0.103";
const int port = 3000;

// Sensor ID
const String sensorID = "55f824c006e288bd25abd3bc";

// other variables
String commandBuffer = "";
boolean stringComplete = false;
int v_error = 0;      // number of consecutive transmission errors
int wait_time = 5000; // time to pause in main loop

// Accelerometer
const int xpin = A1;                  // x-axis of the accelerometer
const int ypin = A2;                  // y-axis
const int zpin = A3;                  // z-axis (only on 3-axis models)

// ESP commands:
// to connect to WiFi network
// CONNECT ssid=Elochka_2.4GHz password=Kukareku
// check wifi status
// WIFI_STATUS
// send API info
// API host=192.168.0.102 port=3000
// send POST request
// POST sensorID=55f824c006e288bd25abd3bc data={"o1":1.0,"o2":2.0,"o3":3.0}

// Runs ones after RST
void setup()
{
  Serial.begin(9600);
  delay(10);
  esp8266.begin(9600);
  delay(10);

  // initialize a string buffer
  commandBuffer.reserve(200);
}

// Runs forever..
void loop()
{
  // Display data from ESP
  //R_communication();
  // Send command from Serial Monitor to ESP
  //S_communication();  

  // check WiFi status, connect if needed
  if (wifi_connected) {

    // Send accelerometer`s data
    int send_data_response = send_acc_data(analogRead(xpin), analogRead(ypin), analogRead(zpin));
  
    // check the number of errors
    if (send_data_response > 0) {
      v_error += 1;
      if (v_error > 3) {
        v_error = 0;
        wifi_connected = false; // try to reconnect on next iteration
      }
    } else {
      v_error = 0;
    }

    
  } else {
    // Attemp to connect to the network
    connect_to_wifi();
  }

  // wait time between iterations
  delay(wait_time);
}

void connect_to_wifi() {
  if (debug) Serial.println("Attempting to connect: " + ssid);
  esp8266.print("CONNECT ssid=" + ssid + " password=" + password + "\n");
  
  // wait 5 seconds
  delay(5000);

  // check ESP response
  String response = get_esp_response();
  if (debug) Serial.println("WiFi Connection = " + response);
  if (response.indexOf("ESP:CONNECTED") != -1) {
    wifi_connected = true;
    if (debug) Serial.println("Connected to " + ssid);
    
    // Set server API
    esp8266.print("API host=" + serverIP + " port=");
    esp8266.println(port, DEC);
    delay(10);

    response = get_esp_response();
    if (debug) Serial.println("API Set = " + response);
  }
}

// Send data to ESP
// POST sensorID=55f824c006e288bd25abd3bc data={"o1":1.1,"o2":1.2,"o3":1.3}
int send_acc_data(int x, int y, int z) {
  // prepare command
  String post = "POST sensorID=";
  post += sensorID;
  post += " data={\"o1\":";
  post += x;
  post += ",\"o2\":";
  post += y;
  post += ",\"o3\":";
  post += z;
  post += "}";

  // send command
  esp8266.println(post);
  delay(100);

  // check response from ESP
  String esp_response = get_esp_response();
  if (debug) Serial.println("Send Data = " + esp_response);
  if (esp_response.indexOf("ESP:POST_OK") != -1) return(0); // return 0 if OK
  
  return(1);
}

// Check ESP wifi status
int check_wifi_status() {
  int wifi_status = -1;
  
  // ESP WiFi status command
  esp8266.print("WIFI_STATUS\n");
  delay(10);

  // Read response from ESP
  String s_buffer = get_esp_response();

  // Parse WiFi status from response
  if (debug) Serial.println("WiFi Status = " + s_buffer);
  if (s_buffer.indexOf("ESP:CONNECTED") != -1) wifi_status = 3;
  if (s_buffer.indexOf("ESP:WIFI_STATUS=") != -1) {
    wifi_status = s_buffer.substring(16).toInt();
  }
  
  return(wifi_status);
}

String get_esp_response() {
  // Read response from ESP
  String s_buffer = "";
  while (esp8266.available() > 0) {
     char c = (char)esp8266.read();
     if (c == '\n') break;
     s_buffer += c;
  }
  return(s_buffer);
}

// Display data from ESP
void R_communication() {
  if(esp8266.available()) {
    while(esp8266.available() > 0) {
      Serial.write(esp8266.read());
    }
  }
}

// Send command from Serial Monitor to ESP
void S_communication() {
    
  // read the whole command from Serial first
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read();
    if (inChar != '\n') {
      // add it to the inputString:
      commandBuffer += inChar;
    } else {
      // do not add the end of the line character
      stringComplete = true;
      break;
    }    
  }

  // send command to ESP
  if (stringComplete) {
    //Serial.println("NANO: " + commandBuffer);
    esp8266.print(commandBuffer + "\n");
    commandBuffer = "";
    stringComplete = false;
  } 
}

