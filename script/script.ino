/**
 * lenny.ino
 * 
 * Firmware for Lenny the Learning Buddy
 * 
 * Vanessa Li, Alan Yu, Karl Somoray, Jarod Lam
 * Semester 1 2021
 */

#include "led.h"
#include "motor.h"
#include "sdCard.h"
#include "Playback.h"
#include "nfcModule.h"
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

// Globals
SdCard SDC; 
Speaker speaker;
nfcReader* nfcReaderVal;
LennyMicrophone mic(I2S_NUM_1, micPins, micConfig);
LEDmod led;
VibMotor vibration;

int numer = 1;

void recordingStart()
{
  Serial.println("One second passed");
  numer = 2;
  led.setStatus(numer);
  vibration.runMotor();
}
  
void setup(){
  Serial.begin(115200);
  SD.begin(5);
  // SD card setup
  Serial.println("Setting up CSV...");
  SDC.processCSV();

  // NFC setup
  nfcReaderVal = new nfcReader();
  Serial.println("Setting up NFC...");
  //delay(1000);
  while(!nfcReaderVal->startNFC()){
    Serial.println("NFC failed. Trying again...");
    nfcReaderVal->nfcReset();
    delay(500);
  }

  // Speaker and playback setup
  Serial.println("Setting up speaker...");
  speaker.WAVSetup();

  Serial.println("Setting up LED and vibration motor...");
  led.setup();
  vibration.setup();
  
  
  // Mic setup
  Serial.println("Setting up microphone...");
  mic.begin();
  pinMode(REC_BUTTON_PIN, INPUT);
}

void loop() {
  Serial.println("\nScan an NFC tag\n");
  led.setStatus(numer);
  if(nfcReaderVal->checkTag()){
    Serial.println("I scanned something!");
    numer = 3;    
    // Get UID of NFC tag
    String tagId = nfcReaderVal->returnUID();
    int strlen = tagId.length()+1;
    char tagName[strlen];
    tagId.toCharArray(tagName,strlen);

    // Create file name
    String fileName = "/" + tagId + ".wav";
    
    // Recording button pressed, record sound
    if (digitalRead(REC_BUTTON_PIN) == 1) {
      Serial.print("Recording to file ");
      Serial.println(fileName);
      
      // Record microphone
      mic.record(fileName.c_str(), REC_BUTTON_PIN, recordingStart);

      // Write record to CSV
      long start = micros(); 
      SDC.writeToCSV(tagId.c_str(), fileName.c_str());
      long timeTaken = micros() - start;
      Serial.print("SD time taken ");
      Serial.println(timeTaken);

    // Recording button not pressed, play corresponding sound file
    } else if (tagName == "no card!") {
      Serial.print("PSYCHED");
    } else {
      vibration.runMotor();
      Serial.print("Playing from file CSV search");  
      String audioFile = SDC.NFCtoAudio(tagName);
      Serial.println(audioFile);
      speaker.WAVSelectLoop(audioFile);
    }
  }
  led.setStatus(numer);
  speaker.WAVLoop();
  
  ESP.restart();
}
