/* Lenny the Learning Buddy
 * nfcModule.h
 * EGH419 - G7 2021
 * Yu Yi Alan Yu
 * 31/03/2020
 */

#ifndef nfcModule
#define nfcModule


#include <DFRobot_PN532.h>

#define  BLOCK_SIZE      16
#define  PN532_IRQ      (2)
#define  INTERRUPT      (1)
#define  POLLING        (0)



class nfcReader {
  public:
    DFRobot_PN532_IIC *nfcObject;
    struct card nfcScannedTag;

    nfcReader(){
      nfcObject = new DFRobot_PN532_IIC(PN532_IRQ, POLLING);
      // NfcAdapter nfcObject = NfcAdapter(pn532_i2c);
    }

    ~nfcReader() {
      delete nfcObject;
    }

    void nfcReset(){
      pinMode(15,OUTPUT);
      digitalWrite(15,HIGH);
      delay(200);
      digitalWrite(15,LOW);
      delay(200);
      digitalWrite(15,HIGH);
    }
  
    bool startNFC(){
      return nfcObject->begin();
    }

    boolean checkTag(){
      return (nfcObject->scan()); 
    }

  /* Read Tag function
  */
  String returnUID(){
    return (nfcObject->readUid()); 
  }


};

#endif
