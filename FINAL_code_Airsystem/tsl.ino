 //*****************************TSL 2561 LUMINOSITy sensor********************************
/*I2C :Connect SCL to LOLIN32 22
       Connect SDA to LOLIN32 21
       Connect VDD to  3.3V */

// There are two light sensors on the device, one for visible light
// and one for infrared. Both sensors are needed for lux calculations data0 and data1 
 void isTSL(){
  delay(ms);
if (light.getData(data0,data1))
{
// getData() returned true, communication was successful

Serial.print("luminosity = ");
Serial.print(data0);
// To calculate lux, pass all your settings and readings
// to the getLux() function.

boolean good; // True if neither sensor is saturated

// Perform lux calculation:
good = light.getLux(gain,ms,data0,data1,lux);

// Print out the results:
Serial.print(" lux: ");
Serial.print(lux);
if (data0 < 50){
  Serial.println(" => BAD lighting");
}
else if (data0 > 50 ){
Serial.println(" => good lighting");

}
else
{
// getData() returned false because of an I2C error, inform the user.
byte error = light.getError();
Serial.print(error);
}  
 }
 }
