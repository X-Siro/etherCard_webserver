#include <TroykaCurrent.h>
#define sensorDelta 0.05

class Lamp {
  public:
    Lamp(byte relayPin, uint8_t lineSensorPin);
  
    const byte  getRelayPin() const { return itsRelayPin; };
    const byte  getRelayStatus() const { return itsRelayStatus; };
    const byte  getLampStatus() const;
    const float getLampAmperage() const;
  
    byte  setRelayPin(byte relayPin);
    byte  setLampStatus(byte lampStatus);
    byte  setRelayStatus(byte relayStatus);
    
    byte  lampSwitch();
    byte  relaySwitch();
  private:
    byte  itsRelayPin;
    byte  itsRelayStatus;
    ACS712 itsLineSensor;
};
