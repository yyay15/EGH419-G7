#pragma once

#include <freertos/FreeRTOS.h>
#include <driver/i2s.h>

#include "LennyWavWriter.h"

// DC offset of the signal as signed 32-bit int
#define MIC_OFFSET -97648640

class LennyMicrophone
{

public:

  /**
   * Create a LennyMicrophone object to read from an external I2S MEMS microphone.
   * 
   * @param i2s_port The ESP32 I2S port to use. 0 or 1.
   * @param i2s_pins The pin configuration struct to use for this channel.
   * @param i2s_config The I2S channel configuration to use.
   */
  LennyMicrophone
  (
    i2s_port_t i2sPort,
    i2s_pin_config_t i2sPins,
    i2s_config_t i2sConfig
  );

  /**
   * Initialise the I2S input.
   */
  void begin();

  /**
   * Disable the I2S input.
   */
  void end();

  /**
   * Read microphone samples into a buffer of 16-bit signed ints.
   * 
   * @param samples Pointer to the sample buffer array.
   * @param count Size of the sample buffer.
   * 
   * @return Number of samples read.
   */
  int read(int16_t* samples, int count);

  /**
   * Get sample rate of the microphone input.
   */
  int getSampleRate()
  {
    return _i2sConfig.sample_rate;
  }

  /**
   * Record microphone samples to SD card until a button is released.
   * Waits for one second before recording and runs function afterOneSecond when recording starts.
   * This is a hack to bypass weird distortion coming from the mic.
   * 
   * @param fileName Path to destination file on SD card.
   * @param buttonPin Digital pin of button that triggers stop recording
   * 
   * @return Number of bytes written.
   */
  int record(const char fileName[], uint8_t buttonPin, void(*afterOneSecond)());
    
private:

  i2s_port_t _i2sPort;
  
  i2s_pin_config_t _i2sPins;
  
  i2s_config_t _i2sConfig;
  
};
