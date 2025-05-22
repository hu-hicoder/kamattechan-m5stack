#include "http_server.h"
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>

const char *ssid = "elecom-a19d07";    // Wi-Fi SSID
const char *password = "c2prrkjhc8nk"; // Wi-Fi Password

AsyncWebServer server(80);

void start_http_server()
{
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(1000);
        Serial.println("Connecting to WiFi...");
    }
    Serial.println("Connected to WiFi");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());

    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
              { request->send(200, "text/plain", "Hello from M5Stack HTTP Server!"); });

    server.begin();
}