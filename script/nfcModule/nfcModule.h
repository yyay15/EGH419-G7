/* Lenny the Learning Buddy
 * nfcModule.h
 * EGH419 - G7 2021
 * Yu Yi Alan Yu
 * 31/03/2020
 */

#ifndef nfcModule
#define nfcModule

#include <Arduino.h>
#include <Wire.h>
#include <PN532_I2C.h>
#include <PN532.h>
#include <NfcAdapter.h>

class nfcReader {
  public:
  
    // nfc Object definition
    nfcReader(TwoWire &pn532_i2c);

    // Tag Data
    NfcTag tag;

  private:



};

#endif
