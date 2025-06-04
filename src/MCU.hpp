#ifndef MCU_HPP
#define MCU_HPP

#include <Control_Surface.h>
#include "ControlSurfaceSingleton.h"
#include "Constants.hpp"



// -------------------------- MIDI Input Elements --------------------------- //
// ========================================================================== //

/*
 * Define all elements that listen for MIDI messages.
 */

Bank<NUM_BANKS> bank(TRACKS_PER_BANK); // Create a new bank with four tracks per bank

// Create a new bank selector to control the bank using a push button
// Instantiate a Bank selector to control which one of the four Banks is active.
IncrementDecrementSelector<NUM_BANKS> selector = {
    bank,       // Bank to manage
    {incrementPin, decrementPin},     // push button pins (increment, decrement)
    Wrap::Wrap, // Wrap around
};



// Main MCU LCD screen, used to get track names
MCU::LCD<> lcd {};

// Time display_L keeps track of the bar counter
MCU::TimeDisplay timedisplay {};

// Play / Record
NoteValue play {MCU::PLAY};
NoteValue record {MCU::RECORD};
NoteValue rudeSolo {MCU::RUDE_SOLO};

Bankable::NoteValue<NUM_BANKS> mute[] {
  {bank, MCU::MUTE_1},
  {bank, MCU::MUTE_2},
  {bank, MCU::MUTE_3},
  {bank, MCU::MUTE_4},
};

// Solo
Bankable::NoteValue<NUM_BANKS> solo[] {
  {bank, MCU::SOLO_1},
  {bank, MCU::SOLO_2},
  {bank, MCU::SOLO_3},
  {bank, MCU::SOLO_4},
};



// Record arm / ready
Bankable::NoteValue<NUM_BANKS> recrdy[] {
  {bank, MCU::REC_RDY_1},
  {bank, MCU::REC_RDY_2},
  {bank, MCU::REC_RDY_3},
  {bank, MCU::REC_RDY_4},
};

// VU meters
MCU::Bankable::VU<NUM_BANKS> vu[] {
  {bank, 1, MCU::VUDecay::Hold},
  {bank, 2, MCU::VUDecay::Hold},
  {bank, 3, MCU::VUDecay::Hold},
  {bank, 4, MCU::VUDecay::Hold},
};

// VPot rings
MCU::Bankable::VPotRing<NUM_BANKS> vpot[] {
  {bank, 1},
  {bank, 2},
  {bank, 3},
  {bank, 4},
};

extern uint8_t trackColors[NUM_BANKS][TRACKS_PER_BANK];

uint8_t trackColors[NUM_BANKS][TRACKS_PER_BANK] = {0};
#endif
