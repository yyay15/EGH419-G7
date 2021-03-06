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

volatile int nfcTriggered = 1;
volatile int playing = 1;
int buttonIn = 13;
int buttonOut = 12;

// Interrupt simulating an NFC being tagged
void IRAM_ATTR handleInterrupt()
  {
    static unsigned long last_interrupt_time = 0;
    unsigned long interrupt_time = millis();
    // If interrupts come faster than 200ms, assume it's a bounce and ignore
    if (interrupt_time - last_interrupt_time > 200) 
    {
      //Figure out which switch was triggered, and which track to play
      if(digitalRead(buttonIn)) nfcTriggered += 1;
//      playing = 0;
    }
    last_interrupt_time = interrupt_time;
  }

class Speaker {

  public:

  AudioFileSourcePROGMEM *in;
  AudioGeneratorAAC *aac;
  AudioOutputI2S *out;
  AudioGeneratorMP3 *mp3;
  AudioFileSourceSD *file;
  AudioGeneratorWAV *wav;
  
  

  void GeneralSetup(){
    pinMode(buttonIn, INPUT);
    pinMode(buttonOut, OUTPUT);
    digitalWrite(buttonOut, HIGH);
    digitalWrite(buttonIn, LOW);
    attachInterrupt(digitalPinToInterrupt(buttonIn),handleInterrupt,FALLING);
  }

  void MP3Setup(){
    out = new AudioOutputI2S();
    mp3 = new AudioGeneratorMP3();
    out -> SetPinout(26,25,27);

    // Setup SD Card
    delay(1000);
    Serial.print("Initializing SD card...");
    if (!SD.begin(33))
    {
      Serial.println("initialization failed!");
      return;
    }
    Serial.println("initialization done.");
    delay(100);
  }
  void WAVSetup(){
    out = new AudioOutputI2S();
    wav = new AudioGeneratorWAV();
    out -> SetPinout(26,25,27);

    // Setup SD Card
    delay(1000);
    Serial.print("Initializing SD card...");
    if (!SD.begin())
    {
      Serial.println("initialization failed!");
      return;
    }
    Serial.println("initialization done.");
    delay(100);
  }

  // Loop to handle action when NFC is tagged
  void MP3SelectLoop(){
      char filename[] = "/4.mp3";
      file = new AudioFileSourceSD(filename);
      out -> SetGain(0.125); //Set the volume
      mp3 -> begin(file,out); //Start playing the track loaded
      //  nfcTriggered = 0;
      playing = 1;
      
      Serial.printf("Playing track %s\n",filename);
  }
  void WAVSelectLoop(){
      char filename[] = "/3.wav";
      file = new AudioFileSourceSD(filename);
      out -> SetGain(0.125); //Set the volume
      wav -> begin(file,out); //Start playing the track loaded
      //  nfcTriggered = 0;
      playing = 1;
      
      Serial.printf("Playing track %s\n",filename);
  }
  
  // Loop to handle playing of mp3
  void MP3Loop(){
    if(playing && mp3->isRunning()) {
      Serial.println("playing 1 and mp3 running");
      if (!mp3->loop())
      {
        mp3->stop();
//        playing = 0;
        Serial.println("Stopped");
      }
    }
  }

  // Loop to handle playing of mp3
  void WAVLoop(){
    if(playing && wav->isRunning()) {
      Serial.println("playing 1 and wav running");
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
