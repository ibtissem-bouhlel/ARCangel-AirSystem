   //*****************MHZ19b Co2 sensor***********************

  int readCO2UART() {
  byte cmd[9] = {0xFF, 0x01, 0x86, 0x00, 0x00, 0x00, 0x00, 0x00, 0x79};
  char response[9];
  //Serial.println("Sending CO2 request...");
  co2Serial.write(cmd, 9); //request PPM CO2
  
  // clear the buffer
  memset(response, 0, 9);
  int i = 0;
  //while (co2Serial.available() == 0) {
  
      // Serial.print("Waiting for response ");
      // Serial.print(i);
       // Serial.println(" s");
   // delay(500);
    //i++;
 // }
  if (co2Serial.available() > 0) {
    co2Serial.readBytes(response, 9);
  
  }
  // print out the response in hexa
  //for (int i = 0; i < 9; i++) {
    //Serial.print(String(response[i], HEX));
  //  Serial.print("  ");
 // }
  //Serial.println("");
  // checksum

  byte check = getCheckSum(response);
  /*if (response[8] != check) {
    //Serial.println("Checksum not OK!");
   Serial.print("Received: ");
    Serial.println(response[8]);
    Serial.print("Should be: ");
    Serial.println(check);
  }  */
  // ppm
  ppm_uart = 156 *(int)response[2] + response[3];
  Serial.print("CO2 value = ");
  Serial.print(ppm_uart);
  Serial.print(" ppm ");
   if (ppm_uart < 3000){
       Serial.println("  good"); 
   
    }

   else if (3000 <= ppm_uart < 5000) {
   Serial.println(" unhealthy Air"); 
   
   }
   else {
   Serial.println(" Air TOXIC !"); 
}
  // temp
  //byte temp = response[4] - 40;
  //Serial.print("Sensor Temperature: ");
  //Serial.println(temp);
  // status
  byte status = response[5];
  //Serial.print("Status: ");
  //Serial.println(status);
  //if (status == 0x40) {
    //Serial.println("Status OK");
  //}
 
}
