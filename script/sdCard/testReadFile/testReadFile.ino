#include "FS.h"
#include "SD.h"
#include "SPI.h"
#include <stdio.h>
#include <string.h>
 
void readFile(fs::FS &fs, const char * path){
    Serial.printf("Reading file: %s\n", path);
    
    File file = fs.open(path);
    if(!file){
    Serial.println("Failed to open file for reading");
    return;
    }
    
    Serial.print("Read from file: ");
    while(file.available()){
    Serial.write(file.read());
    Serial.println("read");
    }
}

// char* readLine(fs::FS &fs, const char * path){
//     char lines[40];
//     File file = fs.open(path);
//     int j = 0;
//     while (file.available()) {
//       int c = file.read();
//       if (c < 0 || c == '\n') {
//         return &lines;
//       } else {
//         lines[j] = c;
//         j++;
//       }
//     }
//     return lines;
// }

void readLine(fs::FS &fs, const char * path, String* lines) {
    File file = fs.open(path);
    char line[40], *ptr, *str;
    if (!singleLine(file, line, 40)) {
    }
    lines[1] = line;
}

bool singleLine(File &f, char* line, size_t maxLen) {
    for (size_t n = 0; n < maxLen; n++) {
    int c = f.read();
    if ( c < 0 && n == 0) return false;  // EOF
    if (c < 0 || c == '\n') {
      line[n] = 0;
      return true;
    }
    line[n] = c;
  }
  return false; // line too long
}


void readCSV(fs::FS &fs, const char * path, char* line) {
    File f = fs.open(path);
    for (size_t n = 0; n < 200; n++) {
      int c = f.read();
      if ( c < 0 && n == 0) return;  // EOF
      line[n] = c;
  }
}

void CSVtoLine(char* csv, String* lines) {
    int iter = 0;
    char *singleLine;
    char * d = "\n";
    singleLine = strtok(csv, d);
    while (singleLine != NULL) {
        lines[iter] = singleLine;
        singleLine = strtok(NULL, d);
        iter++;
    }
}

void linetoField(String* lines, String* NFC_code, String* audioFile) {
    for (size_t i = 0; i < 20; i++) {
        int strlen = lines[i].length() + 1;
        char line[strlen];
        lines[i].toCharArray(line, strlen);
        char *singleField;
        char *d = ",";
        singleField = strtok(line, d);
        NFC_code[i] = singleField;
        singleField = strtok(NULL, d);
        audioFile[i] = singleField;
    }
}


 
void writeFile(fs::FS &fs, const char * path, const char * message){
    Serial.printf("Writing file: %s\n", path);
    
    File file = fs.open(path, FILE_WRITE);
    if(!file){
    Serial.println("Failed to open file for writing");
    return;
    }
    if(file.print(message)){
    Serial.println("File written");
    } else {
    Serial.println("Write failed");
    }
}
 
void appendFile(fs::FS &fs, const char * path, const char * message){
    Serial.printf("Appending to file: %s\n", path);
    
    File file = fs.open(path, FILE_APPEND);
    if(!file){
    Serial.println("Failed to open file for appending");
    return;
    }
    if(file.print(message)){
    Serial.println("Message appended");
    } else {
    Serial.println("Append failed");
    }
}

String NFCtoAudio(const char * receiveCode, String* NFC_code, String* audioFile) {
        for (int i = 0; i < 20; i++) {
            int strlen = NFC_code[i].length() + 1;
            char charNFC[strlen];
            NFC_code[i].toCharArray(charNFC, strlen);
            if (strcmp(receiveCode, charNFC) == 0) {
                return audioFile[i];
            }
        }
}

void setup(){
    Serial.begin(115200);
    if(!SD.begin()){
    Serial.println("Card Mount Failed");
    return;
    }
    uint8_t cardType = SD.cardType();
    
    if(cardType == CARD_NONE){
    Serial.println("No SD card attached");
    return;
    }


    char csv[200];
    readCSV(SD, "/data.csv", csv);
    String lines[20];
    CSVtoLine(csv, lines);
    String NFC_code[20];
    String audioFile[20];
    linetoField(lines, NFC_code, audioFile);
    // Serial.println(NFC_code[2]);
    // Serial.println(audioFile[2]);
    String matchFile;
    matchFile = NFCtoAudio("3NFC", NFC_code, audioFile);
    Serial.println(matchFile);
}
 
void loop(){
 
}