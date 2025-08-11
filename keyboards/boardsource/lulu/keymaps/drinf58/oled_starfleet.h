#pragma once

#include QMK_KEYBOARD_H
#include "progmem_starfleet.h"

#define LAYER_COUNT 4
#define WPM_MAX_DIGITS 3
#define NUM_MOD_STATES 2

typedef struct {
    const uint8_t PROGMEM *data;  // pointer to page-packed bitmap
    uint8_t width; // bytes per page (== pixels wide)
    uint8_t pages; // height/8 (32px -> 4)
} slice_t;

void render_wpm(void);
void render_layers(void);
void render_modifiers(void);
void render_logo(void);
void render_slave(void);