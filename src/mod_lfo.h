#pragma once
#include <Arduino.h>
#include "mod_config.h"
#include "mod_state.h"
#include "mod_hardware.h"

// =========================================================================
// mod_lfo.h — Generazione LFO sinusoidali + output su TLC5628
// =========================================================================

// Mappa rate (0..255) in incremento di fase per un dato range (milliHz).
static inline uint32_t rate_to_inc(uint8_t v, uint32_t minMHz, uint32_t maxMHz) {
    uint32_t f_mhz = minMHz + ((uint32_t)v * (maxMHz - minMHz)) / 255UL;
    // inc = 2^32 * f_mhz / (1000 * LFO_UPDATE_HZ)
    uint64_t num = (uint64_t)0x100000000ULL * (uint64_t)f_mhz;
    uint64_t den = 1000ULL * (uint64_t)LFO_UPDATE_HZ;
    return (uint32_t)(num / den);
}

// Ricalcola tutti gli incrementi di fase dai parametri correnti.
static inline void lfo_recalc_increments() {
    incSlow   = rate_to_inc(crsLfoSlowRate,
                            LFO_CRS_SLOW_MIN_MHZ, LFO_CRS_SLOW_MAX_MHZ);
    incFast   = rate_to_inc(crsLfoFastRate,
                            LFO_CRS_FAST_MIN_MHZ, LFO_CRS_FAST_MAX_MHZ);
    incPhaser = rate_to_inc(phaserRate,
                            LFO_PHASER_MIN_MHZ,  LFO_PHASER_MAX_MHZ);
}

// Fase (uint32) → campione 0..255 (sin, centro 128).
static inline uint8_t phase_to_sin8(uint32_t phase) {
    float ang = (float)phase * (2.0f * PI / 4294967296.0f);
    float s   = sinf(ang);                      // -1..+1
    return (uint8_t)((s + 1.0f) * 127.5f);      // 0..255
}

// Fase (uint32) → campione 0..255 con ampiezza scalata da depth (0..255).
// depth = 0 → uscita fissa 128; depth = 255 → 0..255 pieno.
static inline uint8_t phase_to_sin8_amp(uint32_t phase, uint8_t depth) {
    float ang = (float)phase * (2.0f * PI / 4294967296.0f);
    float s   = sinf(ang);
    float amp = (float)depth / 255.0f;
    return (uint8_t)((s * amp + 1.0f) * 127.5f);
}

// Tick periodico. Da chiamare nel loop(), si autolimita a LFO_UPDATE_HZ.
static inline void lfo_tick() {
    static uint32_t lastUs = 0;
    uint32_t now = micros();
    if ((uint32_t)(now - lastUs) < LFO_UPDATE_PERIOD_US) return;

    // Se siamo in forte ritardo (> 2 periodi), risincronizza
    if ((int32_t)(now - lastUs) > (int32_t)(LFO_UPDATE_PERIOD_US * 2)) {
        lastUs = now;
    } else {
        lastUs += LFO_UPDATE_PERIOD_US;
    }

    // Avanza le fasi (interrupt-safe? no, ma i singoli uint32_t sono atomici su Cortex-M0+)
    phaseSlow   += incSlow;
    phaseFast   += incFast;
    phasePhaser += incPhaser;

    // Output su TLC5628
    tlc5628_write(TLC_CH_CRS_SLOW, phase_to_sin8(phaseSlow));
    tlc5628_write(TLC_CH_CRS_FAST, phase_to_sin8(phaseFast));
    tlc5628_write(TLC_CH_PHASER,   phase_to_sin8_amp(phasePhaser, phaserDepth));
}