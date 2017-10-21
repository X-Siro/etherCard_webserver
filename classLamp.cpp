#include <TroykaCurrent.h>
#include <classLamp.h>
#define sensorDelta 0.05

byte  Lamp::lampSwitch(){
  if (getLampStatus() == HIGH) setLampStatus(LOW);
  else setLampStatus(HIGH);
}

byte  Lamp::setLampStatus(byte lampStatus){
  if (getLampStatus() != lampStatus) {
    relaySwitch();
    return(0);
  }
  return 0;
}

Lamp::Lamp(byte relayPin, uint8_t lineSensorPin):itsLineSensor(lineSensorPin)
{
  itsRelayPin = relayPin;
  pinMode(itsRelayPin , OUTPUT);
}

const byte Lamp::getLampStatus() const{
  float A = getLampAmperage();
  if((abs(getLampAmperage())-sensorDelta) > 0) {
    return HIGH;
  } else {
    return LOW;
  }
}

const float Lamp::getLampAmperage() const{
  return itsLineSensor.readCurrentAC();
}

byte  Lamp::setRelayPin(byte relayPin) {
  itsRelayPin = relayPin;
}

byte  Lamp::setRelayStatus(byte relayStatus){
  switch(relayStatus) {
    case HIGH:
      itsRelayStatus = relayStatus;
      digitalWrite(getRelayPin(),itsRelayStatus);
      return 0;
    case LOW:
      itsRelayStatus = relayStatus;
      digitalWrite(getRelayPin(),itsRelayStatus);
      return 0;
    default:
      //do nothing. relayStatus must be HIGH or LOW!
      return 1;
  }
  return 1;
}

byte  Lamp::relaySwitch() {
  byte result;
  if (getRelayStatus() == HIGH) {
    result = setRelayStatus(LOW);
  } else {
    result = setRelayStatus(HIGH);
  }
  return result;
}
