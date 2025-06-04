#ifndef PIXIES_HPP
#define PIXIES_HPP

#include <Arduino.h>
#define Selectable FastLED_Selectable
#include <FastLED.h>
#include <Control_Surface.h>
#include "Constants.hpp"
#include "MCU.hpp" // Include the MCU header for track colors and other elements

//#define LED_PIN 5  
//#define NUM_LEDS 7

CRGB leds[NUM_LEDS];

void initPixies() {
    FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEDS);
    FastLED.setBrightness(5);
}

// ✅ Explicitly use FastLED namespace
CRGB getFastLEDColor(uint8_t colorByte) {
    switch (colorByte) {
        case 0x01: return CRGB::Red;
        case 0x02: return CRGB::Green;
        case 0x03: return CRGB::Yellow;
        case 0x04: return CRGB::Blue;
        case 0x05: return CRGB::Purple;
        case 0x06: return CRGB::Teal;
        default: return CRGB::White;
    }
}

void setAllLEDsColor(uint8_t colorByte) {
    CRGB trackColor = getFastLEDColor(colorByte);
    leds[0] = trackColor;
    leds[1] = trackColor;
    leds[2] = trackColor;
    leds[3] = trackColor;   
    leds[4] = trackColor;
    leds[5] = trackColor;
    leds[6] = trackColor; // Set all LEDs to the same color for simplicity
    FastLED.show();
}

void updateLEDColor(uint8_t trackIndex) {
    // Assuming trackIndex is 0-based and corresponds to the track number
    uint8_t colorByte = trackIndex; // Convert to 1-based color byte
    setAllLEDsColor(colorByte);
}
void setLEDColor(uint8_t trackIndex, uint8_t colorByte) {
    // Assuming trackIndex is 0-based and corresponds to the track number
    if (trackIndex < NUM_LEDS) {
        leds[trackIndex] = getFastLEDColor(colorByte);
        FastLED.show();
    }
}
void rainbow() {
    // Fill the LEDs with a rainbow effect
    for (int i = 0; i < NUM_LEDS; i++) {
        leds[i] = CHSV((i * 256 / NUM_LEDS), 255, 255);
    }
    FastLED.show();
}

uint8_t hue = 0;

void lightItUp(){
  for (int i = 0; i < NUM_LEDS; i++) {
    //leds[i] = CHSV(hue, 255, 255);
    leds[i] = CHSV(hue + (i * 10), 255, 255);
  }

  EVERY_N_MILLISECONDS(150){
    hue++;
  }

  FastLED.show();
}

void updateVPotRings(uint8_t bankIndex) {
    for (uint8_t ch = 0; ch < 2; ++ch) { // Only first two tracks
        uint8_t vpotPos = vpot[ch].getPosition(bankIndex);
        auto mode = vpot[ch].getMode();
        CRGB color = getFastLEDColor(trackColors[bankIndex][ch]);
        uint8_t ledStart = (ch == 0) ? LED_VU1_START : LED_VU2_START;
        uint8_t numLEDs = 12;
        // ...your ring logic...
    }
    FastLED.show();
}

