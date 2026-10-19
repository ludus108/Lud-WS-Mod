// =====================================================================
// Lud-WS-Mod — RP2040 — modulo effetti (chorus + phaser + 2 delay + EQ + reverb)
// =====================================================================
// LWS  : Serial1 @ 115200 verso Router
// HW   : 2x TLC5628 (DAC 8-bit), 2x AD5206 (6 pot), 2x TPL0502 (digi-pot)
// LFO  : 7 sinusoidi a 1 kHz, uscita su TLC5628 / TPL0502
// =====================================================================

#include <Arduino.h>
#include <SPI.h>

#include "mod_config.h"
#include "mod_state.h"
#include "mod_hardware.h"
#include "mod_lfo.h"

static void on_param(char target, char key, uint8_t value);
static void on_error(const char *msg);

#include "comunicazioni_mcu.h"
#include "mod_lws.h"

void setup() {
    Serial.begin(115200);
    delay(200);
    Serial.printf("\n=== Lud-WS-Mod (MCU_ID='%c') ===\n", MCU_ID);

    hw_init();
    lfo_recalc_increments();

    LWS_SERIAL.begin(LWS_BAUD_UP);
    lws_set_callbacks(on_param, on_error);

    Serial.println("Mod ready.");
}

void loop() {
    lws_mcu_poll();
    lfo_tick();

    if (Serial.available()) {
        char c = Serial.read();
        if (c == '?') dumpStatus();
    }
}