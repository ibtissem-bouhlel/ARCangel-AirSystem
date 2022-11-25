   //*****************MQ sensors ***********************

void is_CO () {

  ppm7=mq7.readPpm();

Serial.print("CO =");
  Serial.print(ppm7);
  Serial.print (" ppm ");
  if(ppm7 < 20 ){
  Serial.println(" good");
  }
  else if (20 <= ppm7 <= 40){
    Serial.println(" unhealthy air ");
     
   }
  else {
    Serial.println(" Air TOXIC ");
  }
      
    
}

  //mq135 
  void is_mq135 () {
  ppm_135 = gasSensor.getPPM();
  
 Serial.print ("Air quality =");
Serial.print(ppm_135);
 Serial.print ("  ppm");
  if (ppm_135 < 300){
    Serial.println (" : GOOD air");
   }
   else if (300 <= ppm_135 <= 500){
    Serial.println(" : Unhealthy air  ");
   }
   else {
    Serial.println(" : Air quality is TOXIC ");
   }

  }
  
//mq8
void is_hydrogene () {
      MQ8ppm= MQGetGasPercentage(MQRead(MQ_PIN)/Ro,GAS_H2); //H2 in ppm

  Serial.print("Hydrogene ="); 
   Serial.print(MQ8ppm);     
   Serial.print( "ppm " );
   if (MQ8ppm < 50) {
    Serial.println (" hydrogene Normal "); }
    else if (50 <= MQ8ppm <=80) {
          Serial.println ("hydrogene elevée "); 
 }
 else {
      Serial.println ("air toxic "); 
}
   Serial.print("\n");
    
}
//mq2
void is_mq2() {
  uint16_t valMQ = MQGetGasPercentage2(MQRead2(MQ_SENSOR_ANALOG_PIN2)/Ro2,GAS_CO);
  //Serial.println(val2);

  Natural_gas=MQGetGasPercentage2(MQRead2(MQ_SENSOR_ANALOG_PIN2)/Ro2,GAS_LPG);

 /* 
  Serial.print("Natural Gas:");
 Serial.print(Natural_gas);
  Serial.println( " ppm" );
  if (Natural_gas < 50) {
      Serial.println( " " );
    }
    else if (50 <= Natural_gas <= 100) {
    Serial.println( " unhealthy air : Gas detected" );
    }
    else {
      Serial.println( " Air toxic" );
    }
  
  //Serial.print("CO:");
  //Serial.print(MQGetGasPercentage2(MQRead2(MQ_SENSOR_ANALOG_PIN2)/Ro2,GAS_CO) );
  //Serial.println(" ppm" );
  Serial.print("SMOKE:"); 
  smoke=MQGetGasPercentage2(MQRead2(MQ_SENSOR_ANALOG_PIN2)/Ro2,GAS_SMOKE);
  Serial.print( smoke);
  Serial.println( " ppm" );
  if (smoke > 200) {
    Serial.println( " Smoke detected " );
  }
*/
  
}

float MQResistanceCalculation2(int raw_adc)
{
  return ( ((float)RL_VALUE2*(1023-raw_adc)/raw_adc));
}

/***************************** MQCalibration ****************************************
Input:   mq_pin - analog channel
Output:  Ro of the sensor
Remarks: This function assumes that the sensor is in clean air. It use
         MQResistanceCalculation to calculates the sensor resistance in clean air
         and then divides it with RO_CLEAN_AIR_FACTOR. RO_CLEAN_AIR_FACTOR is about
         10, which differs slightly between different sensors.
************************************************************************************/
float MQCalibration2(int mq_pin2)
{
  int i;
  float inVal=0;

  for (i=0; i<CALIBARAION_SAMPLE_TIMES2; i++) {          //take multiple samples
    inVal += MQResistanceCalculation2(analogRead(mq_pin2));
    delay(CALIBRATION_SAMPLE_INTERVAL2);
  }
  inVal = inVal/CALIBARAION_SAMPLE_TIMES2;                   //calculate the average value

  inVal = inVal/RO_CLEAN_AIR_FACTOR2;                        //divided by RO_CLEAN_AIR_FACTOR yields the Ro
  //according to the chart in the datasheet

  return inVal;
}
/*****************************  MQRead *********************************************
Input:   mq_pin - analog channel
Output:  Rs of the sensor
Remarks: This function use MQResistanceCalculation to calculate the sensor resistance (Rs).
         The Rs changes as the sensor is in the different concentration of the target
         gas. The sample times and the time interval between samples could be configured
         by changing the definition of the macros.
************************************************************************************/
float MQRead2(int mq_pin2)
{
  int i;
  float rs=0;

  for (i=0; i<READ_SAMPLE_TIMES2; i++) {
    rs += MQResistanceCalculation2(analogRead(mq_pin2));
    delay(READ_SAMPLE_INTERVAL2);
  }

  rs = rs/READ_SAMPLE_TIMES2;

  return rs;
}

