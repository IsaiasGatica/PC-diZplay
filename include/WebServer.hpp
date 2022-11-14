
AsyncWebServer server(80);

void initServer()
{

    if (!SPIFFS.begin())
    {
        Serial.println("An Error has occurred while mounting SPIFFS");
        return;
    }

    server.serveStatic("/assets/main.min.css", SPIFFS, "/assets/main.min.css").setDefaultFile("/assets/main.min.css").setCacheControl("max-age=600");

    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
              { request->send(SPIFFS, "/index.html"); });

    server.begin();
}
