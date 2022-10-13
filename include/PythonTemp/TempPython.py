import wmi
import time
import serial
w = wmi.WMI(namespace="root\OpenHardwareMonitor")
temperature_infos = w.Sensor()
arduinoData= serial.Serial ('com3',115200)

for i in range (0,20):
    temperature_infos = w.Sensor()
    for sensor in temperature_infos:
        if (sensor.SensorType==u'Temperature' and sensor.Name==u'CPU Package'):
                print(sensor.Name)
                print(sensor.Value)
                time.sleep(1)
                Temp= int(sensor.Value)
                arduinoData.write((bytes(b'%r' % Temp)))