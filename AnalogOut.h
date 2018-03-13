#ifndef AnalogOut_h
#define AnalogOut_h

class AnalogOut {

  private:
    int pin;
    boolean state; //HIGH OR LOW
    int analogState;
    unsigned long timeForOneStep; //0 - 255 steps
    unsigned long timeOfLastStep;
    int pointsPerStep;

    
  public:
    AnalogOut(int inPin, unsigned long runTime, int inPointsPerStep) {  //points per step - 1 - 256
      pointsPerStep = inPointsPerStep;
      pin = inPin;
      pinMode(pin, OUTPUT);
      digitalWrite(pin, false);
      timeForOneStep = (double)runTime / (256. / pointsPerStep);
      timeOfLastStep = 0;
      analogState = 0;
      state = false;
    }

    void setState(boolean newState) {
      state = newState;
    }

    void setHardState(boolean newState) {
      state = newState;
      digitalWrite(pin, state);
      if(newState) {
        analogState = 255;
      }
      else {
        analogState = 0;
      }
    }

    void Update() {
      if(state && analogState < 255 && millis() - timeOfLastStep >= timeForOneStep) {
        timeOfLastStep = millis();
        analogState+=pointsPerStep;
      }
      if(!state && analogState > 0 && millis() - timeOfLastStep >= timeForOneStep) {
        timeOfLastStep = millis();
        analogState-=pointsPerStep;
      }
      if(analogState < 0) {
        analogState = 0;
      }
      if(analogState > 255) {
        analogState = 255;
      }
      analogWrite(pin, analogState);
    }

    int getState() {
      return analogState;
    }
};

#endif