int MQGetGasPercentage2(float rs_ro_ratio2, int gas_id)
{
  if ( gas_id == GAS_LPG ) {
    return MQGetPercentage2(rs_ro_ratio2,LPGCurve);
  } else if ( gas_id == GAS_CO ) {
    return MQGetPercentage2(rs_ro_ratio2,COCurve);
  } else if ( gas_id == GAS_SMOKE ) {
    return MQGetPercentage2(rs_ro_ratio2,SmokeCurve);
  }

  return 0;
}

int  MQGetPercentage2(float rs_ro_ratio2, float *pcurve)
{
  return (pow(10,( ((log(rs_ro_ratio2)-pcurve[1])/pcurve[2]) + pcurve[0])));
}

//mq8
float MQResistanceCalculation(int raw_adc)
{
  return ( ((float)RL_VALUE*(1023-raw_adc)/raw_adc));
}

/***************************** MQCalibration ****************************************
Input:   mq_pin - analog channel
Output:  Ro of the sensor
Remarks: This function assumes that the sensor is in clean air. It use  
         MQResistanceCalculation to calculates the sensor resistance in clean air 
         and then divides it with RO_CLEAN_AIR_FACTOR. RO_CLEAN_AIR_FACTOR is about 
         10, which differs slightly between different sensors.
************************************************************************************/ 
float MQCalibration(int mq_pin)
{
  int i;
  float val=0;

  for (i=0;i<CALIBARAION_SAMPLE_TIMES;i++) {            //take multiple samples
    val += MQResistanceCalculation(analogRead(mq_pin));
    delay(CALIBRATION_SAMPLE_INTERVAL);
  }
  val = val/CALIBARAION_SAMPLE_TIMES;                   //calculate the average value

  val = val/RO_CLEAN_AIR_FACTOR;                        //divided by RO_CLEAN_AIR_FACTOR yields the Ro 
                                                        //according to the chart in the datasheet 
  return val; 
}
/*****************************  MQRead *********************************************
Input:   mq_pin - analog channel
Output:  Rs of the sensor
Remarks: This function use MQResistanceCalculation to caculate the sensor resistenc (Rs).
         The Rs changes as the sensor is in the different consentration of the target
         gas. The sample times and the time interval between samples could be configured
         by changing the definition of the macros.
************************************************************************************/ 
float MQRead(int mq_pin)
{
  int i;
  float rs=0;

  for (i=0;i<READ_SAMPLE_TIMES;i++) {
    rs += MQResistanceCalculation(analogRead(mq_pin));
    delay(READ_SAMPLE_INTERVAL);
  }

  rs = rs/READ_SAMPLE_TIMES;

  return rs;  
}
/*****************************  MQGetGasPercentage **********************************
Input:   rs_ro_ratio - Rs divided by Ro
         gas_id      - target gas type
Output:  ppm of the target gas
Remarks: This function passes different curves to the MQGetPercentage function which 
         calculates the ppm (parts per million) of the target gas.
************************************************************************************/ 
int MQGetGasPercentage(float rs_ro_ratio, int gas_id)
{
  if ( gas_id == GAS_H2) {
     return MQGetPercentage(rs_ro_ratio,H2Curve);
  }  
  return 0;
}
/*****************************  MQGetPercentage **********************************
Input:   rs_ro_ratio - Rs divided by Ro
         pcurve      - pointer to the curve of the target gas
Output:  ppm of the target gas
Remarks: By using the slope and a point of the line. The x(logarithmic value of ppm) 
         of the line could be derived if y(rs_ro_ratio) is provided. As it is a 
         logarithmic coordinate, power of 10 is used to convert the result to non-logarithmic 
         value.
************************************************************************************/ 
int  MQGetPercentage(float rs_ro_ratio, float *pcurve)
{
  return (pow(10,( ((log(rs_ro_ratio)-pcurve[1])/pcurve[2]) + pcurve[0])));
}
