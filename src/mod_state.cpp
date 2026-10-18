#include "mod_state.h"

uint8_t chorusMode     = 0;
uint8_t crsLfoSlowRate = 128;
uint8_t crsLfoSlowGain = 200;
uint8_t crsLfoFastRate = 128;
uint8_t crsLfoFastGain = 200;
uint8_t crsOffset      = 128;
uint8_t crsEq[6]       = {128, 128, 128, 128, 128, 128};
uint8_t phaserRate     = 128;
uint8_t phaserDepth    = 128;
uint8_t phaserFb       = 128;

volatile uint32_t phaseSlow    = 0;
volatile uint32_t phaseFast    = 0;
volatile uint32_t phasePhaser  = 0;

volatile uint32_t incSlow    = 0;
volatile uint32_t incFast    = 0;
volatile uint32_t incPhaser  = 0;

uint32_t g_frameFromRouter = 0;
uint32_t g_pongSent        = 0;
uint32_t g_paramApplied    = 0;