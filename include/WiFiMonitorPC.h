/*

  28/10/2022
  Se logro reducir el tamaño utilizando la funcion filter del JSON.
  En la implementacion actual se obtienen los "id" de ultimo nivel del JSON.
  No se se logro filtrar mas que por "childrens".
  Si no es posible un filtrado mayor, queda pendiente "ordenar" lo obtenido.
  Puede ser util ver el codigo generado por el asistente cuando se pida mas de un valor.


*/

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

    StaticJsonDocument<200> filter;
    filter["Children"][0]["Children"][0]["Children"][0]["Children"][0]["Children"][0]["id"] = true;

    DynamicJsonDocument doc(8000);

    DeserializationError error = deserializeJson(doc, http.getStream(), DeserializationOption::Filter(filter), DeserializationOption::NestingLimit(12));

    if (error)
    {
    D:
        Serial.print(F("deserializeJson() failed: "));
        Serial.println(error.f_str());
        return;
    }

    serializeJsonPretty(doc, Serial);

    // Serial.print("\n\n\n\nCPU: ");
    // Serial.println(cpuTempJ);

    http.end();
}