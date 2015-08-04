#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
 
const char* ssid = "your-SSID-here";
const char* password = "wifi-password-here";

ESP8266WebServer server(80);

const int led = 4;

String htmlRootPage = "<html><head><title>Simple ESP8266 Web Server</title></head><body><header><h1>ESP8266</h1><h2>IoT made easy!</h2></header><p><a href=\"/on?pin=4\">Turn on LED on pin 4</a></p></body></html>";

String htmlPinOnPage = "<html><head><title>Simple ESP8266 Web Server</title></head><body><header><h1>ESP8266</h1><h2>IoT made easy!</h2></header><p><a href=\"/off?pin=4\">Turn off LED on pin 4</a></p></body></html>";

String htmlPinOffPage = "<html><head><title>Simple ESP8266 Web Server</title></head><body><header><h1>ESP8266</h1><h2>IoT made easy!</h2></header><p><a href=\"/on?pin=4\">Turn on LED on pin 4</a></p></body></html>";

void handleRoot() {
  server.send(200, "text/html", htmlRootPage);
}

void handleOn() {
  //digitalWrite(led, 1);
  server.send(200, "text/html", htmlPinOnPage);
  
  String httpArgumentName = "";
  String httpArgumentValue = "";
  
  for (uint8_t i=0; i<server.args(); i++){
    httpArgumentName = server.argName(i);
    if(httpArgumentName == "pin") {
      httpArgumentValue = server.arg(i);
      digitalWrite(httpArgumentValue.toInt(), 1);
    }
  }
  
}

void handleOff() {
  //digitalWrite(led, 0);
  server.send(200, "text/html", htmlPinOffPage);

  String httpArgumentName = "";
  String httpArgumentValue = "";
  
  for (uint8_t i=0; i<server.args(); i++){
    httpArgumentName = server.argName(i);
    if(httpArgumentName == "pin") {
      httpArgumentValue = server.arg(i);
      digitalWrite(httpArgumentValue.toInt(), 0);
    }
  }

}

void handleNotFound(){
  digitalWrite(led, 1);
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET)?"GET":"POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i=0; i<server.args(); i++){
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
  digitalWrite(led, 0);
}
 
void setup(void){
  pinMode(led, OUTPUT);
  digitalWrite(led, 0);
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);
  server.on("/on", handleOn);
  server.on("/off", handleOff);

  server.onNotFound(handleNotFound);
  
  server.begin();
  Serial.println("HTTP server started");
}
 
void loop(void){
  server.handleClient();
} 
