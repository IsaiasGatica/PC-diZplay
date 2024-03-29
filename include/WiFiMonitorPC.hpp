/*


  04/04/2023
  Se implementa una función de restart a modo AP si la conexión con Libre Hardware
  Monitor no se puede concretar. La misma captura los intervalos de tiempo que se obtienen al
  no poder conectarse. Si luego de 3 errores de reconexión, el tiempo obtenido esta dentro de este
  intervalo se resetea el ESP32 a modo defaulWifi. No se implementó únicamente con contador ya que estos
  errores pueden darse de forma aleatoria durante todo el tiempo de uso y aún asi lograr reconectarse.
  En resumen se busca la situación de obtener errores consecutivos (el tiempo del intervalo se obtuvo empíricamente)

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

const char *hostname = "Hardware Monitor";

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org");

String Nan = "--";
int reconnect = 0;
int CountError = 0;
unsigned long tiempo1 = 0;
unsigned long tiempo2 = 0;

const byte DNS_PORT = 53;
IPAddress apIP(192, 168, 1, 1);
DNSServer dnsServer;

void startupWifi()
{
    ssid = readSPIFSS("/red.txt");
    password = readSPIFSS("/pass.txt");
    url = readSPIFSS("/url.txt");

    Serial.println("Credenciales");
    Serial.println(ssid);
    Serial.println(password);

    WiFi.hostname(hostname);
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED)
    {

        for (int i = 0; i < 19; i++)

        {
            delay(40);
            tft.pushImage(0, 0, animation_width, animation_height, Eye[i]);
        }
        for (int i = 18; i > 0; i--)

        {
            delay(40);
            tft.pushImage(0, 0, animation_width, animation_height, Eye[i]);
        }

        reconnect = reconnect + 1;

        if (reconnect == 10)
        {
            Serial.println("No se pudo conectar");
            defaultWifi();
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
    http.begin(client, "http://" + url + "/data.json");
    http.GET();

    StaticJsonDocument<144> filter;
    filter["Children"][0]["Children"][0]["Children"][0]["Children"][0]["Value"] = true;

    DynamicJsonDocument doc(9144);

    DeserializationError error = deserializeJson(doc, http.getStream(), DeserializationOption::Filter(filter), DeserializationOption::NestingLimit(12));

    if (error)
    {
        Serial.print(F("deserializeJson() failed: "));
        Serial.println(error.f_str());
        CountError++;
        tiempo2 = millis() - tiempo2;
        Serial.println(tiempo2);
        if ((CountError > 3) && (tiempo2 < 60000))
        {
            defaultWifi();
        }
    }

    http.end();

    timeClient.update();
    int currentHour = timeClient.getHours();
    int currentMinute = timeClient.getMinutes();

    String Hora = timeClient.getFormattedTime();

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
