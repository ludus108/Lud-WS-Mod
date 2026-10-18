#pragma once
#include <Arduino.h>
#include "mod_config.h"
#include "mod_state.h"
#include "mod_hardware.h"
#include "mod_lfo.h"

// =========================================================================
// mod_lws.h — Layer LWS: callback, dispatch, dumpStatus
// =========================================================================
// Il parsing del frame è delegato a comunicazioni_mcu.h (incluso in main).
// Qui si definiscono:
//   - on_param  : ricezione CMD_PARAM (fire-and-forget)
//   - on_error  : ricezione CMD_ERROR
//   - dispatch  : handler per CMD non-param (per ora niente da fare)
//   - dumpStatus: dump su seriale
// =========================================================================

static void dumpStatus() {
    Serial.println();
    Serial.println("[DUMP] ========== MOD STATUS ==========");
    Serial.printf("[DUMP] mcu=%c uptime=%lu ms\n", MCU_ID, millis());
    Serial.printf("[DUMP] frames=%lu pongs=%lu params=%lu\n",
                  g_frameFromRouter, g_pongSent, g_paramApplied);

    Serial.println("[DUMP] --- PARAMS ---");
    Serial.printf("[DUMP] chorus_mode=%u\n", chorusMode);
    Serial.printf("[DUMP] crs_slow rate=%u gain=%u\n",
                  crsLfoSlowRate, crsLfoSlowGain);
    Serial.printf("[DUMP] crs_fast rate=%u gain=%u\n",
                  crsLfoFastRate, crsLfoFastGain);
    Serial.printf("[DUMP] crs_offset=%u\n", crsOffset);
    Serial.printf("[DUMP] eq=%u,%u,%u,%u,%u,%u\n",
                  crsEq[0], crsEq[1], crsEq[2],
                  crsEq[3], crsEq[4], crsEq[5]);
    Serial.printf("[DUMP] phaser rate=%u depth=%u fb=%u\n",
                  phaserRate, phaserDepth, phaserFb);

    Serial.println("[DUMP] --- LFO (milliHz calcolati) ---");
    Serial.printf("[DUMP] inc_slow=%lu inc_fast=%lu inc_phaser=%lu\n",
                  (unsigned long)incSlow,
                  (unsigned long)incFast,
                  (unsigned long)incPhaser);

    Serial.println("[DUMP] ====================================");
    Serial.println();
}

static inline void on_error(const char *msg) {
    Serial.printf("[%c] ERROR: %s\n", MCU_ID, msg);
}

static inline void on_param(char target, char key, uint8_t value) {
    if (target != MCU_ID) return;
    g_paramApplied++;

    switch (key) {
        // ---------- Chorus mode (digitale) ----------
        case K_CHORUS_MODE:
            chorusMode = value ? 1 : 0;
            digitalWrite(PIN_CHORUS_MODE, chorusMode ? HIGH : LOW);
            break;

        // ---------- LFO slow ----------
        case K_CRS_LFO_SLOW_RATE:
            crsLfoSlowRate = value;
            lfo_recalc_increments();
            break;
        case K_CRS_LFO_SLOW_GAIN:
            crsLfoSlowGain = value;
            tlc5628_write(TLC_CH_CRS_SLOW_GAIN, crsLfoSlowGain);
            break;

        // ---------- LFO fast ----------
        case K_CRS_LFO_FAST_RATE:
            crsLfoFastRate = value;
            lfo_recalc_increments();
            break;
        case K_CRS_LFO_FAST_GAIN:
            crsLfoFastGain = value;
            tlc5628_write(TLC_CH_CRS_FAST_GAIN, crsLfoFastGain);
            break;

        // ---------- Offset DC ----------
        case K_CRS_OFFSET:
            crsOffset = value;
            tlc5628_write(TLC_CH_CRS_OFFSET, crsOffset);
            break;

        // ---------- EQ (AD5206) ----------
        case K_CRS_EQ_1: crsEq[0] = value; ad5206_write(AD_CH_EQ1, value); break;
        case K_CRS_EQ_2: crsEq[1] = value; ad5206_write(AD_CH_EQ2, value); break;
        case K_CRS_EQ_3: crsEq[2] = value; ad5206_write(AD_CH_EQ3, value); break;
        case K_CRS_EQ_4: crsEq[3] = value; ad5206_write(AD_CH_EQ4, value); break;
        case K_CRS_EQ_5: crsEq[4] = value; ad5206_write(AD_CH_EQ5, value); break;
        case K_CRS_EQ_6: crsEq[5] = value; ad5206_write(AD_CH_EQ6, value); break;

        // ---------- Phaser ----------
        case K_PHASER_RATE:
            phaserRate = value;
            lfo_recalc_increments();
            break;
        case K_PHASER_DEPTH:
            phaserDepth = value;
            // applicato al volo nel tick (phase_to_sin8_amp)
            break;
        case K_PHASER_FB:
            phaserFb = value;
            // TODO: inviare al digi pot quando definito
            break;

        default:
            Serial.printf("[%c] param sconosciuto: %c=%u\n", MCU_ID, key, value);
            break;
    }
}