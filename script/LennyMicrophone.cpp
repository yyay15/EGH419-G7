#include "LennyMicrophone.h"

LennyMicrophone::LennyMicrophone
(
    i2s_port_t i2sPort,
    i2s_pin_config_t i2sPins,
    i2s_config_t i2sConfig
) :
    _i2sPort(i2sPort),
    _i2sPins(i2sPins),
    _i2sConfig(i2sConfig)
{
}

void LennyMicrophone::begin()
{
    // Install and start I2S driver
    i2s_driver_install(_i2sPort, &_i2sConfig, 0, NULL);
    
    // Fixes for SPH0645
    REG_SET_BIT(I2S_TIMING_REG(_i2sPort), BIT(9));
    REG_SET_BIT(I2S_CONF_REG(_i2sPort), I2S_RX_MSB_SHIFT);

    // Set pin config
    i2s_set_pin(_i2sPort, &_i2sPins);
}

void LennyMicrophone::end()
{
    // Uninstall the I2S driver
    i2s_driver_uninstall(_i2sPort);
}

int LennyMicrophone::read(int16_t *samples, int count)
{
    // Create a buffer to store samples
    int32_t *raw_samples = (int32_t *)malloc(sizeof(int32_t) * count);

    // Read samples from I2S DMA buffer
    size_t bytes_read = 0;
    i2s_read(_i2sPort, raw_samples, sizeof(int32_t) * count, &bytes_read, portMAX_DELAY);
    int samples_read = bytes_read / sizeof(int32_t);

    // Copy raw_samples to samples and process them
    for (int i = 0; i < samples_read; i++)
    {
        // Remove DC offset
        raw_samples[i] -= MIC_OFFSET;
        
        // Bit-shift down to 16-bit int
        // Not sifting all the way to amplify the signal
        samples[i] >>= raw_samples[i] >> 11;
    }
    
    free(raw_samples);
    return samples_read;
}

int LennyMicrophone::record(const char fileName[], uint8_t buttonPin, void(*afterOneSecond)())
{
  int16_t *samples = (int16_t *)malloc(sizeof(int16_t) * 1024);
  
  Serial.println("Start recording");
  SD.begin(5);
  begin();
  
  // Open the file on the SD card
  File f = SD.open(fileName, FILE_WRITE);
  if (!f) {
    Serial.print("Failed to open file ");
    Serial.print(fileName);
    Serial.println(" on SD card!");
    return 0;
  }
  
  // Create WAV file writer to write to SD card
  LennyWavWriter writer(f, getSampleRate());
  
  // Keep writing until button is released
  int numLoops = 0;
  while (digitalRead(buttonPin) == 1)
  {
    numLoops++;
    int samples_read = read(samples, 1024);
    Serial.println(samples[0]);

    // Wait until ~16000 samples
    if (numLoops >= 16)
    {
      if (numLoops == 16 && afterOneSecond != nullptr)
      {
        afterOneSecond();
      }
      
      writer.write(samples, samples_read);
      
      Serial.print("Wrote ");
      Serial.print(samples_read);
      Serial.println(" samples");
    }
  }
  
  // Clean up mic, writer and SD card
  end();
  int fileSize = writer.end();
  f.close();
  SD.end();
  free(samples);
  
  Serial.print(fileSize);
  Serial.println(" bytes written to file.");
  
  Serial.println("Finished recording");
}
