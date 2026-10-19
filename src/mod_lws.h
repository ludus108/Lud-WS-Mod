#pragma once
#include <Arduino.h>
#include "mod_config.h"
#include "mod_state.h"
#include "mod_hardware.h"
#include "mod_lfo.h"

// =========================================================================
// mod_lws.h — Callback LWS + dumpStatus
// =========================================================================

static void dumpStatus() {
    Serial.println();
    Serial.println("[DUMP] ========== MOD STATUS ==========");
    Serial.printf("[DUMP] mcu=%c uptime=%lu ms\n", MCU_ID, millis());
    Serial.printf("[DUMP] frames=%lu pongs=%lu params=%lu\n",
                  g_frameFromRouter, g_pongSent, g_paramApplied);

    Serial.println("[DUMP] --- CHORUS / PHASER ---");
    Serial.printf("[DUMP] chorus_mode=%u  crs_offset=%u\n", chorusMode, crsOffset);
	Serial.printf("[DUMP] crs_level=%u\n", crsLevel);
    Serial.printf("[DUMP] crs_slow rate=%u gain=%u\n", crsLfoSlowRate, crsLfoSlowGain);
    Serial.printf("[DUMP] crs_fast rate=%u gain=%u\n", crsLfoFastRate, crsLfoFastGain);
    Serial.printf("[DUMP] eq=%u,%u,%u,%u,%u,%u\n",
                  crsEq[0], crsEq[1], crsEq[2], crsEq[3], crsEq[4], crsEq[5]);
    Serial.printf("[DUMP] phaser rate=%u depth=%u fb=%u\n",
                  phaserRate, phaserDepth, phaserFb);

    Serial.println("[DUMP] --- DELAY A ---");
    Serial.printf("[DUMP] mode=%u time=%u(base) fb=%u\n", dlyAMode, dlyATime, dlyAFb);
    Serial.printf("[DUMP] lfo depth=%u rate=%u\n", dlyALfo, dlyALfoRate);
    Serial.printf("[DUMP] vcf mode=%u cut=%u(base) res=%u\n",
                  dlyAVcfMode, dlyAVcfCut, dlyAVcfRes);
    Serial.printf("[DUMP] vcf_lfo depth=%u rate=%u\n", dlyAVcfLfo, dlyAVcfLfoRate);

    Serial.println("[DUMP] --- DELAY B ---");
    Serial.printf("[DUMP] mode=%u time=%u(base) fb=%u\n", dlyBMode, dlyBTime, dlyBFb);
    Serial.printf("[DUMP] lfo depth=%u rate=%u\n", dlyBLfo, dlyBLfoRate);
    Serial.printf("[DUMP] vcf mode=%u cut=%u(base) res=%u\n",
                  dlyBVcfMode, dlyBVcfCut, dlyBVcfRes);
    Serial.printf("[DUMP] vcf_lfo depth=%u rate=%u\n", dlyBVcfLfo, dlyBVcfLfoRate);

    Serial.println("[DUMP] --- REVERB ---");
    Serial.printf("[DUMP] sel=%u param1=%u param2=%u param3=%u\n",
                  revSel, revParam1, revParam2, revParam3);

    Serial.println("[DUMP] --- LFO EFFECTIVE (base + mod) ---");
    Serial.printf("[DUMP] dlyA_time base=%u eff=%u\n",
                  dlyATime, mod_u8(dlyATime, phaseDlyA, dlyALfo));
    Serial.printf("[DUMP] dlyA_cut  base=%u eff=%u\n",
                  dlyAVcfCut, mod_u8(dlyAVcfCut, phaseVcfDlyA, dlyAVcfLfo));
    Serial.printf("[DUMP] dlyB_time base=%u eff=%u\n",
                  dlyBTime, mod_u8(dlyBTime, phaseDlyB, dlyBLfo));
    Serial.printf("[DUMP] dlyB_cut  base=%u eff=%u\n",
                  dlyBVcfCut, mod_u8(dlyBVcfCut, phaseVcfDlyB, dlyBVcfLfo));

    Serial.println("[DUMP] ====================================");
    Serial.println();
}

static inline void on_error(const char *msg) {
    Serial.printf("[%c] ERROR: %s\n", MCU_ID, msg);
}