void updateVUAndVPotRings(uint8_t bankIndex) {
    // --- 1. Draw VU meters ---
    // Channel 1
    uint8_t vu1Value = vu[0].getValue();
    uint8_t vu1Max = LED_VU1_END - LED_VU1_START;
    bool vu1Peaked = (vu1Value >= vu1Max);
    for (uint8_t i = 0; i < vu1Max; ++i) {
        if (i < vu1Value) {
            leds[LED_VU1_START + i] = vu1Peaked ? CRGB::Red : CRGB::Green;
        } else {
            leds[LED_VU1_START + i] = CRGB::Black;
        }
    }
    // Channel 2
    uint8_t vu2Value = vu[1].getValue();
    uint8_t vu2Max = LED_VU2_END - LED_VU2_START ;
    bool vu2Peaked = (vu2Value >= vu2Max);
    for (uint8_t i = 0; i < vu2Max; ++i) {
        if (i < vu2Value) {
            leds[LED_VU2_START + i] = vu2Peaked ? CRGB::Red : CRGB::Green;
        } else {
            leds[LED_VU2_START + i] = CRGB::Black;
        }
    }

    // --- 2. Overlay VPot rings ---
    for (uint8_t ch = 0; ch < 2; ++ch) {
        auto mode = vpot[ch].getMode();
        uint8_t vpotPos = vpot[ch].getPosition();
        CRGB color = getFastLEDColor(trackColors[bankIndex][ch]);
        uint8_t ledStart = (ch == 0) ? LED_VU1_START : LED_VU2_START;
        uint8_t numLEDs = 12;
        uint8_t center = 5;
        
        switch (mode) {
    case 0: // SingleDot
        // this is a single dot in the center
        Serial.println("SingleDot mode");
        if (vpotPos == center) {
            leds[ledStart + center] = color;
        } else {
            // Clear the center LED if not in the center
            leds[ledStart + center] = CRGB::Black;
            
        }
        break;
    case 1: // BoostCut
        // This is a boost/cut mode
        Serial.println("BoostCut mode");
        if (vpotPos == center) {
            leds[ledStart + center] = color; // Center LED
        } else if (vpotPos > center) {
            for (uint8_t i = center; i <= vpotPos && i < numLEDs; ++i)
                leds[ledStart + i] = color; // Boost
        } else {
            for (int8_t i = center; i >= (int8_t)vpotPos && i >= 0; --i)
                leds[ledStart + i] = color; // Cut
        }
        break;
    case 2: // Wrap
        Serial.println("Wrap mode");
        // This is a wrap mode
        for (uint8_t i = 0; i <= vpotPos && i < numLEDs; ++i) {
            leds[ledStart + i] = color; // Wrap around
        }
        // Clear the rest of the LEDs
        for (uint8_t i = vpotPos + 1; i < numLEDs; ++i) {
            leds[ledStart + i] = CRGB::Black; // Clear the rest
        }
        // Clear the center LED if not in the center
        if (vpotPos != center) {
            leds[ledStart + center] = CRGB::Black; // Clear the center LED
        }
        break;
    case 3: // Spread
        // This is a spread mode
        Serial.println("Spread mode");
        // Spread the color from the center to the left and right
        if (vpotPos == center) {
            leds[ledStart + center] = color; // Center LED
        } else if (vpotPos > center) {
            for (uint8_t i = center; i <= vpotPos && i < numLEDs; ++i)
                leds[ledStart + i] = color; // Spread to the right
        } else {
            for (int8_t i = center; i >= (int8_t)vpotPos && i >= 0; --i)
                leds[ledStart + i] = color; // Spread to the left
        }
        // Clear the rest of the LEDs
        for (uint8_t i = 0; i < numLEDs; ++i) {
            if (i < vpotPos || i > center) {
                leds[ledStart + i] = CRGB::Black; // Clear the rest
            }
        }
        break;
    default:
        
        Serial.println("Unknown mode");
     
        break;
}


        /*
        if (mode == MCU::VPotState::Spread) {
            if (vpotPos == center) {
                leds[ledStart + center] = color;
            } else if (vpotPos > center) {
                for (uint8_t i = center; i <= vpotPos && i < numLEDs; ++i)
                    leds[ledStart + i] = color;
            } else {
                for (int8_t i = center; i >= (int8_t)vpotPos && i >= 0; --i)
                    leds[ledStart + i] = color;
            }
        } else if (mode == MCU::VPotState::Wrap) {
            for (uint8_t i = 0; i <= vpotPos && i < numLEDs; ++i)
                leds[ledStart + i] = color;
        } else {
            if (vpotPos < numLEDs)
                leds[ledStart + vpotPos] = color;
        }*/
    }
    FastLED.show();
}
uint8_t ledCounter = 0;
void  updatePixies() {
  static uint32_t lastUpdateTime = 0;
    if (millis() - lastUpdateTime > 5000) {  // ✅ Print every 5 seconds
        //trackManager.printTrackNames();
        //debugPrint(trackManager.getTrackColor(1));
        //updateLEDColor(ledCounter);
        rainbow();
        lastUpdateTime = millis();
        if (ledCounter > 5){
          ledCounter = 0;
        }
        ledCounter++;
    }
}
void updateRudeSoloLED() {
    if (rudeSolo.getValue()) {
        leds[LED_RUDE_SOLO] = CRGB::Yellow; // or your chosen color
    } else {
        leds[LED_RUDE_SOLO] = CRGB::Black;
    }
}
void updateChannelStatusLEDs(uint8_t bankIndex) {
    for (uint8_t ch = 0; ch < 2; ++ch) {
        uint8_t ledIndex = (ch < 2) ? (LED_CHAN1_STRIP_START + ch) : (LED_CHAN2_STRIP_START + (ch - 2));
        leds[ledIndex] = getFastLEDColor(trackColors[bankIndex][ch]);
        if (ch >= NUM_BANKS) { // Ensure we don't go out of bounds
        // Mute
        leds[LED_CHAN1_STRIP_START + ch] = mute[ch].getValue() ? CRGB::Red : CRGB::Black;
        // Solo
        leds[LED_CHAN1_STRIP_START + ch + 1] = solo[ch].getValue() ? CRGB::Yellow : CRGB::Black;
        // Record Ready
        leds[LED_CHAN1_STRIP_START + ch + 2] = recrdy[ch].getValue() ? CRGB::Green : CRGB::Black;
        // (Adjust LED indices as needed for your layout)
        } else {
            // Mute
            leds[LED_CHAN2_STRIP_START + ch] = mute[ch].getValue() ? CRGB::Red : CRGB::Black;
            // Solo
            leds[LED_CHAN2_STRIP_START + ch + 1] = solo[ch].getValue() ? CRGB::Yellow : CRGB::Black;
            // Record Ready
            leds[LED_CHAN2_STRIP_START + ch + 2] = recrdy[ch].getValue() ? CRGB::Green : CRGB::Black;
        }
    }
}
#endif