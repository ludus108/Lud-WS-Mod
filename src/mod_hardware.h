#pragma once
#include <Arduino.h>
#include <SPI.h>
#include "mod_config.h"

// =========================================================================
// mod_hardware.h — Driver TLC5628 e AD5206 via SPI
// =========================================================================
//
// TLC5628  (8-bit DAC, 8 canali):
//   frame 12 bit: [A2 A1 A0 RNG D7 D6 D5 D4 D3 D2 D1 D0]  MSB first
//   RNG = 0 (Vref = VDD*1, range 0..VDD), RNG = 1 (range 0..2*VDD)
//   LOAD basso durante il frame, poi si alza (latch interno)
//   LDAC basso per trasferire al DAC (latchato LOW in init = sempre attivo)
//
// AD5206  (6 pot digitali, 256 step):
//   frame 11 bit: [A2 A1 A0 D7 D6 D5 D4 D3 D2 D1 D0]  MSB first
//   CS basso durante il frame
//
// MOSI/SCK condivisi. Ogni chip ha il suo CS.
// =========================================================================

static inline void tlc5628_write(uint8_t ch, uint8_t value, bool rng = false) {
    // 12 bit totali: A2 A1 A0 RNG D7..D0
    uint16_t frame = ((uint16_t)(ch & 0x07) << 9)
                   | ((rng ? 1u : 0u) << 8)
                   | (uint16_t)value;

    SPI.beginTransaction(SPISettings(4000000, MSBFIRST, SPI_MODE1));
    digitalWrite(PIN_TLC_LOAD, LOW);
    SPI.transfer((uint8_t)(frame >> 4));       // primi 8 bit: A2 A1 A0 RNG D7 D6 D5 D4
    SPI.transfer((uint8_t)((frame & 0x0F) << 4)); // ultimi 4 bit: D3 D2 D1 D0 + 4 zeri
    digitalWrite(PIN_TLC_LOAD, HIGH);
    SPI.endTransaction();
}

static inline void ad5206_write(uint8_t ch, uint8_t value) {
    // 11 bit totali: A2 A1 A0 D7..D0
    uint16_t frame = ((uint16_t)(ch & 0x07) << 8) | (uint16_t)value;

    SPI.beginTransaction(SPISettings(4000000, MSBFIRST, SPI_MODE1));
    digitalWrite(PIN_AD_CS, LOW);
    SPI.transfer((uint8_t)(frame >> 3));         // A2 A1 A0 D7 D6 D5 D4 D3
    SPI.transfer((uint8_t)((frame & 0x07) << 5));// D2 D1 D0 + 5 zeri
    digitalWrite(PIN_AD_CS, HIGH);
    SPI.endTransaction();
}

static inline void hw_init() {
    // --- Pin ---
    pinMode(PIN_TLC_LOAD, OUTPUT);  digitalWrite(PIN_TLC_LOAD, HIGH);
    pinMode(PIN_TLC_LDAC, OUTPUT);  digitalWrite(PIN_TLC_LDAC, LOW);   // sempre latch
    pinMode(PIN_AD_CS,    OUTPUT);  digitalWrite(PIN_AD_CS,    HIGH);
    pinMode(PIN_CHORUS_MODE, OUTPUT); digitalWrite(PIN_CHORUS_MODE, LOW);

    // --- SPI (pin default RP2040: SCK=GP18, MOSI=GP19) ---
    SPI.begin();

    // --- Azzera/centra le uscite ---
    for (uint8_t ch = 0; ch < 6; ch++) ad5206_write(ch, 128);
    tlc5628_write(TLC_CH_CRS_SLOW,      128);
    tlc5628_write(TLC_CH_CRS_FAST,      128);
    tlc5628_write(TLC_CH_CRS_OFFSET,    128);
    tlc5628_write(TLC_CH_PHASER,        128);
    tlc5628_write(TLC_CH_CRS_SLOW_GAIN, 128);
    tlc5628_write(TLC_CH_CRS_FAST_GAIN, 128);

    // --- Fase LFO azzerata ---
    phaseSlow = phaseFast = phasePhaser = 0;
}