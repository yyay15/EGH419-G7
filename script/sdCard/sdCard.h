#define SDCARD_H
//https://github.com/arduino-libraries/SD
// for audio playback https://github.com/TMRh20/TMRpcm
// https://forum.arduino.cc/index.php?topic=231631.0 read csv file
#include "SD.h"
#include "FS.h"
#include "SPI.h"
#include <stdio.h>
#include <string.h>

class SdCard {
    public:
        SdCard(); // initiialise SD card things 
        String NFCtoAudio(const char * receiveCode);
        void processCSV();
    private:
        String NFC_code[20];
        String audioFile[20];
        void readCSV(fs::FS &fs, const char * path, char* line);
        void CSVtoLine(char* csv, String* lines);
        void lineToField(String* lines, String* NFC_code, String* audioFile);
        
        String _NFCtoAudio(const char * receiveCode, String* NFC_code, String* audioFile);

};
