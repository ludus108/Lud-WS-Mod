#pragma once
#include "mod_config.h"

// =========================================================================
// mod_state.h — Variabili globali di stato
// =========================================================================

// --- Parametri (0..255, mode 0/1) ---
extern uint8_t chorusMode;
extern uint8_t crsLevel;         // 0..255 (livello chorus su AD2 CH4)
extern uint8_t crsLfoSlowRate, crsLfoSlowGain;
extern uint8_t crsLfoFastRate, crsLfoFastGain;
extern uint8_t crsOffset;
extern uint8_t crsEq[6];
extern uint8_t phaserRate, phaserDepth, phaserFb;

extern uint8_t dlyAMode,   dlyATime,   dlyAFb;
extern uint8_t dlyALfo,    dlyALfoRate;
extern uint8_t dlyAVcfMode, dlyAVcfCut, dlyAVcfRes;
extern uint8_t dlyAVcfLfo, dlyAVcfLfoRate;

extern uint8_t dlyBMode,   dlyBTime,   dlyBFb;
extern uint8_t dlyBLfo,    dlyBLfoRate;
extern uint8_t dlyBVcfMode, dlyBVcfCut, dlyBVcfRes;
extern uint8_t dlyBVcfLfo, dlyBVcfLfoRate;

extern uint8_t revSel, revParam1, revParam2, revParam3;

// --- Fase LFO (0..2^32-1) ---
extern volatile uint32_t phaseSlow,     phaseFast,     phasePhaser;
extern volatile uint32_t phaseDlyA,     phaseVcfDlyA;
extern volatile uint32_t phaseDlyB,     phaseVcfDlyB;

// --- Incrementi fase ---
extern volatile uint32_t incSlow,   incFast,   incPhaser;
extern volatile uint32_t incDlyA,   incVcfDlyA;
extern volatile uint32_t incDlyB,   incVcfDlyB;

// --- Diagnostica ---
extern uint32_t g_frameFromRouter;
extern uint32_t g_pongSent;
extern uint32_t g_paramApplied;