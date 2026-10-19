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

// --- SPI hardware (condiviso: 2x TLC5628 + 2x AD5206 + 2x TPL0502) ---
#define PIN_SPI_SCK      18
#define PIN_SPI_MOSI     19
#define PIN_TLC1_LOAD    20    // CS TLC5628 #1
#define PIN_TLC_LDAC     21    // latch condiviso TLC1+TLC2
#define PIN_AD1_CS       22    // CS AD5206 #1
#define PIN_TLC2_LOAD    10    // CS TLC5628 #2
#define PIN_AD2_CS       11    // CS AD5206 #2
#define PIN_TPL_A_CS     12    // CS TPL0502 (dlyA_time)
#define PIN_TPL_B_CS     13    // CS TPL0502 (dlyB_time)

// --- Uscite digitali ---
#define PIN_CHORUS_MODE   15   // HIGH = EQ ON
#define PIN_DLYA_MODE      2   // 0=pre, 1=post
#define PIN_DLYA_VCF_MODE  3   // 0=LP,  1=BP
#define PIN_DLYB_MODE      4
#define PIN_DLYB_VCF_MODE  5
#define PIN_REV_SEL        6   // 0=Rev1, 1=Rev2

// --- Tick LFO ---
#define LFO_UPDATE_HZ        1000UL
#define LFO_UPDATE_PERIOD_US (1000000UL / LFO_UPDATE_HZ)

// -------------------------------------------------------------------------
// RANGE FREQUENZE LFO (milliHz) — modificare qui per calibrare
// -------------------------------------------------------------------------
#define LFO_CRS_SLOW_MIN_MHZ    100UL      // 0.10 Hz
#define LFO_CRS_SLOW_MAX_MHZ    3000UL     // 3.00 Hz
#define LFO_CRS_FAST_MIN_MHZ    1000UL     // 1.00 Hz
#define LFO_CRS_FAST_MAX_MHZ    10000UL    // 10.0 Hz
#define LFO_PHASER_MIN_MHZ      50UL       // 0.05 Hz
#define LFO_PHASER_MAX_MHZ      5000UL     // 5.00 Hz
#define LFO_DLY_MIN_MHZ         50UL       // 0.05 Hz  (modulazione delay time)
#define LFO_DLY_MAX_MHZ         2000UL     // 2.00 Hz
#define LFO_VCF_DLY_MIN_MHZ     50UL       // 0.05 Hz  (modulazione VCF cut)
#define LFO_VCF_DLY_MAX_MHZ     5000UL     // 5.00 Hz

// --- Canali TLC5628 #1 ---
#define TLC1_CH_CRS_SLOW       0
#define TLC1_CH_CRS_FAST       1
#define TLC1_CH_CRS_OFFSET     2
#define TLC1_CH_PHASER         3
#define TLC1_CH_CRS_SLOW_GAIN  4
#define TLC1_CH_CRS_FAST_GAIN  5

// --- Canali TLC5628 #2 ---
#define TLC2_CH_DLYA_VCF_CUT   0
#define TLC2_CH_DLYB_VCF_CUT   1
#define TLC2_CH_REV_PARAM_1    2
#define TLC2_CH_REV_PARAM_2    3
#define TLC2_CH_REV_PARAM_3    4

// --- Canali AD5206 #1 (EQ) ---
#define AD1_CH_EQ1  0
#define AD1_CH_EQ2  1
#define AD1_CH_EQ3  2
#define AD1_CH_EQ4  3
#define AD1_CH_EQ5  4
#define AD1_CH_EQ6  5

// --- Canali AD5206 #2 (delay) ---
#define AD2_CH_DLYA_FB       0
#define AD2_CH_DLYA_VCF_RES  1
#define AD2_CH_DLYB_FB       2
#define AD2_CH_DLYB_VCF_RES  3
// --- Canali AD5206 #2 (delay + crs_level) ---
#define AD2_CH_DLYA_FB       0
#define AD2_CH_DLYA_VCF_RES  1
#define AD2_CH_DLYB_FB       2
#define AD2_CH_DLYB_VCF_RES  3
#define AD2_CH_CRS_LEVEL     4   // ← NUOVO
// -------------------------------------------------------------------------
// Chiavi LWS (CMD_PARAM: [target][key][value])
// -------------------------------------------------------------------------
// --- Chorus / Phaser (esistenti) ---
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
#define K_CRS_LEVEL          'L'   //  (crs_level 0..255)
#define K_PHASER_RATE        'p'
#define K_PHASER_DEPTH       'd'
#define K_PHASER_FB          'f'

// --- Delay A ---
#define K_DLYA_MODE          'a'
#define K_DLYA_TIME          'b'
#define K_DLYA_FB            'c'
#define K_DLYA_LFO           'e'
#define K_DLYA_LFO_RATE      'h'
#define K_DLYA_VCF_MODE      'i'
#define K_DLYA_VCF_CUT       'j'
#define K_DLYA_VCF_RES       'k'
#define K_DLYA_VCF_LFO       'l'
#define K_DLYA_VCF_LFO_RATE  'm'

// --- Delay B ---
#define K_DLYB_MODE          'n'
#define K_DLYB_TIME          'q'
#define K_DLYB_FB            'r'
#define K_DLYB_LFO           't'
#define K_DLYB_LFO_RATE      'u'
#define K_DLYB_VCF_MODE      'v'
#define K_DLYB_VCF_CUT       'w'
#define K_DLYB_VCF_RES       'x'
#define K_DLYB_VCF_LFO       'y'
#define K_DLYB_VCF_LFO_RATE  'z'

// --- Reverb ---
#define K_REV_SEL            'A'
#define K_REV_PARAM_1        'B'
#define K_REV_PARAM_2        'C'
#define K_REV_PARAM_3        'D'

#ifndef PI
  #define PI 3.14159265358979f
#endif
#define UNUSED(x) ((void)(x))