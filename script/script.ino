/** Include files **/
#include "sdCard.h"
#include "Playback.h"
#include "nfcModule.h"

// NFC Libraries
#include <Wire.h>
#include <PN532_I2C.h>
#include <PN532.h>
#include <NfcAdapter.h>

/** Instantiate classes **/
SdCard SDC; 
Speaker speaker;
nfcReader nfcReaderVal;


void setup(){
// SD card set up
    // read in CSV file 
    SDC.processCSV();

// NFC set up
    nfcReaderVal.startNFC();
    
// Speaker set up
    speaker.GeneralSetup();
    speaker.WAVSetup();
// Mic set up

// testing
//Serial.println(SDC.audioFile[2]);
//String audioFile = SDC.NFCtoAudio("3NFC");
//speaker.WAVSelectLoop(audioFile);

}


void loop() {

  Serial.println("\nScan a NFC tag\n");
  if(nfcReaderVal.checkTag()){
        String tagId = nfcReaderVal.returnUID();
        int strlen = tagId.length()+1;
        char tagName[strlen];
        tagId.toCharArray(tagName,strlen);
        
        Serial.println(tagName);
        Serial.println(SDC.audioFile[2]);
        String audioFile = SDC.NFCtoAudio(tagName);
        speaker.WAVSelectLoop(audioFile);
    }



    speaker.WAVLoop();
    delay(2000);
    // if (nfc code true) {
    //     NFC_code = getNFC?? 
    //     if (button pressed) {
    //         do microphone record stuff
    //         fileName = writeAudioToSD()
    //         writeToCSV(fileName, NFC_code)
    //         hashmap = updateHashmap
    //     } else {
    //         audioFile = NFCtoAudio(NFC_code, hashmap)   *Vanessa
    //         playAudio(audioFile)               * Karl
    //     }
    // }

}
