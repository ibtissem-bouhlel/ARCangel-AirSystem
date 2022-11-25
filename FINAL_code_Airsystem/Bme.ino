
   //******************bme************************
  void isBME() {
  if (! bme.performReading()) {
    Serial.println("Failed to perform reading :(");
    return;
    }
    temp= bme.temperature;
    hum=bme.humidity;
    pressure=bme.pressure/100.0;
      
    Serial.print(F("Temperature = "));
    Serial.print(bme.temperature);
    Serial.println(F(" °C"));
   
    Serial.print(F("Humidity = "));
    Serial.print(bme.humidity);
    Serial.println(F(" %"));
   
    Serial.print(F("Pressure = "));
    Serial.print(bme.pressure/ 100.0);
    Serial.println(F(" hPa"));
    Serial.print("\n");
    

   

}
