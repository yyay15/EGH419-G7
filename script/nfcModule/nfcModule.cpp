/* Lenny the Learning Buddy
 * nfcModule.cpp
 * EGH419 - G7 2021
 * Yu Yi Alan Yu
 * 31/03/2020
 */

// Include Header Files
#include "nfcModule.h"
#include <Arduino.h>
#include <Wire.h>
#include <PN532_I2C.h>
#include <PN532.h>
#include <NfcAdapter.h>

/* Custom Function:
 * INPUT: Wire reference to be i2c mode
 * OUTPUT: Sets up nfcReader object instance
*/
nfcReader::nfcReader(TwoWire &pn532_i2c) {

  // Create NFC Adapter object inside the class 
  NfcAdapter nfc = NfcAdapter(pn532_i2c);

}


/* Custom Function:
 * INPUT: NA
 * OUTPUT: Reads Tag and outputs to field and serial 
*/
void nfcReader::readTag() {
    if (nfc.tagPresent()){
        tag = nfc.read();
        tag.print();
    }
}
