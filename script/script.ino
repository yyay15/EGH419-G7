/** Include files **/
#include "sdCard.h"
#include "Playback.h"

// NFC Libraries
#include <Wire.h>
#include <PN532_I2C.h>
#include <PN532.h>
#include <NfcAdapter.h>

/** Variable definitions **/
PN532_I2C pn532_i2c(Wire);

// NFC Initialise
NfcAdapter nfc = NfcAdapter(pn532_i2c);


/** Instantiate classes **/
SdCard SDC; 
Speaker speaker;
void setup(){
// SD card set up
    // read in CSV file 
    SDC.processCSV();

// NFC set up
    Serial.println("NDEF Reader");
    nfc.begin();
    
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
    if (nfc.tagPresent()){
        NfcTag tag = nfc.read();
        String tagId = tag.getUidString();

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
