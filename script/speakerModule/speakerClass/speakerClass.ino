#include "Playback.h"
#include <string>

Speaker obj1;
void setup()
{
    
    Serial.begin(115200);
    obj1.AACSetup();
    obj1.GeneralSetup();
}

bool loading = 0;
void loop()
{
  if (nfcTriggered>0){
    if (playing){playing = 0; //STOP MUSIC
      }
      
      char filename[1024];
      sprintf(filename, "nfctest%i.mp3",nfcTriggered);
//      file = new AudioFileSourceSD(filename);
//    out -> SetGain(0.125); //Set the volume
//    mp3 -> begin(file,out); //Start playing the track loaded
//    nfcTriggered = 0;
      
      Serial.printf("Playing track %s\n",filename);
    }

  obj1.AACLoop();
   
}
