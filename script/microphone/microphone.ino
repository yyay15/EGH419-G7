#include <stdio.h>
#include <FreeRTOS.h>
#include <FS.h>
#include <SD.h>
#include <SPI.h>

#include "LennyMicrophone.h"

// Microphone settings
#define MIC_SAMPLE_RATE 16000
#define MIC_BCLK_PIN GPIO_NUM_32
#define MIC_LRCL_PIN GPIO_NUM_33
#define MIC_DOUT_PIN GPIO_NUM_35

// Recording button settings
#define REC_BUTTON_PIN 37

// Microphone setup
i2s_config_t micConfig = {
  .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX),
  .sample_rate = MIC_SAMPLE_RATE,
  .bits_per_sample = I2S_BITS_PER_SAMPLE_32BIT,
  .channel_format = I2S_CHANNEL_FMT_ONLY_LEFT,
  .communication_format = I2S_COMM_FORMAT_I2S,
  .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
  .dma_buf_count = 4,
  .dma_buf_len = 1024,
  .use_apll = false,
  .tx_desc_auto_clear = false,
  .fixed_mclk = 0
};
i2s_pin_config_t micPins = {
  .bck_io_num = MIC_BCLK_PIN,
  .ws_io_num = MIC_LRCL_PIN,
  .data_out_num = I2S_PIN_NO_CHANGE,
  .data_in_num = MIC_DOUT_PIN
};
LennyMicrophone mic(I2S_NUM_0, micPins, micConfig);

void recordingStart()
{
  Serial.println("One second passed");
}

void setup()
{
  Serial.begin(115200);

  mic.begin();

  if (!SD.begin())
  {
    Serial.println("Failed to mount SD card!");
  }

  pinMode(REC_BUTTON_PIN, INPUT);
}

void loop()
{
  while (digitalRead(REC_BUTTON_PIN) == 0) {
    delay(100);
  }
  mic.record("/test.wav", REC_BUTTON_PIN, recordingStart);
  delay(1000);
}
