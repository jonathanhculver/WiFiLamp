/*
  Wifi Lamp
  Built upon the arduino wifi shield web server example
 */
#include <SPI.h>
#include <WiFi.h>
#include <TextFinder.h>

char ssid[] = "Wifi";      //  your network SSID (name) 
char pass[] = "WifiPass";   // your network password
int pin = 5;
String lightStatus ="";
int REDPin = 4; 
int GREENPin =2;
int BLUEPin = 8;

int status = WL_IDLE_STATUS;

WiFiServer server(80);

void setup() {
  //initialize rgb led
 pinMode(REDPin, OUTPUT);
 pinMode(GREENPin, OUTPUT);
 pinMode(BLUEPin, OUTPUT);
 // power switch tail
 pinMode(pin, OUTPUT);
 /* light red to show wifi not connected */
 lightRED();
  //Initialize serial and wait for port to open:
  Serial.begin(9600); 
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }
  
  // check for the presence of the shield:
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present"); 
    // don't continue:
    while(true);
  } 
  
  // attempt to connect to Wifi network:
  while ( status != WL_CONNECTED) { 
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:    
    status = WiFi.begin(ssid, pass);

    // wait 10 seconds for connection:
    delay(10000);
  } 
  server.begin();
  printWifiStatus();
  // turn light off
  digitalWrite(pin, LOW);
  // wifi status light on
  lightGREEN();
}


void loop() {
  // listen for incoming clients
  WiFiClient client = server.available();
  if (client) {
    TextFinder  finder(client ); 
    Serial.println("new client");
    // an http request ends with a blank line
    while (client.connected()) {
      if (client.available()) {
        if(finder.find("GET /")) {
           while(finder.findUntil("led", "\n\r")) {
             char type = client.read();
             int val = finder.getValue();
             lightStatus = changeLight(val);
           } 
        }
        //pass a simple http response to the browser
        printWebResponse(client, lightStatus);
        break;
      }
    }
    // give the web browser time to receive the data
    delay(1);
      // close the connection:
      client.stop();
      Serial.println("client disonnected");
  }
}

/* changes the light or reports its status */
String changeLight(int val) {
  /* turn light on */
  if(val== 1) {
      digitalWrite(pin, HIGH);
      lightStatus = "on";
    /* turn light off  */
  } else if(val==0) {
      digitalWrite(pin, LOW);
      lightStatus = "off";
    /* check current status of the light */
  } else if(val==-1) {
      int status = digitalRead(pin);
      if(status == 0) {
          lightStatus = "off";  
      } else if(status == 1) {
          lightStatus = "on";
      }
  }
  return lightStatus;
}



void printWebResponse(WiFiClient client, String lightStatus) {
  Serial.println();
  // so the http request has ended and we can send a reply
  // send a standard http response header
  client.println("HTTP/1.1 200 OK");
  client.println("Access-Control-Allow-Origin: *");
  client.println("Content-Type: text/html");
  client.println();
  client.println("{\"status\":\""+lightStatus+"\"}");
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

void lightRED() {
  analogWrite(REDPin, 255);
  analogWrite(GREENPin, 0);
  analogWrite(BLUEPin, 0);
}
 
void lightGREEN() {
  analogWrite(REDPin, 0);
  analogWrite(GREENPin, 255);
  analogWrite(BLUEPin, 0);
}
 
void lightBLUE() {
  analogWrite(REDPin, 0);
  analogWrite(GREENPin, 0);
  analogWrite(BLUEPin, 255);
}
 
void lightOFF() {
  analogWrite(REDPin, 0);
  analogWrite(GREENPin, 0);
  analogWrite(BLUEPin, 0);
}

