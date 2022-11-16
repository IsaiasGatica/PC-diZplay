

AsyncWebServer server(80);

void initServer()
{

    server.serveStatic("/assets/main.min.css", SPIFFS, "/assets/main.min.css").setDefaultFile("/assets/main.min.css").setCacheControl("max-age=600");

    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
              { request->send(SPIFFS, "/index.html"); });

    server.on("/", HTTP_POST, [](AsyncWebServerRequest *request)
              {
                  String nr, np, cr, cp;
                  if (request->hasArg("new_red"))
                  {
                      nr = request->arg("new_red");
                  }
                  if (request->hasArg("new_pass"))
                  {
                      np = request->arg("new_pass");
                  }
                  if (request->hasArg("conf_red"))
                  {
                      cr = request->arg("conf_red");
                  }
                  if (request->hasArg("conf_pass"))
                  {
                      cp = request->arg("conf_pass");
                  }


                if (np!=""&& nr !=""){

                  writeSPIFSS("/red.txt",nr);
                  writeSPIFSS("/pass.txt",np);

              
                     
                    // ModoAP=false;
                    // ESP.restart();
                    // saveEeprom(0,nr);
                    // saveEeprom(100,np);




                    EEPROM.write(250, 1);
                    EEPROM.commit();

                  Serial.println(nr);
                  Serial.println(np);

                  ESP.restart();
                  
                } });

    server.begin();
}
