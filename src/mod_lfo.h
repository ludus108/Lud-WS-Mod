#pragma once
#include <Arduino.h>
#include "mod_config.h"
#include "mod_state.h"
#include "mod_hardware.h"

// =========================================================================
// mod_lfo.h — 7 LFO sinusoidali + modulazione
// =========================================================================

// Rate 0..255 → incremento di fase (Hz espresso in milliHz).
static inline uint32_t rate_to_inc(uint8_t v, uint32_t minMHz, uint32_t maxMHz) {
    uint32_t f_mhz = minMHz + ((uint32_t)v * (maxMHz - minMHz)) / 255UL;
    uint64_t num = (uint64_t)0x100000000ULL * (uint64_t)f_mhz;
    uint64_t den = 1000ULL * (uint64_t)LFO_UPDATE_HZ;
    return (uint32_t)(num / den);
}

// Ricalcola tutti gli incrementi di fase dai parametri correnti.
static inline void lfo_recalc_increments() {
    incSlow     = rate_to_inc(crsLfoSlowRate,    LFO_CRS_SLOW_MIN_MHZ, LFO_CRS_SLOW_MAX_MHZ);
    incFast     = rate_to_inc(crsLfoFastRate,    LFO_CRS_FAST_MIN_MHZ, LFO_CRS_FAST_MAX_MHZ);
    incPhaser   = rate_to_inc(phaserRate,        LFO_PHASER_MIN_MHZ,   LFO_PHASER_MAX_MHZ);
    incDlyA     = rate_to_inc(dlyALfoRate,       LFO_DLY_MIN_MHZ,      LFO_DLY_MAX_MHZ);
    incVcfDlyA  = rate_to_inc(dlyAVcfLfoRate,    LFO_VCF_DLY_MIN_MHZ,  LFO_VCF_DLY_MAX_MHZ);
    incDlyB     = rate_to_inc(dlyBLfoRate,       LFO_DLY_MIN_MHZ,      LFO_DLY_MAX_MHZ);
    incVcfDlyB  = rate_to_inc(dlyBVcfLfoRate,    LFO_VCF_DLY_MIN_MHZ,  LFO_VCF_DLY_MAX_MHZ);
}

// Fase (uint32) → campione 0..255 (sin, centro 128).
static inline uint8_t phase_to_sin8(uint32_t phase) {
    float s = sinf((float)phase * (2.0f * PI / 4294967296.0f));
    return (uint8_t)((s + 1.0f) * 127.5f);
}

// Fase → campione con ampiezza scalata da depth (peak-to-peak = depth).
// depth=0 → 128 fisso; depth=255 → 0..255 pieno (centro 128).
static inline uint8_t phase_to_sin8_amp(uint32_t phase, uint8_t depth) {
    float s = sinf((float)phase * (2.0f * PI / 4294967296.0f));
    float amp = (float)depth / 255.0f;
    return (uint8_t)((s * amp + 1.0f) * 127.5f);
}

// base + swing sinusoidale (peak-to-peak = depth), clamped 0..255.
// depth=0 → base immutato.
static inline uint8_t mod_u8(uint8_t base, uint32_t phase, uint8_t depth) {
    if (depth == 0) return base;
    float s = sinf((float)phase * (2.0f * PI / 4294967296.0f));
    int v = (int)base + (int)(s * (float)depth * 0.5f);
    if (v < 0) v = 0;
    if (v > 255) v = 255;
    return (uint8_t)v;
}

// -------------------------------------------------------------------------
// Tick LFO: da chiamare nel loop(), autolimita a LFO_UPDATE_HZ.
// -------------------------------------------------------------------------
static inline void lfo_tick() {
    static uint32_t lastUs = 0;
    uint32_t now = micros();
    if ((uint32_t)(now - lastUs) < LFO_UPDATE_PERIOD_US) return;

    if ((int32_t)(now - lastUs) > (int32_t)(LFO_UPDATE_PERIOD_US * 2)) {
        lastUs = now;
    } else {
        lastUs += LFO_UPDATE_PERIOD_US;
    }

    // Avanza fasi
    phaseSlow     += incSlow;
    phaseFast     += incFast;
    phasePhaser   += incPhaser;
    phaseDlyA     += incDlyA;
    phaseVcfDlyA  += incVcfDlyA;
    phaseDlyB     += incDlyB;
    phaseVcfDlyB  += incVcfDlyB;

    // --- Uscite dirette su TLC1 ---
    tlc1_write(TLC1_CH_CRS_SLOW, phase_to_sin8(phaseSlow));
    tlc1_write(TLC1_CH_CRS_FAST, phase_to_sin8(phaseFast));
    tlc1_write(TLC1_CH_PHASER,   phase_to_sin8_amp(phasePhaser, phaserDepth));

    // --- Uscite modulate su TLC2 (VCF cut A/B) ---
    tlc2_write(TLC2_CH_DLYA_VCF_CUT, mod_u8(dlyAVcfCut, phaseVcfDlyA, dlyAVcfLfo));
    tlc2_write(TLC2_CH_DLYB_VCF_CUT, mod_u8(dlyBVcfCut, phaseVcfDlyB, dlyBVcfLfo));

    // --- Uscite modulate su TPL0502 (delay time A/B) ---
    tpl_a_write(mod_u8(dlyATime, phaseDlyA, dlyALfo));
    tpl_b_write(mod_u8(dlyBTime, phaseDlyB, dlyBLfo));
}