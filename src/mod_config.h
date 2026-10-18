#pragma once
#include <Arduino.h>

// =========================================================================
// mod_config.h — Pin, K_*, range LFO per Lud-WS-Mod (RP2040)
// =========================================================================

#ifndef MCU_ID
  #define MCU_ID 'M'
#endif

// --- LWS ---
#define LWS_SERIAL    Serial1
#define LWS_BAUD_UP   115200UL
#define LWS_DEBUG     Serial

// --- SPI hardware (condiviso TLC5628 + AD5206) ---
#define PIN_SPI_SCK     18
#define PIN_SPI_MOSI    19
#define PIN_TLC_LOAD    20    // CS TLC5628 (attivo basso)
#define PIN_TLC_LDAC    21    // latch DAC (LOW = aggiorna uscite)
#define PIN_AD_CS       22    // CS AD5206 (attivo basso)

// --- Uscita digitale chorus_mode (HIGH = EQ ON) ---
#define PIN_CHORUS_MODE 15

// --- Tick LFO ---
#define LFO_UPDATE_HZ        1000UL
#define LFO_UPDATE_PERIOD_US (1000000UL / LFO_UPDATE_HZ)

// -------------------------------------------------------------------------
// RANGE FREQUENZE LFO (milliHz) — modificare qui per calibrare
// -------------------------------------------------------------------------
#define LFO_CRS_SLOW_MIN_MHZ   100UL      // 0.10 Hz
#define LFO_CRS_SLOW_MAX_MHZ   3000UL     // 3.00 Hz
#define LFO_CRS_FAST_MIN_MHZ   1000UL     // 1.00 Hz
#define LFO_CRS_FAST_MAX_MHZ   10000UL    // 10.0 Hz
#define LFO_PHASER_MIN_MHZ     50UL       // 0.05 Hz
#define LFO_PHASER_MAX_MHZ     5000UL     // 5.00 Hz

// --- Canali TLC5628 ---
#define TLC_CH_CRS_SLOW       0
#define TLC_CH_CRS_FAST       1
#define TLC_CH_CRS_OFFSET     2
#define TLC_CH_PHASER         3
#define TLC_CH_CRS_SLOW_GAIN  4
#define TLC_CH_CRS_FAST_GAIN  5

// --- Canali AD5206 (EQ) ---
#define AD_CH_EQ1  0
#define AD_CH_EQ2  1
#define AD_CH_EQ3  2
#define AD_CH_EQ4  3
#define AD_CH_EQ5  4
#define AD_CH_EQ6  5

// -------------------------------------------------------------------------
// Chiavi LWS (CMD_PARAM: [target][key][value])
// -------------------------------------------------------------------------
#define K_CHORUS_MODE        'M'
#define K_CRS_LFO_SLOW_RATE  's'
#define K_CRS_LFO_SLOW_GAIN  'g'
#define K_CRS_LFO_FAST_RATE  'S'
#define K_CRS_LFO_FAST_GAIN  'G'
#define K_CRS_OFFSET         'o'
#define K_CRS_EQ_1           '1'
#define K_CRS_EQ_2           '2'
#define K_CRS_EQ_3           '3'
#define K_CRS_EQ_4           '4'
#define K_CRS_EQ_5           '5'
#define K_CRS_EQ_6           '6'
#define K_PHASER_RATE        'p'
#define K_PHASER_DEPTH       'd'
#define K_PHASER_FB          'f'

#ifndef PI
  #define PI 3.14159265358979f
#endif

#define UNUSED(x) ((void)(x))