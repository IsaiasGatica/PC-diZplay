/*

  31/10/2022
  Se agregó un switch-case que se encarga de retorar el valor de data PC
  correspondiente segun la pantalla del menu.


  30/10/2022
  Se logro reducir el tamaño significativamente con el filtrado de JSON.
  Si bien el filtrado puede ser mayor, al día de hoy no se puede por como esta
  armado el JSON de openhardwaremonitor.
  Se esta enviando a la pantalla la TempPackage del Cpu, previamente transformado
  a entero.
  Queda pendiente registrar los demas datos de interes y como tratarlos.
  String?


  28/10/2022
  Se logro reducir el tamaño utilizando la funcion filter del JSON.
  En la implementacion actual se obtienen los "id" de ultimo nivel del JSON.
  No se se logro filtrar mas que por "childrens".
  Si no es posible un filtrado mayor, queda pendiente "ordenar" lo obtenido.
  Puede ser util ver el codigo generado por el asistente cuando se pida mas de un valor.


*/

const char *url = "http://arduinojson.org/example.json";
const char *hostname = "Hardware Monitor";

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org");

String Nan = "--";
int reconnect = 0;

const byte DNS_PORT = 53;
IPAddress apIP(192, 168, 1, 1);
DNSServer dnsServer;

void startupWifi()
{

    // readEeprom(0).toCharArray(ssid, 50);
    // readEeprom(100).toCharArray(password, 50);

    ssid = readSPIFSS("/red.txt");
    password = readSPIFSS("/pass.txt");

    Serial.println("Credenciales");
    Serial.println(ssid);
    Serial.println(password);

    if (!SPIFFS.begin())
    {
        Serial.println("An Error has occurred while mounting SPIFFS");
        return;
    }

    WiFi.hostname(hostname);
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        tft.pushImage(0, 0, 160, 80, image);
        delay(500);

        tft.pushImage(0, 0, 160, 80, image2);
        delay(100);

        tft.pushImage(0, 0, 160, 80, image3);
        delay(10);

        reconnect = reconnect + 1;

        if (reconnect == 15)
        {
            Serial.println("No se pudo conectar");
            defaultWifi();
            ESP.restart();
        }
    }

    Serial.println("Ready");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());

    timeClient.begin();
    timeClient.setTimeOffset(-10800);
}

void startAP()
{

    WiFi.mode(WIFI_AP);
    WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
    WiFi.softAP("Pc diZplay");
    dnsServer.setTTL(300);
    dnsServer.start(DNS_PORT, "Pcdizplay.com", apIP);

    Serial.print("Iniciado AP ");
    Serial.println(ssid_AP);
    Serial.print("IP address:\t");
    Serial.println(WiFi.softAPIP());
}

String getJSONdata(uint8_t Pantalla)
{
    WiFiClient client;
    HTTPClient http;

    http.useHTTP10(true);
    http.begin(client, "http://192.168.1.36:8085/data.json");
    http.GET();

    StaticJsonDocument<144> filter;
    filter["Children"][0]["Children"][0]["Children"][0]["Children"][0]["Value"] = true;

    DynamicJsonDocument doc(9144);

    DeserializationError error = deserializeJson(doc, http.getStream(), DeserializationOption::Filter(filter), DeserializationOption::NestingLimit(12));

    if (error)
    {
        Serial.print(F("deserializeJson() failed: "));
        Serial.println(error.f_str());
    }

    // serializeJsonPretty(doc, Serial);

    http.end();

    timeClient.update();
    int currentHour = timeClient.getHours();
    int currentMinute = timeClient.getMinutes();

    String Hora = timeClient.getFormattedTime();

    // String Hora = String(currentHour) + ":" + String(currentMinute);

    switch (Pantalla)
    {

    case Gputemp:
    {
        String GpuTempData = doc["Children"][0]["Children"][3]["Children"][2]["Children"][0]["Value"];
        return GpuTempData.substring(0, 2);

        break;
    }
    case Gpuload:
    {
        String GpuLoadData = doc["Children"][0]["Children"][3]["Children"][3]["Children"][0]["Value"];
        return GpuLoadData.substring(0, 2);

        break;
    }
    case Cputemp:
    {
        String CpuTempData = doc["Children"][0]["Children"][1]["Children"][3]["Children"][6]["Value"];
        return CpuTempData.substring(0, 2);
        break;
    }
    case Cpuload:
    {
        String CpuLoadData = doc["Children"][0]["Children"][1]["Children"][4]["Children"][0]["Value"];
        return CpuLoadData.substring(0, 2);
        break;
    }
    case RAM:

    {
        String RAMData = doc["Children"][0]["Children"][2]["Children"][0]["Children"][0]["Value"];
        return RAMData.substring(0, 2);
        break;
    }
    case Gpufan:
    {
        String GpuFAN = doc["Children"][0]["Children"][3]["Children"][4]["Children"][0]["Value"];
        return GpuFAN.substring(0, 2);

        break;
    }
    case Reloj:
    {
        return Hora;
    }
    default:

        break;
    }

    return Nan;
}
