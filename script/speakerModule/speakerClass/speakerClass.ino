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
  obj1.MP3SelectLoop();

  obj1.AACLoop();
}
