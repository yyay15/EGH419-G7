#define SDCARD_H

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
        void writeToCSV(const char * NFC_code, const char * audioFile);
        String audioFile[20];
    private:
        String NFC_code[20];
        void readCSV(fs::FS &fs, const char * path, char* line);
        void CSVtoLine(char* csv, String* lines);
        void lineToField(String* lines, String* NFC_code, String* audioFile);
        String _NFCtoAudio(const char * receiveCode, String* NFC_code, String* audioFile);
        void _writeToCSV(fs::FS &fs, const char * path, const char * NFC_code, const char * audioFile);
        void readFile(fs::FS &fs, const char * path);
};
