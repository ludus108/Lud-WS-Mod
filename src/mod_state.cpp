#include "mod_state.h"
extern uint8_t crsLevel;         // 0..255 (livello chorus su AD2 CH4)
uint8_t crsLevel = 128;
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

uint8_t dlyAMode    = 0;
uint8_t dlyATime    = 128;
uint8_t dlyAFb      = 128;
uint8_t dlyALfo     = 0;      // depth, default off
uint8_t dlyALfoRate = 128;
uint8_t dlyAVcfMode = 0;
uint8_t dlyAVcfCut  = 128;
uint8_t dlyAVcfRes  = 128;
uint8_t dlyAVcfLfo  = 0;      // depth, default off
uint8_t dlyAVcfLfoRate = 128;

uint8_t dlyBMode    = 0;
uint8_t dlyBTime    = 128;
uint8_t dlyBFb      = 128;
uint8_t dlyBLfo     = 0;
uint8_t dlyBLfoRate = 128;
uint8_t dlyBVcfMode = 0;
uint8_t dlyBVcfCut  = 128;
uint8_t dlyBVcfRes  = 128;
uint8_t dlyBVcfLfo  = 0;
uint8_t dlyBVcfLfoRate = 128;

uint8_t revSel = 0, revParam1 = 128, revParam2 = 128, revParam3 = 128;

volatile uint32_t phaseSlow = 0, phaseFast = 0, phasePhaser = 0;
volatile uint32_t phaseDlyA = 0, phaseVcfDlyA = 0;
volatile uint32_t phaseDlyB = 0, phaseVcfDlyB = 0;

volatile uint32_t incSlow = 0, incFast = 0, incPhaser = 0;
volatile uint32_t incDlyA = 0, incVcfDlyA = 0;
volatile uint32_t incDlyB = 0, incVcfDlyB = 0;

uint32_t g_frameFromRouter = 0;
uint32_t g_pongSent        = 0;
uint32_t g_paramApplied    = 0;