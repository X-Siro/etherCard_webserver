#include "etherShield.h"
#include "ETHER_28J60.h"
#include <classLamp.h>
#include <EEPROM.h>
#ifndef STR_BUFFER_SIZE
#define STR_BUFFER_SIZE 32
#endif


static uint8_t mac[6] = {
  0x54, 0x55, 0x58, 0x10, 0x00, 0x24
};

static uint8_t ip[4] = {
  192,168,1,175
};

static uint16_t port = 80;

ETHER_28J60 ethernet;

Lamp lampA(3,A2);
Lamp lampB(2,A3);

void setup() {
  // put your setup code here, to run once:
  ethernet.setup(mac,ip,port);
  //Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (String URL = ethernet.serviceRequest()) {
    char choice[STR_BUFFER_SIZE+1];
    URL.toCharArray(choice,256,0);
    ethernet.print("<link rel=\"shortcut icon\" type=\"image/x-icon\" href=\"http://arduino.cc/en/favicon.png\" />");
    ethernet.print("<head><meta charset=UTF-8></head>");

    if (strcmp(choice,"") == 0) {
      ethernet.print("<a href='lampa?switch' >Включить лампочку A</a><p></p>");
      ethernet.print("<a href='lampb?switch' >Включить лампочку B</a><p></p>");
    } else if (strcmp(choice,"lampb?switch") == 0) {
      lampB.lampSwitch();
      ethernet.print("Lamp B turned ");
      if (lampB.getLampStatus() == HIGH) ethernet.print("ON");
      else ethernet.print("OFF");
    } else if (strcmp(choice,"lampa?switch") == 0) {
      lampA.lampSwitch();
      ethernet.print("Lamp A turned ");
      if (lampB.getLampStatus() == HIGH) ethernet.print("ON");
      else ethernet.print("OFF");
    } else {
      ethernet.print("Error");
    }      
    ethernet.respond();
  }
  //Serial.println(choice);
  delay(1000);
}
