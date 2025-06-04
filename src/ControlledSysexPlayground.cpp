// ControlledSysexPlayground.cpp
#include <Arduino.h>
#include "Screen.hpp"
#include "Pixies.hpp"
#include "ControlSurfaceSingleton.h"
#include "Wire.hpp"
#include "MCU.hpp"




void setup() {
  Serial.begin(115200);
  initPixies();
  delay(1000);
  Serial.println("Strarting control surface");
  
  initWire(); // Initialize I2C buses for displays
  initDisplays();
  
  
    // Initialize the Control Surface through the singleton
  ControlSurfaceSingleton::getInstance().begin();
  //Control_Surface.onSysEx(handleSysEx);
  //Serial.println("Hi");
  ControlSurfaceSingleton::getInstance().requestAllTrackColors();
  delay(1000);
  Serial.print("Free heap: ");
  Serial.println(ESP.getFreeHeap());
  //delay(1000);
  
  

}
uint8_t lastBankIndex = 0; 
void loop() {
  ControlSurfaceSingleton::getInstance().loop();
  if (ControlSurfaceSingleton::getInstance().getStatus() == false) {
    lightItUp(); // Only run this when not connected
  } else {
    uint8_t bankIndex = bank.getSelection();
    updateVUAndVPotRings(bankIndex); // Only run this when connected
    updateRudeSoloLED();
    updateChannelStatusLEDs(bankIndex);
    FastLED.show();
    // Remove or comment out any other LED update calls here!
  }
  updateDisplays();
}
