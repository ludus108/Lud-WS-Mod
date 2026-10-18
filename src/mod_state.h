#pragma once
#include "mod_config.h"

// =========================================================================
// mod_state.h — Variabili globali di stato
// =========================================================================

// --- Parametri correnti (0..255, chorus_mode 0/1) ---
extern uint8_t chorusMode;
extern uint8_t crsLfoSlowRate;   // 0..255 (indice rate)
extern uint8_t crsLfoSlowGain;   // 0..255
extern uint8_t crsLfoFastRate;
extern uint8_t crsLfoFastGain;
extern uint8_t crsOffset;
extern uint8_t crsEq[6];         // EQ 1..6
extern uint8_t phaserRate;       // 0..255
extern uint8_t phaserDepth;      // 0..255
extern uint8_t phaserFb;         // 0..255 (per ora solo memorizzato)

// --- Fase LFO (0..UINT32_MAX) ---
extern volatile uint32_t phaseSlow;
extern volatile uint32_t phaseFast;
extern volatile uint32_t phasePhaser;

// --- Incremento di fase (aggiornato quando cambia il rate) ---
extern volatile uint32_t incSlow;
extern volatile uint32_t incFast;
extern volatile uint32_t incPhaser;

// --- Contatori diagnostici ---
extern uint32_t g_frameFromRouter;
extern uint32_t g_pongSent;
extern uint32_t g_paramApplied;