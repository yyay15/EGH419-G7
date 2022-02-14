#include <Adafruit_NeoPixel.h>

class LEDmod {

  public:
  float vals[3] = {0,0,0};
  int ledPin = 14;
  Adafruit_NeoPixel pixels = Adafruit_NeoPixel(1, ledPin, NEO_GRB + NEO_KHZ800);

  void updateLED(){
    pixels.setPixelColor(0, pixels.Color(vals[0] * 255, vals[1] * 255, vals[2] * 255)); // Moderately bright green color.
    pixels.show(); // This sends the updated pixel color to the hardware.
  }
  void setup(){
    pixels.begin();
    pinMode(ledPin, OUTPUT);
  }

  void setVals(float R, float G, float B){
      vals[0] = G;
      vals[1] = R;
      vals[2] = B;
  }

  void turnOn(){
    setVals(1,1,1);
    updateLED();
  }

  void turnOff(){
    setVals(0,0,0);
    updateLED();
  }

  void setColor(float R, float G, float B){
    setVals(R,G,B);
    updateLED();
  }

  void setStatus(int stat){
    switch(stat){
      case 1: //awaiting -blue
      setVals(0,0,1);
      break;

      case 2: //recording - red
      setVals(1,0,0);
      break; 

      case 3: //playing - green
      setVals(0,1,0);
      break;

      default:
      setVals(0,0,0);
      break;
    }
    updateLED();

  }

};
