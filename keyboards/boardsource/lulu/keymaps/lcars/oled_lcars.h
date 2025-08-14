#pragma once

#include QMK_KEYBOARD_H
#include "progmem_lcars.h"

#define LAYER_COUNT 4
#define WPM_MAX_DIGITS 3
#define NUM_MOD_STATES 2

void render_wpm(void);
void render_layers(void);
void render_modifiers(void);
void render_logo(void);
void render_slave(void);
