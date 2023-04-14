<h1 align="center"> PC diZplay </h1>

<h4 align="center">
:construction: Proyecto en construcción :construction:
</h4>

### :mag: Descripción:

Proyecto personal de un monitor externo para PC. Se desea mostrar la temperatura, velocidad y/o carga del hardware principal (GPU-CPU-RAM). 
- Se utiliza una pantalla TFT y la navegación entre menus se realiza con un sensor de gestos.
-  ~~La información de la PC se "extrae" de `OpenHardwareMonitorLib.dll `.~~
- La información de la PC se "extrae" del webserver de `LibreHardwareMonitor`
### :wrench: Hardware:
- Wemos D1 Mini
- ST 7735 - 0.96' 80x160
- APDS 9960
### :electric_plug: Wiring:
El capacitor es de 220uF. Es necesario si la alimentación proviene del USB.
<h4 align="center">
<img src="https://user-images.githubusercontent.com/52428096/195724783-010c7869-5348-44a4-9799-43330b96c2d9.png" width=80% height=80%>
</h4>

### :books: Librerias:
- [APDS-9960_Gesture_Sensor_esp8266_Library](https://github.com/Dgemily/APDS-9960_Gesture_Sensor_esp8266_Library)
- [Bodmer/TFT_eSPI](https://github.com/Bodmer/TFT_eSPI)
- [ArduinoJSON](https://github.com/bblanchon/ArduinoJson)
