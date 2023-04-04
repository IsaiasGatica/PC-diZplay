void saveEeprom(int addr, String credencial)
{

    int size = credencial.length();
    char inchar[50];
    credencial.toCharArray(inchar, size + 1);

    for (int i = 0; i < size; i++)
    {

        EEPROM.write(addr + i, inchar[i]);
    }

    for (int i = size; i < 50; i++)
    {

        EEPROM.write(addr + 1, 255);
    }

    EEPROM.commit();
}

String readEeprom(int addr)
{
    byte lectura;
    String strlectura;
    for (int i = addr; i < addr + 50; i++)
    {

        lectura = EEPROM.read(i);
        if (lectura != 255)
        {

            strlectura += (char)lectura;
        }
    }

    return strlectura;
}

boolean Modowifi()
{
    int ModAP;
    ModAP = EEPROM.read(250);

    if (ModAP != 1)
    {
        return true;
    }
    else
    {

        return false;
    }
}

void defaultWifi()
{

    EEPROM.write(250, 0);
    EEPROM.commit();
    ESP.restart();
}

void writeSPIFSS(String path, String credencial)
{
    File f = SPIFFS.open(path, "w");
    if (!f)
    {
        Serial.println("Fallo apertura de archivo");
    }
    Serial.println("====== Escribiendo en el archivo SPIFFS =========");
    // write 10 strings to file
    f.print(credencial);
}

String readSPIFSS(String path)
{

    File f = SPIFFS.open(path, "r");
    if (!f)
    {
        Serial.println("Fallo apertura de archivo");
    }
    Serial.println("====== Leyendo del archivo SPIFFS =======");
    // write 10 strings to file

    String s = f.readStringUntil('\n');

    return s;
}