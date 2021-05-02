class VibMotor {
    public:

    int pin = 12;


    void setup()
    {
        pinMode(pin, OUTPUT);
    }

    void runMotor(){
      digitalWrite(pin,HIGH);
      delay(200);
      digitalWrite(pin,LOW);
    }

    void motorOn(){
      digitalWrite(pin,HIGH);
    }
    void motorOff(){
      digitalWrite(pin,LOW);
    }

};
