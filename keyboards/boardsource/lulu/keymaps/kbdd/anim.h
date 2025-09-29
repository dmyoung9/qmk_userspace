#pragma once

#include QMK_KEYBOARD_H
#include "progmem_anim.h"
#include "oled_utils.h"
#include "oled_unified_anim.h"  // Modern unified animation system
#include "oled_declarative.h"

#define LAYER_COUNT 5

// Modern unified animation system
void init_widgets(void);
void draw_wpm_frame(void);
void draw_logo(void);
void tick_widgets(void);

// Enhanced features
bool is_boot_animation_complete(void);
void trigger_layer_transition_effect(void);
