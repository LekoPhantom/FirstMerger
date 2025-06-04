#pragma once
#include <Arduino.h>
constexpr uint8_t LED_PIN = 5;
constexpr uint8_t NUM_LEDS = 50;
constexpr uint8_t incrementPin = 18;
constexpr uint8_t decrementPin = 23;
constexpr uint8_t TRACKS_PER_BANK = 4; // Number of tracks per bank
constexpr uint8_t NUM_BANKS = 2; // Number of banks
/*
LED Array
0      - rude solo
1      - rude rready <-- is this a thing?
2-14   - vu meter/vpot ring chan1
15-27  - vu meter/vpotring chan2
28-32  - chan1 channel strip
33-36  - chan2 channel strip



Updated LED Array...
0-11 - jog ring
12 play
13 record
14 loop
15 rude solo
16 rude ready
17-21 chanStrip 1 
22-26 chanStrip 2
28-39 vu meter/vpot ring chan1
40-51 vu meter/vpot ring chan2
*/


constexpr uint8_t LED_JOG_RING_START = 0;
constexpr uint8_t LED_SCRUB_RING_END = 11;
constexpr uint8_t LED_PLAY = 12;
constexpr uint8_t LED_RECORD = 13;
constexpr uint8_t LED_RUDE_SOLO = 15;
constexpr uint8_t LED_RUDE_READY = 16;

constexpr uint8_t LED_LOOP = 16;
constexpr uint8_t LED_CHAN1_STRIP_START = 17;
constexpr uint8_t LED_CHAN1_STRIP_END = 21;
constexpr uint8_t LED_CHAN2_STRIP_START = 22;
constexpr uint8_t LED_CHAN2_STRIP_END = 26;
constexpr uint8_t LED_VU1_START = 28;
constexpr uint8_t LED_VU1_END = 39;         
constexpr uint8_t LED_VU2_START = 40;
constexpr uint8_t LED_VU2_END = 51;
constexpr uint8_t LED_VU1_MID = (LED_VU1_START + LED_VU1_END) / 2;
constexpr uint8_t LED_VU2_MID = (LED_VU2_START + LED_VU2_END) / 2;  

