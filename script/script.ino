/**
 * lenny.ino
 * 
 * Firmware for Lenny the Learning Buddy
 * 
 * Vanessa Li, Alan Yu, Karl Somoray, Jarod Lam
 * Semester 1 2021
 */

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
nfcReader nfcReaderVal;
LennyMicrophone mic(I2S_NUM_1, micPins, micConfig);

void recordingStart()
{
  Serial.println("One second passed");
}
  
void setup(){
  Serial.begin(115200);
  // NFC setup
  Serial.println("starting NFC");
  while(!nfcReaderVal.startNFC()){
    Serial.println("fail nfc");
  }
  Serial.println("Finish NFC");
  // SD card setup
  Serial.println("Starting CSV");
//  SDC.processCSV();
  Serial.println("Finsih CSV");
    
  // Speaker setup
  Serial.println("Starting General");
  speaker.GeneralSetup();
  Serial.println("Finish General");
  Serial.println("Starting WAV");
  speaker.WAVSetup();
//  speaker.AACSetup();
  Serial.println("Finish WAV");
  
  // Mic setup
  mic.begin();
  pinMode(REC_BUTTON_PIN, INPUT);
}

int numer = 0;

void loop() {
//  if (numer ==0){
//    numer++;
//    speaker.AACSelect();
//  }

  Serial.println("\nScan an NFC tag\n");
  if(nfcReaderVal.checkTag()){
    Serial.println("I scanned Something");
    // Get UID of NFC tag
    String tagId = nfcReaderVal.returnUID();
    Serial.println("This is string");
    Serial.println(tagId);
    int strlen = tagId.length()+1;
    char tagName[strlen];
    tagId.toCharArray(tagName,strlen);
    Serial.println("This is char");
    Serial.println(tagName);

    // Create file name
    String fileName = "/" + tagId + ".wav";

    
    // Recording button pressed, record sound
    if (digitalRead(REC_BUTTON_PIN) == 1) {

      Serial.println("Recording mode");
      
      // Record microphone
      mic.record(fileName.c_str(), REC_BUTTON_PIN, nullptr);

      // Write record to CSV
      SDC.writeToCSV(tagId.c_str(), fileName.c_str());

    // Recording button not pressed, play corresponding sound file
    } else {
      Serial.println("Playback mode");
//      String audioFile = SDC.NFCtoAudio(fileName);
      speaker.WAVSelectLoop(fileName);
    }
  }

//    speaker.AACLoop();
  speaker.WAVLoop();
  delay(2000);
}
