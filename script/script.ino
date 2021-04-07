/** Include files **/
#include "sdCard.h"
#include "Playback.h"
/** Variable definitions **/

/** Instantiate classes **/
SdCard SD; 
Speaker speaker;
void setup(){
// SD card set up
    // read in CSV file 
    SD.processCSV();

// NFC set up

// Speaker set up
    speaker.GeneralSetup();
    speaker.WAVSetup();
// Mic set up

// testing
String audioFile = SD.NFCtoAudio("3NFC");
speaker.WAVSelectLoop(audioFile);

}


void loop() {

    speaker.WAVLoop();
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