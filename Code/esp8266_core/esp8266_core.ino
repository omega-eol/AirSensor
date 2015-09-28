#include <ESP8266WiFi.h>

// API
char host[16];
int port;

// Communication
String inputString = "";         // a string to hold incoming data
boolean stringComplete = false;  // whether the string is complete

// Debug mode
boolean debug = false;

void setup() {
  // initialize serial port
  Serial.begin(9600);
  delay(10);

  // reserve 256 bytes for the inputString:
  inputString.reserve(256);

  // ESP is ready to receive commands
  Serial.println(""); Serial.println("ESP:OK");
}

void loop() {

  serialEvent(); //call the function
  if (stringComplete) {
    if (debug) Serial.println("ESP:COMMAND=" + inputString);

    // DEBUG
    // DEBUG 1 or DEBUG 0
    if (inputString.indexOf("DEBUG") != -1) {
      // parsing value if 1 then true, else 0
      int value = inputString.substring(inputString.indexOf(" ")+1).toInt();
      if (value == 1) {
        debug = true;
        Serial.println("ESP:DEBUG=1");
      } else {
        debug = false;
        Serial.println("ESP:DEBUG=0");
      }
    }

    // CONNECT
    // CONNECT ssid=Elochka_2.4GHz password=Kukareku
    if (inputString.indexOf("CONNECT") != -1) {
      // get SSID
      String temp = inputString.substring(inputString.indexOf("ssid=")+5);
      String ssid = temp.substring(0, temp.indexOf(" "));
      //Serial.println("ssid=" + ssid);     

      // get PASSWORD
      String password = inputString.substring(inputString.indexOf("password=")+9);
      //Serial.println("password=" + password);

      // Connect to WiFi network
      char c_ssid[ssid.length()+1];
      char c_password[password.length()+1];
      ssid.toCharArray(c_ssid, ssid.length()+1);
      password.toCharArray(c_password, password.length()+1);
      int wifi_status = connect_to_wifi(c_ssid, c_password);

      if (wifi_status == 0) {
        Serial.println("ESP:CONNECTED"); 
      } else {
        if (debug) Serial.println("ESP:CONNECTION_ERR");
      }
    }

    // WIFI_STATUS
    if (inputString.indexOf("WIFI_STATUS") != -1) {
      Serial.print("ESP:WIFI_STATUS=");
      Serial.println(WiFi.status());
    }

    // API - set API host and port number
    // API host=192.168.0.101 port=3000
    if (inputString.indexOf("API ") != -1) {//we need this space after API so that this -> API_INFO!
      // get Sensor ID
      String temp = inputString.substring(inputString.indexOf("host=")+5);
      String strHost = temp.substring(0, temp.indexOf(" "));
      strHost.toCharArray(host, strHost.length()+1);

      // get data JSON
      String strPort = inputString.substring(inputString.indexOf("port=")+5);
      port = strPort.toInt();
      if (debug) {
        Serial.print("API is SET: host=");
        Serial.print(host);
        Serial.println(" port=" + strPort);
      }
      
      Serial.println("ESP:API_SET");
    }

    // API_INFO
    if (inputString.indexOf("API_INFO") != -1) {
      Serial.print("ESP:API_INFO host="); 
      Serial.print(host);
      Serial.print(" port=");
      Serial.println(port, DEC);
    }

    // POST - Post data to server
    // POST sensorID=55f824c006e288bd25abd3bc data={"o1":1.1,"o2":1.2,"o3":1.3}
    if (inputString.indexOf("POST") != -1) {
      // get Sensor ID
      String temp = inputString.substring(inputString.indexOf("sensorID=")+9);
      String sensorID = temp.substring(0, temp.indexOf(" "));
      //Serial.println("sensorID=" + sensorID);     

      // get data JSON
      String data = inputString.substring(inputString.indexOf("data=")+5);
      //Serial.println("data=" + data);

      // send data to server API
      int res = save_data(sensorID, data);
      if (res == 0) {
        Serial.println("ESP:POST_OK");
      } else {
        Serial.println("ESP:POST_ERR");
      };
    }
    
    // clear the string:
    inputString = "";
    stringComplete = false;
  }
    
}

// Connect to WiFi network
int connect_to_wifi(char* ssid, char* password) {
  if (debug) {
    Serial.print("Connecting to "); Serial.println(ssid); 
  }

  // try to connect
  WiFi.begin(ssid, password);
  int v_tt = 0;
  while ((WiFi.status() != WL_CONNECTED) && (v_tt <= 100)) {
    delay(500);
    v_tt += 1;
    if (debug) Serial.print(".");
  }
  if (debug) Serial.println("");

  // check status
  if (WiFi.status() == WL_CONNECTED) { // WiFi Connected
    if (debug) printWifiStatus();
    return(0);
  }; 

  return(-1);
}

void printWifiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}

int save_data(String sensorID, String data) {
  // set the API host
  if (debug) {
    Serial.print("connecting to "); 
    Serial.println(host);
  }
 
  // Use WiFiClient class to create TCP connections
  WiFiClient client;
  if (!client.connect(host, port)) {
    if (debug) Serial.println("Could not connect to the host.");
    return(-1);
  }
  
  // Compose requerst to the host
  String request = String("POST ") + "/api/sensors/" + sensorID + "/dataentry" + " HTTP/1.1\r\n" +
       "Accept: application/json\r\n" +
       "Content-Length: " + data.length() + "\r\n" +
       "Content-Type: application/json\r\n"+
       "Host: " + host + "\r\n" + 
       "Connection: close\r\n\r\n" +
       data + "\r\n";
  if (debug) Serial.println(request);
  client.print(request);
  delay(10);
  
  // Read all the lines of the reply from server and print them to Serial
  String response = "";
  response.reserve(256);
  while (client.available()) {
    char c = (char)client.read();
    response += c;
  }
  if (debug) Serial.println(response);

  // check if the response status is 200 OK
  if (response.indexOf("200 OK") == -1) return(-1);
  return(0);
}

// Read data input from Serial
void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read();
    if (inChar != '\n') {
      // add it to the inputString:
      inputString += inChar;
    } else {
      // do not add the end of the line character
      stringComplete = true;
      break;
    }    
  }
}



