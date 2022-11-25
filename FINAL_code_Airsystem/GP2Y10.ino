   //*****************Dust Sensor ************************

 void isPFsensor(){
 Serial.print("Dust density: ");
 Serial.print(dustSensor.getDustDensity());
 Serial.print(" ug/m3");

 dust = dustSensor.getRunningAverage();
 smoke = dustSensor.getDustDensity();
  if (dust < 300) {
     Serial.println(" good ");
  }
  else if (300 <= dust <=500) {
     Serial.println("  Unhealthy ");
  }
  else{
  Serial.println("Toxic : ");
  }
  Serial.print(" Running average: ");
  Serial.print(dustSensor.getRunningAverage());
  Serial.println(" ug/m3");
  
  }
