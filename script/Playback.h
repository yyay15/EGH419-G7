#include <Arduino.h>
#include "AudioGeneratorAAC.h"
#include "AudioOutputI2S.h"
#include "AudioFileSourcePROGMEM.h"
#include "sampleaac.h"

#include "AudioGeneratorMP3.h"
#include "AudioFileSourceSD.h"
#include "AudioGeneratorWav.h"
#include "driver/i2s.h"
#include <SD.h>





//Constants
volatile int nfcTriggered = 1;
volatile int playing = 0;
//int buttonIn = 13;
//int buttonOut = 12;


// Interrupt simulating an NFC being tagged
//void IRAM_ATTR handleInterrupt()
//  {
//    static unsigned long last_interrupt_time = 0;
//    unsigned long interrupt_time = millis();
//    // If interrupts come faster than 200ms, assume it's a bounce and ignore
//    if (interrupt_time - last_interrupt_time > 200) 
//    {
//      //Figure out which switch was triggered, and which track to play
//      if(digitalRead(buttonIn)) nfcTriggered += 1;
//      playing = 1;
//    }
//    last_interrupt_time = interrupt_time;
//  }

class Speaker {

  public:

  AudioFileSourcePROGMEM *in;
  AudioGeneratorAAC *aac;
  AudioOutputI2S *out;
  AudioGeneratorMP3 *mp3;
  AudioFileSourceSD *file;
  AudioGeneratorWAV *wav;
  
  

  void GeneralSetup(){
//    pinMode(buttonIn, INPUT);
//    pinMode(buttonOut, OUTPUT);
//    digitalWrite(buttonOut, HIGH);
//    digitalWrite(buttonIn, LOW);
//    attachInterrupt(digitalPinToInterrupt(buttonIn),handleInterrupt,FALLING);
  }

  void WAVSetup(){
    out = new AudioOutputI2S();
    wav = new AudioGeneratorWAV();
    out -> SetPinout(26,25,27);

    // Setup SD Card
//    delay(1000);
//    Serial.print("Initializing SD card...");
//    if (!SD.begin(5))
//    {
//      Serial.println("initialization failed!");
//      return;
//    }
//    Serial.println("initialization done.");
//    delay(100);
  }

  void WAVSelectLoop(String filename){
      int strlen = filename.length()+1;
      char wavname[strlen];
      filename.toCharArray(wavname,strlen);
//      Serial.printf("attempting to play %s \n",wavname);
//      file = new AudioFileSourceSD("/error.wav");
      file = new AudioFileSourceSD(wavname);
      out -> SetGain(1); //Set the volume
      wav -> begin(file,out); //Start playing the track loaded
      //  nfcTriggered = 0;
      playing = 1;
      
//      Serial.printf("Playing track %s\n",filename);
  }
  

  // Loop to handle playing of mp3
  void WAVLoop(){
    while(playing && wav->isRunning()) {
      //Serial.println("playing 1 and wav running");
      if (!wav->loop())
      {
        wav->stop();
//        playing = 0;
        Serial.println("Stopped");
      }
    }
  }

   void AACSetup(){
  //audioLogger = &Serial;
  in = new AudioFileSourcePROGMEM(sampleaac, sizeof(sampleaac));
  aac = new AudioGeneratorAAC();
  out = new AudioOutputI2S();
  out -> SetGain(0.125);
  out -> SetPinout(26,25,27);
    
  }

  void AACSelect(){
    
  aac->begin(in, out);
  }
  void AACLoop(){
    if (aac->isRunning()) {
    aac->loop();
    } else {
      aac-> stop();
      Serial.printf("AAC done\n");
      delay(1000);
    }
    
  }
};
