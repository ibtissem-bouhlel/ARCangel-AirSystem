  void isNeopixel(){

 if ((dust < 300) and (ppm_uart < 3000)  and (ppm_135 < 300) and (ppm7 < 20 ) and (MQ8ppm < 50 ) ) { //and (Natural_gas <50)

      for(int i=0;i<NUMPIXELS;i++)       //****** VERT
  {
    // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
    pixels.setPixelColor(i, pixels.Color(0,100,0)); // Moderately bright green color.
    pixels.show(); // This sends the updated pixel color to the hardware.
     // Delay for a period of time (in milliseconds).
  }
 // xSemaphoreTake(syncSemaphore, portMAX_DELAY);
 if (etat_buzzer =="0"){
        ledcWriteTone(channel,0);

  }
else if (etat_buzzer =="1") {
        ledcWriteTone(channel,0);
        delay (1000);
  }

 }
 
  else if ((dust > 500)or (ppm_uart > 5000) or(ppm_135 > 500)or (ppm7 > 40)  or (MQ8ppm >80)) //or (smoke > 200)or (Natural_gas > 50)
  {
    
 for(int i=0;i<NUMPIXELS;i++)  // ********** ROUGE
  {
    // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
    pixels.setPixelColor(i, pixels.Color(255,0,0)); // Moderately bright green color.
    pixels.show(); // This sends the updated pixel color to the hardware.
  }

 if (etat_buzzer =="1"){
  for (int j=0;j<50;j++){
     ledcWriteTone(channel, freq);  
     delay(100);
     ledcWriteTone(channel,0);        
     delay(100);
  }
 }
  else if (etat_buzzer =="0") {
 ledcWriteTone(channel,0);
  }
  }
  
  else if ((300 <= dust <=500) or (3000 <= ppm_uart <= 5000)or (300 <= ppm_135 <= 500)or (20 <= ppm7 <= 40) or( 50 <= MQ8ppm<= 80) ){

    for(int i=0;i<NUMPIXELS;i++)    //******* orange
  {
    // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
    pixels.setPixelColor(i, pixels.Color(255,70,0)); // Moderately bright green color.
    pixels.show(); // This sends the updated pixel color to the hardware.
     // Delay for a period of time (in milliseconds).
  }
       if (etat_buzzer =="0"){
        ledcWriteTone(channel,0);
  }
  else if  (etat_buzzer =="1"){
        ledcWriteTone(channel,0);
  }
  }
  
  else {
      Serial.println ("blanc");

     for(int i=0;i<NUMPIXELS;i++)       //******blanc
  {
    // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
    pixels.setPixelColor(i, pixels.Color(255,255,255)); // Moderately bright green color.
    pixels.show(); // This sends the updated pixel color to the hardware.
  }
         if (etat_buzzer =="0"){
        ledcWriteTone(channel,0);
  }
          else if (etat_buzzer =="1"){
        ledcWriteTone(channel,0);
          }
    }
  }
