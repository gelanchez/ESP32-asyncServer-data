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
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include "constants.h"
#include <WiFi.h>

AsyncWebServer g_server(80);

void setup()
{
    Serial.begin(115200);
    delay(1000);

    pinMode(Constants::ledPin, OUTPUT);

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
    g_server.on("/hello", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(200, "text/plain", "Hello async");
        });
    g_server.onNotFound([](AsyncWebServerRequest *request){
        request->send(404, "text/plain", "Not found async");
    });
    g_server.begin();
}

void loop()
{
}