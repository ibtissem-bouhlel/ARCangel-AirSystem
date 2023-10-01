  #include <Wire.h>
  #include <SPI.h>
  #include <WiFi.h>
  #include <FirebaseESP32.h>
  #include <WiFiClient.h>
  #include <WiFiAP.h>

  #define API_KEY "I8Q2YIHtv3qVzoiLUwHN1mH8Eoz0M7r99yAP1Vms"
  #define DATABASE_URL "https://analyseur-air-arcangel.firebaseio.com/"

/* Define the Firebase Data object */
  #define USER_EMAIL "Ibtissem38@gmail.com"
  #define USER_PASSWORD "xxxxxxx"

  /*Define Firebase Data object */
  FirebaseData fbdo;
  FirebaseData stream;

  FirebaseAuth auth;
  FirebaseConfig config;


  unsigned long sendDataPrevMillis = 0;
  int count = 0;
  uint32_t idleTimeForStream = 15000;

/* Define the WiFi credentials */
  const char *WIFI_SSID = "xxxxxxx";  
  const char *WIFI_PASSWORD = "xxxxxxxxx" ;
/*esp access point  */
  const char *ssid = "ESP32_AP";
  const char *password = "12345678";

  /* mq8 hydrogene */
  #define         MQ_PIN                      (35)     //define which analog input channel you are going to use
  #define         RL_VALUE                     (10)    //define the load resistance on the board, in kilo ohms
  #define         RO_CLEAN_AIR_FACTOR          (9.21)  //RO_CLEAR_AIR_FACTOR=(Sensor resistance in clean air)/RO,
                                                     //which is derived from the chart in datasheet
  #define         CALIBARAION_SAMPLE_TIMES     (20)    //define how many samples you are going to take in the calibration phase
  #define         CALIBRATION_SAMPLE_INTERVAL  (200)   //define the time interal(in milisecond) between each samples in the
                                                     //cablibration phase
  #define         READ_SAMPLE_INTERVAL         (50)    //define how many samples you are going to take in normal operation
  #define         READ_SAMPLE_TIMES            (5)     //define the time interal(in milisecond) between each samples in 
                                                     //normal operation
  #define         GAS_H2                      (0)
  float           H2Curve[3]  =  {2.3, 0.93,-1.44};    //two points are taken from the curve in datasheet. 
  float MQ8ppm;                                                 //with these two points, a line is formed which is "approximately equivalent" 
                                                     //to the original curve.                                                    //data format:{ x, y, slope}; point1: (lg200, lg8.5), point2: (lg10000, lg0.03) 
  float Ro=  10;                  //Ro is initialized to 10 kilo ohms
  float  Natural_gas;
  // mQ7
  #include <MQ7.h>
  #define A_PIN 34
  #define VOLTAGE 5
  float ppm7;
  MQ7 mq7(A_PIN, VOLTAGE); // init MQ7 device

  //*********mq2
  #define   MQ_SENSOR_ANALOG_PIN2        33 //define which analog input channel you are going to use
  #define         RL_VALUE2                     (5)     //define the load resistance on the board, in kilo ohms
  #define         RO_CLEAN_AIR_FACTOR2          (9.83)  //RO_CLEAR_AIR_FACTOR=(Sensor resistance in clean air)/RO, from datasheet
  #define         CALIBARAION_SAMPLE_TIMES2     (50)    //define how many samples you are going to take in the calibration phase
  #define         CALIBRATION_SAMPLE_INTERVAL2  (500)   //define the time interval(in milliseconds) between each samples in the
  //calibration phase
  #define         READ_SAMPLE_INTERVAL2         (50)    //define how many samples you are going to take in normal operation
  #define         READ_SAMPLE_TIMES2            (5)     //define the time interval(in milliseconds) between each samples in
  //normal operation
  #define         GAS_LPG                      (0)
  #define         GAS_CO                       (1)
  #define         GAS_SMOKE                    (2)
  //VARIABLES
  float Ro2 = 10.0;    // this has to be tuned 10K Ohm
  int val2 = 0;           // variable to store the value coming from the sensor
  uint16_t lastMQ = 0;
  float  LPGCurve[3]  =  {0.27,1.25,-0.47};   //two points are taken from the curve.
  //with these two points, a line is formed which is "approximately equivalent"
  //to the original curve.
  //data format:{ x, y, slope}; point1: (lg200, 0.21), point2: (lg10000, -0.59)
  float COCurve[3]  =  {1.35,1.51,-0.34};    //two points are taken from the curve.
  //with these two points, a line is formed which is "approximately equivalent"
  //to the original curve.
  //data format:{ x, y, slope}; point1: (lg200, 0.72), point2: (lg10000,  0.15)
  float SmokeCurve[3] = {0.6,1.62,-0.44};   //two points are taken from the curve. //data format:{ x, y, slope}; point1: (lg200, 0.53), point2:(lg10000,-0.22)

  //mhz co2 sensor
  #include <SoftwareSerial.h>
  SoftwareSerial co2Serial(16, 17); // define MH-Z19 RX TX D3 (GPIO0) and D4 (GPIO2)
  unsigned long startTime = millis();

  //mq135
  #include <MQ135.h>
  int ANALOGPIN=39; //pin VN
  MQ135 gasSensor = MQ135(ANALOGPIN);
  float ppm_135;

  //bme
  #include <Adafruit_Sensor.h>
  #include "Adafruit_BME680.h"
  #define SEALEVELPRESSURE_HPA (1013.25)
  Adafruit_BME680 bme; // i2c
  float temp;
  float pressure;
  float hum;

  //tsl
  #include <SparkFunTSL2561.h> //luminosity
  SFE_TSL2561 light;  //i2c
  char response[9];
  //light.manualStart();
  boolean gain;
  unsigned int ms;
  unsigned int data0,data1;
  double lux;
  float dust, smoke;

  int ppm_uart;
  unsigned long dataMillis = 0;
  float mean;

  //micro i2s
  #include <driver/i2s.h>
  const i2s_port_t I2S_PORT = I2S_NUM_0;
  const int BLOCK_SIZE = 1024;

  // Buzzer
  int freq = 4000;
  int channel = 0;
  int resolution = 8;
  String etat_buzzer= "1";

  // dust sensor
  #include <GP2YDustSensor.h>
  const uint8_t SHARP_LED_PIN = 12;   // Sharp Dust/particle sensor Led Pin
  const uint8_t SHARP_VO_PIN = 36;    // Sharp Dust/particle analog out pin used for reading 
  GP2YDustSensor dustSensor(GP2YDustSensorType::GP2Y1010AU0F, SHARP_LED_PIN, SHARP_VO_PIN);

  //neopixel
  #include <Adafruit_NeoPixel.h>
  #define PIN 23
  #define NUMPIXELS 12
  Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

  void streamCallback(StreamData data)
  {
                etat_buzzer=data.payload();       
  //Serial.printf("Received stream payload size: %d (Max. %d)\n\n", data.payloadLength(), data.maxPayloadLength());
  }
  void streamTimeoutCallback(bool timeout)
  {
  if (timeout)
  Serial.println("stream timed out, resuming...\n");

  if (!stream.httpConnected())
   Serial.printf("error code: %d, reason: %s\n\n", stream.httpCode(), stream.errorReason().c_str());
  }
  void loop(){
  is_hydrogene ();
  is_mq135 ();
  is_CO ();
  is_mq2();
  isTSL();
  isBME();
  isNeopixel();
  isPFsensor();
  fnI2S();
  //ppm_uart = readCO2UART(); 
  readCO2UART();


   if (Firebase.ready())
  {
    Firebase.setString(fbdo, "analyseurs/analyseur1/live/Temperature",temp);
    Firebase.setString(fbdo, "analyseurs/analyseur1/live/Humidity",hum);
    Firebase.setString(fbdo, "analyseurs/analyseur1/live/Pressure",pressure);
    Firebase.setString(fbdo, "analyseurs/analyseur1/live/Smoke",smoke);
    //Firebase.setFloat(fbdo, "analyseurs/analyseur1/live/Co",CO );
    Firebase.setString(fbdo, "analyseurs/analyseur1/live/CO",ppm7);
    Firebase.setString(fbdo, "analyseurs/analyseur1/live/AirQuality",ppm_135);
    Firebase.setString(fbdo, "analyseurs/analyseur1/live/Hydrogene",MQ8ppm);
    Firebase.setString(fbdo, "analyseurs/analyseur1/live/Luminosity",lux);
    Firebase.setString(fbdo, "analyseurs/analyseur1/live/DustDensity",dustSensor.getRunningAverage());
    Firebase.setString(fbdo, "analyseurs/analyseur1/live/Co2", ppm_uart);
    Firebase.setString(fbdo, "analyseurs/analyseur1/live/Noise",mean);
   //Firebase.setString(fbdo, "analyseurs/analyseur1/live/Gaz",Natural_gas);
  }
  }
  byte getCheckSum(char *packet) {
  byte i;
  unsigned char checksum = 0;
  for (i = 1; i < 8; i++) {
    checksum += packet[i];
  } 
  checksum = 0xff - checksum;
  checksum += 1;
  return checksum;
}
