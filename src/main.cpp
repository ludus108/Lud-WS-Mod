// =====================================================================
// Lud-WS-Mod — RP2040 — modulo effetti (chorus + phaser + EQ)
// =====================================================================
// LWS  : Serial1 @ 115200 verso Router
// HW   : TLC5628 (DAC 8-bit, 8 ch) + AD5206 (6 pot digitali) via SPI
// LFO  : 3 sinusoidi a 1 kHz di update, uscita su TLC5628
// =====================================================================

#include <Arduino.h>
#include <SPI.h>

#include "mod_config.h"
#include "mod_state.h"
#include "mod_hardware.h"
#include "mod_lfo.h"

// Forward declaration dei callback LWS (usati da lws_set_callbacks in setup)
static void on_param(char target, char key, uint8_t value);
static void on_error(const char *msg);

// Layer LWS: include dopo le forward declaration
#include "comunicazioni_mcu.h"
#include "mod_lws.h"

// ---------------------------------------------------------------------
// SETUP
// ---------------------------------------------------------------------
void setup() {
    Serial.begin(115200);
    delay(200);
    Serial.printf("\n=== Lud-WS-Mod (MCU_ID='%c') ===\n", MCU_ID);

    // --- HW ---
    hw_init();

    // --- LFO ---
    lfo_recalc_increments();

    // --- LWS ---
    LWS_SERIAL.begin(LWS_BAUD_UP);
    lws_set_callbacks(on_param, on_error);

    Serial.println("Mod ready.");
}

// ---------------------------------------------------------------------
// LOOP
// ---------------------------------------------------------------------
void loop() {
    // --- LWS: poll + retry ACK ---
    lws_mcu_poll();

    // --- Tick LFO (autolimitato a 1 kHz) ---
    lfo_tick();

    // --- Debug locale ---
    if (Serial.available()) {
        char c = Serial.read();
        if (c == '?') dumpStatus();
    }
}