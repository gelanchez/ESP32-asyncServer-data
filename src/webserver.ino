/**
 * @file webserver.ino
 * @author José Ángel Sánchez (https://github.com/gelanchez)
 * @brief Main implementation of the async webserver to control a LED and present
 * data from the sensors to the clients using websockets.
 * @version 0.0.1
 * @date 2020-09-29
 * @copyright GPL-3.0
 */

#define ESP32
#include <Arduino.h>
#include <ArduinoJson.h>
#include <AsyncTCP.h>
#include "constants.h"
#include <ESPAsyncWebServer.h>
#include "index.h"  // HTML webpage contents with javascripts
#include "mysensors.h"
#include <WiFi.h>

Photoresistor g_photoresistor(Constants::photoresistorPin);
Thermistor g_thermistor(Constants::thermistorPin);

StaticJsonDocument<150> g_doc;

AsyncWebServer g_server(80);
AsyncWebSocket g_ws("/ws");

static unsigned long g_lastUpdate = millis();

void onWsEvent(AsyncWebSocket * server, AsyncWebSocketClient * client, AwsEventType type, void * arg, uint8_t *data, size_t len){
    if(type == WS_EVT_CONNECT)
    {
        Serial.println("Websocket client connection received");
    }
    else if(type == WS_EVT_DISCONNECT)
    {
        Serial.println("Client disconnected");
    }
    else if(type == WS_EVT_DATA)
        Serial.println("Data received: ");
}

void setup()
{
    Serial.begin(115200);
    delay(1000);

    pinMode(Constants::ledPin, OUTPUT);
    pinMode(Constants::photoresistorPin, INPUT);

    /**
     * @brief Connect to WiFi.
     */
    WiFi.config(Constants::ip, Constants::gateway, Constants::subnet);
    WiFi.begin(Constants::ssid, Constants::password);
    while (WiFi.status() != WL_CONNECTED)
        delay(1000);
    Serial.print("Connected to WiFi ");
    Serial.print(Constants::ssid);
    Serial.print(" as ");
    Serial.println(WiFi.localIP());

    /**
     * @brief Server setup.
     */
    g_server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send_P(200, "text/html", MAIN_page);
        });
    g_server.on("/hello", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(200, "text/plain", "Hello world");
        });
    g_server.onNotFound([](AsyncWebServerRequest *request){
        request->send(404, "text/plain", "Not found");
    });

    g_ws.onEvent(onWsEvent);
    g_server.addHandler(&g_ws);

    g_server.begin();
}

void loop()
{
    if ((millis() - g_lastUpdate) > 2000)
    {
        g_lastUpdate = millis();

        g_doc["illuminance"] = g_photoresistor.read();
        g_doc["temperature"] = g_thermistor.read();

        /**
         * @brief Print values.
         */
        //serializeJson(g_doc, Serial);
        //Serial.println();

        String output;
        serializeJson(g_doc, output);
        
        //char charJson[output.length() + 1];
        //output.toCharArray(charJson, output.length() + 1);
            
        g_ws.textAll(output);
    
        g_ws.cleanupClients();
    }
}