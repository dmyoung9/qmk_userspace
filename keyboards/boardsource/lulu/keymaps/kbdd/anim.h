#pragma once

#include QMK_KEYBOARD_H
#include "progmem_anim.h"
#include "oled_declarative.h"

#define LAYER_COUNT 4

extern widget_t layer_widget;
extern const widget_config_t layer_widget_config;

void init_widgets(void);
void draw_wpm_frame(void);
void draw_logo(void);
void tick_widgets(void);
