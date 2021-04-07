#include <Arduino.h>
#include "AudioGeneratorAAC.h"
#include "AudioOutputI2S.h"
#include "AudioFileSourcePROGMEM.h"
#include "sampleaac.h"

#include "AudioGeneratorMP3.h"
#include "AudioFileSourceSD.h"
#include "driver/i2s.h"
#include <SD.h>

volatile int nfcTriggered = 0;
volatile int playing = 0;
int buttonIn = 13;
int buttonOut = 12;


void IRAM_ATTR handleInterrupt()
  {
    static unsigned long last_interrupt_time = 0;
    unsigned long interrupt_time = millis();
    // If interrupts come faster than 200ms, assume it's a bounce and ignore
    if (interrupt_time - last_interrupt_time > 200) 
    {
      //Figure out which switch was triggered, and which track to play
      if(digitalRead(buttonIn)) nfcTriggered += 1;
      playing = 1;
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
  }

  void MP3Loop(){
    if(playing && mp3->isRunning()) {
      if (!mp3->loop())
      {
        mp3->stop();
        playing = 0;
        Serial.println("Stopped");
      }
    }
  }
  
  void AACSetup(){
  audioLogger = &Serial;
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
