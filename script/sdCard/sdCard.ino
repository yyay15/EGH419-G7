#include "SdCard.h"

SdCard SDC;

void setup(){

    SDC.writeToCSV("7NFC", "7.wav");
    SDC.processCSV();
    String matchAudio = SDC.NFCtoAudio("7NFC");
    Serial.println(matchAudio);
}

void loop(){}