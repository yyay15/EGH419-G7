#ifndef SDCARD_H
#define SDCARD_H
//https://github.com/arduino-libraries/SD
// for audio playback https://github.com/TMRh20/TMRpcm
// https://forum.arduino.cc/index.php?topic=231631.0 read csv file
#include "SD.h"

class SdCard {
    public:
        SdCard();
        bool writeCSV(const char * csvFile, const char * audioFile);
        File csvFile; 


}
