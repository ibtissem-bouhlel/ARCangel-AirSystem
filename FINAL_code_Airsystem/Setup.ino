void setup() {
 Serial.begin(115200);
 
 ledcSetup(channel, freq, resolution);
  ledcAttachPin(5, channel);
 // syncSemaphore = xSemaphoreCreateBinary();
  // attachInterrupt(digitalPinToInterrupt(5), handleInterrupt, CHANGE);
 
  //wifi
  WiFi.mode(WIFI_MODE_APSTA);   
  WiFi.softAP(ssid, password );
  WiFi.begin(WIFI_SSID,WIFI_PASSWORD);
    
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi..");
    
    Serial.print("ESP32 IP on the WiFi network: ");
    Serial.println(WiFi.localIP());
  }
  Serial.print("ESP32 IP as soft AP: ");
  Serial.println(WiFi.softAPIP());    

  //firebase
  Serial.printf("Firebase Client v%s\n\n", FIREBASE_CLIENT_VERSION);
  Firebase.begin(DATABASE_URL, API_KEY);
  
  /* Assign the api key (required) */
  config.api_key = API_KEY; //

  /* Assign the user sign in credentials */
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;

  /* Assign the RTDB URL (required) */
  config.database_url = DATABASE_URL;
  config.database_url = DATABASE_URL;
  config.signer.tokens.legacy_token = "I8Q2YIHtv3qVzoiLUwHN1mH8Eoz0M7r99yAP1Vms";

  /* Assign the callback function for the long running token generation task */
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true); 
 
  
    Firebase.setInt(fbdo, "analyseurs/analyseur1/live/Buzzer",1);
    if(!Firebase.beginStream(stream,"analyseurs/analyseur1/live/Buzzer"))
    Serial.printf("sream begin error, %s\n\n", stream.errorReason().c_str());
    Firebase.setStreamCallback(stream, streamCallback, streamTimeoutCallback);
  
  
  //MQ
  Serial.println("Calibrating MQ2..");                       
  Ro2 = MQCalibration2(MQ_SENSOR_ANALOG_PIN2);         //Calibrating the sensor. Please make sure the sensor is in clean air

  Serial.println("Calibrating MQ7..");
  mq7.calibrate();    // calculates R0
  Serial.print("Calibrating MQ8...\n");  
               
  Ro = MQCalibration(MQ_PIN);   
  while (!Serial);
  Serial.println(F("BME680 test"));

  if (!bme.begin()) {
    Serial.println(F("Could not find a valid BME680 sensor"));
    while (1);
  }
   // Set up oversampling and filter initialization
  bme.setTemperatureOversampling(BME680_OS_8X);
  bme.setHumidityOversampling(BME680_OS_2X);
  bme.setPressureOversampling(BME680_OS_4X);
  bme.setIIRFilterSize(BME680_FILTER_SIZE_3);

  //tsl
    light.begin();    
  gain = 0;
  unsigned char time = 2;
  Serial.println("Set timing...");
  light.setTiming(gain,time,ms);
  Serial.println("Powerup...");
  light.setPowerUp(); 

 //dust sensor
  dustSensor.setBaseline(0.4); // set no dust voltage according to your own experiments
  dustSensor.setCalibrationFactor(1.1); // calibrate against precision instrument
  dustSensor.begin(); 

 //micro i2s
  Serial.println("Configuring I2S...");
  esp_err_t err;

  // The I2S config as per the example
  const i2s_config_t i2s_config = {
      .mode = i2s_mode_t(I2S_MODE_MASTER | I2S_MODE_RX), // Receive, not transfer
      .sample_rate = 16000,                         // 16KHz
      .bits_per_sample = I2S_BITS_PER_SAMPLE_32BIT, // could only get it to work with 32bits
      .channel_format = I2S_CHANNEL_FMT_ONLY_RIGHT, // although the SEL config should be left, it seems to transmit on right
      .communication_format = i2s_comm_format_t(I2S_COMM_FORMAT_I2S | I2S_COMM_FORMAT_I2S_MSB),
      .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,     // Interrupt level 1
      .dma_buf_count = 8,                           // number of buffers
      .dma_buf_len = BLOCK_SIZE                     // samples per buffer
  };

  // The pin config as per the setup
  const i2s_pin_config_t pin_config = {
      .bck_io_num = 14,   // BCKL
      .ws_io_num = 15,    // LRCL
      .data_out_num = -1, // not used (only for speakers)
      .data_in_num = 32   // DOUT
  };

  // Configuring the I2S driver and pins.
  // This function must be called before any I2S driver read/write operations.
  err = i2s_driver_install(I2S_PORT, &i2s_config, 0, NULL);
  if (err != ESP_OK) {
    Serial.printf("Failed installing driver: %d\n", err);
    while (true);
  }
  err = i2s_set_pin(I2S_PORT, &pin_config);
  if (err != ESP_OK) {
    Serial.printf("Failed setting pin: %d\n", err);
    while (true);
  }

   //mhz
  co2Serial.begin(9600);
  //neopixel
   pixels.begin(); // This initializes the NeoPixel library.

  


}
