#pragma once
#include <Arduino.h>
#include <SPI.h>
#include "mod_config.h"

// =========================================================================
// mod_hardware.h — Driver SPI: TLC5628, AD5206, TPL0502
// =========================================================================
//
// TLC5628 (8-bit DAC, 8 ch, 11-bit SPI):
//   frame MSB-first: [A2 A1 A0 D7 D6 D5 D4 D3 D2 D1 D0] (11 bit)
//   LOAD basso durante frame → LOAD alto latcha nel registro input
//   LDAC basso (tied low) → trasferisce al DAC (transparent mode)
//
// AD5206 (6 pot, 256 step, 11-bit SPI):
//   frame identico a TLC5628, solo significato diverso
//
// TPL0502 (8-bit single-channel digi-pot, SPI):
//   protocollo ASSUNTO: 16-bit [D7..D0][dummy=0x00], verificare datasheet
// =========================================================================

static const SPISettings LWS_SPI_CFG(4000000, MSBFIRST, SPI_MODE0);

static inline void spi_write_11bit(uint8_t ch, uint8_t value) {
    uint16_t frame = ((uint16_t)(ch & 0x07) << 8) | (uint16_t)value;
    SPI.transfer((uint8_t)(frame >> 3));           // bits 10..3
    SPI.transfer((uint8_t)((frame & 0x07) << 5));  // bits 2..0 + 5 zeri
}

static inline void tlc1_write(uint8_t ch, uint8_t value) {
    SPI.beginTransaction(LWS_SPI_CFG);
    digitalWrite(PIN_TLC1_LOAD, LOW);
    spi_write_11bit(ch, value);
    digitalWrite(PIN_TLC1_LOAD, HIGH);
    SPI.endTransaction();
}

static inline void tlc2_write(uint8_t ch, uint8_t value) {
    SPI.beginTransaction(LWS_SPI_CFG);
    digitalWrite(PIN_TLC2_LOAD, LOW);
    spi_write_11bit(ch, value);
    digitalWrite(PIN_TLC2_LOAD, HIGH);
    SPI.endTransaction();
}

static inline void ad1_write(uint8_t ch, uint8_t value) {
    SPI.beginTransaction(LWS_SPI_CFG);
    digitalWrite(PIN_AD1_CS, LOW);
    spi_write_11bit(ch, value);
    digitalWrite(PIN_AD1_CS, HIGH);
    SPI.endTransaction();
}

static inline void ad2_write(uint8_t ch, uint8_t value) {
    SPI.beginTransaction(LWS_SPI_CFG);
    digitalWrite(PIN_AD2_CS, LOW);
    spi_write_11bit(ch, value);
    digitalWrite(PIN_AD2_CS, HIGH);
    SPI.endTransaction();
}

// TPL0502 — 8-bit, 16-bit SPI frame (data + dummy)
// NB: se il chip usa un protocollo diverso (es. 8 bit), adattare qui.
static inline void tpl_a_write(uint8_t value) {
    SPI.beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE0));
    digitalWrite(PIN_TPL_A_CS, LOW);
    SPI.transfer(value);
    SPI.transfer(0x00);
    digitalWrite(PIN_TPL_A_CS, HIGH);
    SPI.endTransaction();
}

static inline void tpl_b_write(uint8_t value) {
    SPI.beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE0));
    digitalWrite(PIN_TPL_B_CS, LOW);
    SPI.transfer(value);
    SPI.transfer(0x00);
    digitalWrite(PIN_TPL_B_CS, HIGH);
    SPI.endTransaction();
}

static inline void hw_init() {
    // --- Chip selects + LDAC ---
    pinMode(PIN_TLC1_LOAD, OUTPUT); digitalWrite(PIN_TLC1_LOAD, HIGH);
    pinMode(PIN_TLC2_LOAD, OUTPUT); digitalWrite(PIN_TLC2_LOAD, HIGH);
    pinMode(PIN_TLC_LDAC,  OUTPUT); digitalWrite(PIN_TLC_LDAC,  LOW);   // transparent
    pinMode(PIN_AD1_CS,    OUTPUT); digitalWrite(PIN_AD1_CS,    HIGH);
    pinMode(PIN_AD2_CS,    OUTPUT); digitalWrite(PIN_AD2_CS,    HIGH);
    pinMode(PIN_TPL_A_CS,  OUTPUT); digitalWrite(PIN_TPL_A_CS,  HIGH);
    pinMode(PIN_TPL_B_CS,  OUTPUT); digitalWrite(PIN_TPL_B_CS,  HIGH);

    // --- Uscite digitali ---
    pinMode(PIN_CHORUS_MODE,   OUTPUT); digitalWrite(PIN_CHORUS_MODE,   LOW);
    pinMode(PIN_DLYA_MODE,     OUTPUT); digitalWrite(PIN_DLYA_MODE,     LOW);
    pinMode(PIN_DLYA_VCF_MODE, OUTPUT); digitalWrite(PIN_DLYA_VCF_MODE, LOW);
    pinMode(PIN_DLYB_MODE,     OUTPUT); digitalWrite(PIN_DLYB_MODE,     LOW);
    pinMode(PIN_DLYB_VCF_MODE, OUTPUT); digitalWrite(PIN_DLYB_VCF_MODE, LOW);
    pinMode(PIN_REV_SEL,       OUTPUT); digitalWrite(PIN_REV_SEL,       LOW);

    // --- SPI ---
    SPI.begin();

    // --- Inizializza uscite a valori di default ---
    for (uint8_t ch = 0; ch < 6; ch++) ad1_write(ch, 128);
    for (uint8_t ch = 0; ch < 4; ch++) ad2_write(ch, 128);
    ad2_write(AD2_CH_CRS_LEVEL, 128);
	
    tlc1_write(TLC1_CH_CRS_SLOW,      128);
    tlc1_write(TLC1_CH_CRS_FAST,      128);
    tlc1_write(TLC1_CH_CRS_OFFSET,    128);
    tlc1_write(TLC1_CH_PHASER,        128);
    tlc1_write(TLC1_CH_CRS_SLOW_GAIN, 200);
    tlc1_write(TLC1_CH_CRS_FAST_GAIN, 200);

    tlc2_write(TLC2_CH_DLYA_VCF_CUT,  128);
    tlc2_write(TLC2_CH_DLYB_VCF_CUT,  128);
    tlc2_write(TLC2_CH_REV_PARAM_1,   128);
    tlc2_write(TLC2_CH_REV_PARAM_2,   128);
    tlc2_write(TLC2_CH_REV_PARAM_3,   128);

    tpl_a_write(128);
    tpl_b_write(128);
}