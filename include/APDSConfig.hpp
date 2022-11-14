

#define APDS9960_SDA    4  //GPIO4 (D2)
#define APDS9960_SCL    5  //GPIO5 (D1)

const byte APDS_INT  = 12; //GPIO12 (D6)
volatile bool isr_flag = 0;


SparkFun_APDS9960 apds = SparkFun_APDS9960();



void startupAPDS(){

  if ( apds.init() ) {
    // Serial.println(F("APDS-9960 initialization complete"));
  } else {
    // Serial.println(F("Something went wrong during APDS-9960 init!"));
  }
  
  // Start running the APDS-9960 gesture sensor engine
  if ( apds.enableGestureSensor(true) ) {
    // Serial.println(F("Gesture sensor is now running"));
  } else {
    // Serial.println(F("Something went wrong during gesture sensor init!"));
  }

  apds.setGestureGain(2);
}

