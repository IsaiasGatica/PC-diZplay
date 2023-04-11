

AsyncWebServer server(80);

void initServer()
{

    // server.serveStatic("/assets/style.css", SPIFFS, "/assets/style.css").setDefaultFile("/style.css").setCacheControl("max-age=600");

    // server.serveStatic("/banner.png", SPIFFS, "/banner.png").setDefaultFile("/banner.png").setCacheControl("max-age=600");

    server.on("/assets/bootstrap.min.css", HTTP_GET, [](AsyncWebServerRequest *request)
              {
        AsyncWebServerResponse* response = request->beginResponse(SPIFFS, "/assets/bootstrap.min.css.gz", "text/css");
        response->addHeader("Content-Encoding","gzip");
        response->addHeader("Cache-Control","max-age=86400"); // 1 día
        request->send(response); });
    // server.on("/assets/jquery.min.js", HTTP_GET, [](AsyncWebServerRequest *request)
    //           {
    //     AsyncWebServerResponse* response = request->beginResponse(SPIFFS, "/assets/jquery.min.js.gz", "application/javascript");
    //     response->addHeader("Content-Encoding","gzip");
    //     response->addHeader("Cache-Control","max-age=86400"); // 1 día
    //     request->send(response); });

    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
              { request->send(SPIFFS, "/index.html"); });

    server.on("/", HTTP_POST, [](AsyncWebServerRequest *request)
              {
                  String nr, np, url;
                  if (request->hasArg("new_red"))
                  {
                      nr = request->arg("new_red");
                  }
                  if (request->hasArg("new_pass"))
                  {
                      np = request->arg("new_pass");
                  }
                  if (request->hasArg("URL_LHM"))
                  {
                      url = request->arg("URL_LHM");
                  }
                 

                if (np!=""&& nr !=""){

                  writeSPIFSS("/red.txt",nr);
                  writeSPIFSS("/pass.txt",np);
                  writeSPIFSS("/url.txt",url);

                  EEPROM.write(250, 1);
                  EEPROM.commit();

                  Serial.println(nr);
                  Serial.println(np);
                  Serial.println(url);

                  ESP.restart();
                  
                } });

    server.begin();
}