static inline void on_param(char target, char key, uint8_t value) {
    if (target != MCU_ID) return;
    g_paramApplied++;

    if (key == '?') { dumpStatus(); return; }

    switch (key) {
        // ================= CHORUS / PHASER =================
        case K_CHORUS_MODE:
            chorusMode = value ? 1 : 0;
            digitalWrite(PIN_CHORUS_MODE, chorusMode ? HIGH : LOW);
            break;
        case K_CRS_LFO_SLOW_RATE: crsLfoSlowRate = value; lfo_recalc_increments(); break;
        case K_CRS_LFO_SLOW_GAIN: crsLfoSlowGain = value; tlc1_write(TLC1_CH_CRS_SLOW_GAIN, value); break;
        case K_CRS_LFO_FAST_RATE: crsLfoFastRate = value; lfo_recalc_increments(); break;
        case K_CRS_LFO_FAST_GAIN: crsLfoFastGain = value; tlc1_write(TLC1_CH_CRS_FAST_GAIN, value); break;
        case K_CRS_OFFSET:        crsOffset    = value; tlc1_write(TLC1_CH_CRS_OFFSET, value); break;
        case K_CRS_EQ_1: crsEq[0]=value; ad1_write(AD1_CH_EQ1,value); break;
        case K_CRS_EQ_2: crsEq[1]=value; ad1_write(AD1_CH_EQ2,value); break;
        case K_CRS_EQ_3: crsEq[2]=value; ad1_write(AD1_CH_EQ3,value); break;
        case K_CRS_EQ_4: crsEq[3]=value; ad1_write(AD1_CH_EQ4,value); break;
        case K_CRS_EQ_5: crsEq[4]=value; ad1_write(AD1_CH_EQ5,value); break;
        case K_CRS_EQ_6: crsEq[5]=value; ad1_write(AD1_CH_EQ6,value); break;
        case K_PHASER_RATE:  phaserRate  = value; lfo_recalc_increments(); break;
        case K_PHASER_DEPTH: phaserDepth = value; break;   // applicato al volo nel tick
        case K_PHASER_FB:    phaserFb    = value; break;   // TODO digi-pot
		case K_CRS_LEVEL:    crsLevel   = value; ad2_write(AD2_CH_CRS_LEVEL, value); break;

        // ================= DELAY A =================
        case K_DLYA_MODE:
            dlyAMode = value ? 1 : 0;
            digitalWrite(PIN_DLYA_MODE, dlyAMode ? HIGH : LOW);
            break;
        case K_DLYA_TIME:   dlyATime = value; break;   // TPL gestito nel tick
        case K_DLYA_FB:     dlyAFb   = value; ad2_write(AD2_CH_DLYA_FB, value); break;
        case K_DLYA_LFO:    dlyALfo  = value; break;
        case K_DLYA_LFO_RATE: dlyALfoRate = value; lfo_recalc_increments(); break;
        case K_DLYA_VCF_MODE:
            dlyAVcfMode = value ? 1 : 0;
            digitalWrite(PIN_DLYA_VCF_MODE, dlyAVcfMode ? HIGH : LOW);
            break;
        case K_DLYA_VCF_CUT: dlyAVcfCut = value; break;   // TLC2 nel tick
        case K_DLYA_VCF_RES: dlyAVcfRes = value; ad2_write(AD2_CH_DLYA_VCF_RES, value); break;
        case K_DLYA_VCF_LFO: dlyAVcfLfo = value; break;
        case K_DLYA_VCF_LFO_RATE: dlyAVcfLfoRate = value; lfo_recalc_increments(); break;

        // ================= DELAY B =================
        case K_DLYB_MODE:
            dlyBMode = value ? 1 : 0;
            digitalWrite(PIN_DLYB_MODE, dlyBMode ? HIGH : LOW);
            break;
        case K_DLYB_TIME:   dlyBTime = value; break;
        case K_DLYB_FB:     dlyBFb   = value; ad2_write(AD2_CH_DLYB_FB, value); break;
        case K_DLYB_LFO:    dlyBLfo  = value; break;
        case K_DLYB_LFO_RATE: dlyBLfoRate = value; lfo_recalc_increments(); break;
        case K_DLYB_VCF_MODE:
            dlyBVcfMode = value ? 1 : 0;
            digitalWrite(PIN_DLYB_VCF_MODE, dlyBVcfMode ? HIGH : LOW);
            break;
        case K_DLYB_VCF_CUT: dlyBVcfCut = value; break;
        case K_DLYB_VCF_RES: dlyBVcfRes = value; ad2_write(AD2_CH_DLYB_VCF_RES, value); break;
        case K_DLYB_VCF_LFO: dlyBVcfLfo = value; break;
        case K_DLYB_VCF_LFO_RATE: dlyBVcfLfoRate = value; lfo_recalc_increments(); break;

        // ================= REVERB =================
        case K_REV_SEL:
            revSel = value ? 1 : 0;
            digitalWrite(PIN_REV_SEL, revSel ? HIGH : LOW);
            break;
        case K_REV_PARAM_1: revParam1 = value; tlc2_write(TLC2_CH_REV_PARAM_1, value); break;
        case K_REV_PARAM_2: revParam2 = value; tlc2_write(TLC2_CH_REV_PARAM_2, value); break;
        case K_REV_PARAM_3: revParam3 = value; tlc2_write(TLC2_CH_REV_PARAM_3, value); break;

        default:
            Serial.printf("[%c] param sconosciuto: '%c'=%u\n", MCU_ID, key, value);
            break;
    }
}