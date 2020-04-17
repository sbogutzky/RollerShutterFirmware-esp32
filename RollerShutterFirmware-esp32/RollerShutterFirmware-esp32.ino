/*
  Roller Shutter Firmware
  The MIT License (MIT)
  Copyright (c) 2020 Simon Bogutzky
  Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the “Software”), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
  The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
  THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
  
  v1.0.0
*/

#include <ESP32WebServer.h>

const char* ssid     = "";
const char* password = "";

ESP32WebServer server(80);

const char* vers = "1.0.0";
const int dwn = 12;
const int stp = 13;
const int up = 14;

void handleRoot() {
  server.send(200, "text/plain; charset=utf-8", "Roller shutter webserver");
}

void handleNotFound(){
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
}

void setup()
{
  pinMode(dwn, OUTPUT);
  pinMode(stp, OUTPUT);
  pinMode(up, OUTPUT);

  digitalWrite(dwn, HIGH);
  digitalWrite(stp, HIGH);
  digitalWrite(up, HIGH);
  
  Serial.begin(115200);
    
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

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

  server.on("/down", [](){
    resetCmd();
    digitalWrite(dwn, LOW);
    server.send(200, "text/plain", "down");
  });

  server.on("/stop", [](){
    resetCmd();
    digitalWrite(stp, LOW);
    server.send(200, "text/plain", "stop");
  });

  server.on("/up", [](){
    resetCmd();
    digitalWrite(up, LOW);
    server.send(200, "text/plain", "up");
  });

  server.on("/version", [](){
    server.send(200, "text/plain", vers);
  });

  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");
}

void resetCmd() {
  digitalWrite(dwn, HIGH);
  digitalWrite(stp, HIGH);
  digitalWrite(up, HIGH);
  delay(2000);
}

void loop(void){
  server.handleClient();
}
