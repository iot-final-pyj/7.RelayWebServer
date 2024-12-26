#include <Arduino.h>
#include <WiFi.h>
#include <ESPmDNS.h>
#include <WebServer.h>

const char* ssid = "IoT518";
const char* password = "iot123456";

#define RELAY 15

WebServer server(80);

void setup(void){
    Serial.begin(115200);
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    Serial.println("");
    
    pinMode(RELAY, OUTPUT);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("");
    Serial.print("Connected to "); Serial.println(ssid);
    Serial.print("IP address: "); Serial.println(WiFi.localIP());

    if (MDNS.begin("dhtweb")) {
       Serial.println("MDNS responder started");
    }

    server.on("/turnOn", [](){
        digitalWrite(RELAY, HIGH);
        server.send(200, "text/plain", "Turned on");
    });
 
    server.on("/turnOff", [](){
        digitalWrite(RELAY, LOW);
        server.send(200, "text/plain", "Turned off");
    });

    server.on("/toggle", [](){
        digitalWrite(RELAY, !digitalRead(RELAY));
        char response[64];
        sprintf(response, "Turned %s", digitalRead(RELAY) ? "on" : "off");
        server.send(200, "text/plain", response);
    });

    server.onNotFound([](){
        server.send(400, "text/plain", "Not Found\n");
    });

    server.begin();
    Serial.println("HTTP server started");
}

void loop(void){
    server.handleClient();
}
