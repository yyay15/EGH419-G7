#include "SdCard.h"

SdCard SDC;

void setup(){
    SDC.processCSV();
    String matchAudio = SDC.NFCtoAudio("2NFC");
    Serial.println(matchAudio);
}

void loop(){}