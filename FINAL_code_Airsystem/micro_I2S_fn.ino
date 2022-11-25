
    //******************SPH noise sensor***********************

void fnI2S(){
// Read multiple samples at once and calculate the sound pressure
  int32_t samples[BLOCK_SIZE];
  int num_bytes_read = i2s_read_bytes(I2S_PORT, 
                                      (char *)samples, 
                                      BLOCK_SIZE,     // the doc says bytes, but its elements.
                                      portMAX_DELAY); // no timeout
  
  int samples_read = num_bytes_read / 8;
  if (samples_read > 0) {
    //samples_read >>= 14;
     mean = 0;
    for (int i = 0; i < samples_read; ++i) {
      mean += samples[i];
    }
    Serial.println(mean);
  }
  
}
