#include "Playback.h"
#include <string>

Speaker obj1;
void setup()
{
    Serial.begin(115200);
    obj1.GeneralSetup();
//    obj1.MP3Setup();
//    obj1.MP3SelectLoop();
    obj1.WAVSetup();
    obj1.WAVSelectLoop();
}

bool loading = 0;
void loop()
{

//  obj1.MP3Loop();
  obj1.WAVLoop();
}
