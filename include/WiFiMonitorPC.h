

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WebServer.h>
#include <ArduinoJson.h>

const char *ssid = "WiFiG";
const char *password = "independencia4a";
const char *url = "http://arduinojson.org/example.json";
const char *hostname = "Hardware Monitor";



void startupWifi()
{
    WiFi.hostname(hostname);
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }

    Serial.println("Ready");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
}

void getJSONdata()
{
    WiFiClient client;

    HTTPClient http;

    http.useHTTP10(true);
    http.begin(client, "http://192.168.1.33:8085/data.json");
    http.GET();

    DynamicJsonDocument doc(45508);

    DeserializationError error = deserializeJson(doc, http.getStream(), DeserializationOption::NestingLimit(12));

    if (error)
    {
        Serial.print(F("deserializeJson() failed: "));
        Serial.println(error.f_str());
        return;
    }

    JsonObject Children_0 = doc["Children"][0];
    int Children_0_id = Children_0["id"];             // 1
    const char *Children_0_Text = Children_0["Text"]; // "DESKTOP-GLR5S6K"

    String cpuTempJ = doc["Children"][0]["Children"][1]["Children"][3]["Children"][6]["Value"];

    

    Serial.print("\n\n\n\nCPU: ");
    Serial.println(cpuTempJ);

    http.end();

    

   
}