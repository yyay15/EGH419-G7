#include "SdCard.h"

SdCard::SdCard(){
  return;
}

String SdCard::NFCtoAudio(const char * receiveCode) {
    return _NFCtoAudio(receiveCode, NFC_code, audioFile);
}
String SdCard::_NFCtoAudio(const char * receiveCode, String* NFC_code, String* audioFile) {
        for (int i = 0; i < 20; i++) {
            int strlen = NFC_code[i].length() + 1;
            char charNFC[strlen];
            NFC_code[i].toCharArray(charNFC, strlen);
            if (strcmp(receiveCode, charNFC) == 0) {
                return audioFile[i];
            }
        }
        return "/error.wav";
}

void SdCard::processCSV() {
    Serial.begin(115200);
    SD.begin(33);
    char csv[200];
    String lines[20];
    readCSV(SD, "/data.csv", csv);
    SD.end();
    CSVtoLine(csv, lines);
    lineToField(lines, NFC_code, audioFile);
    Serial.println(NFC_code[0]);
    Serial.println(audioFile[0]);
}

void SdCard::readCSV(fs::FS &fs, const char * path, char* line) {
    File f = fs.open(path);
    for (size_t n = 0; n < 200; n++) {
      int c = f.read();
      if ( c < 0 && n == 0) return;  // EOF
      line[n] = c;
  }
}

void SdCard::CSVtoLine(char* csv, String* lines) {
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

void SdCard::lineToField(String* lines, String* NFC_code, String* audioFile) {
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

void SdCard::writeToCSV(const char * NFC_code, const char * audioFile) {
    Serial.begin(115200);
    SD.begin(33);
    _writeToCSV(SD, "/data.csv", NFC_code, audioFile);
    //readFile(SD, "/data.csv");
    SD.end();
}

void SdCard::_writeToCSV(fs::FS &fs, const char * path, const char * NFC_code, const char * audioFile){
    String codeString = String(NFC_code);
    String fileString = String(audioFile);
    String newLine = codeString + "," + fileString;
    File file = fs.open(path, FILE_APPEND);
    file.println(newLine);

}

void SdCard::readFile(fs::FS &fs, const char * path){
    File file = fs.open(path);
    Serial.println("Read from file: ");
    while(file.available()){
    Serial.write(file.read());
    }
}
