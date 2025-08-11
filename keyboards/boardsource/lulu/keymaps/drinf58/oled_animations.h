#pragma once

#include QMK_KEYBOARD_H

#define LAYER_FRAME_COUNT 7
#define LAYER_FRAME_WIDTH 11
#define LAYER_FRAME_PAGES 4

#define MOD_FRAME_COUNT 5
#define MOD_FRAME_WIDTH 16
#define MOD_FRAME_PAGES 2

#define ANIM_FRAME_MS 80

bool render_logo(void);
bool render_wpm(void);
bool render_layer_indicators(void);
bool render_modifier_indicators(void);

extern const char PROGMEM logo[];
extern const char PROGMEM wpm[];

extern const char PROGMEM base_0[];
extern const char PROGMEM base_1[];
extern const char PROGMEM base_2[];
extern const char PROGMEM base_3[];
extern const char PROGMEM base_4[];
extern const char PROGMEM base_5[];
extern const char PROGMEM base_6[];
extern const char * const base_frames[LAYER_FRAME_COUNT] PROGMEM;

extern const char PROGMEM num_0[];
extern const char PROGMEM num_1[];
extern const char PROGMEM num_2[];
extern const char PROGMEM num_3[];
extern const char PROGMEM num_4[];
extern const char PROGMEM num_5[];
extern const char PROGMEM num_6[];
extern const char * const num_frames[LAYER_FRAME_COUNT] PROGMEM;

extern const char PROGMEM nav_0[];
extern const char PROGMEM nav_1[];
extern const char PROGMEM nav_2[];
extern const char PROGMEM nav_3[];
extern const char PROGMEM nav_4[];
extern const char PROGMEM nav_5[];
extern const char PROGMEM nav_6[];
extern const char * const nav_frames[LAYER_FRAME_COUNT] PROGMEM;

extern const char PROGMEM func_0[];
extern const char PROGMEM func_1[];
extern const char PROGMEM func_2[];
extern const char PROGMEM func_3[];
extern const char PROGMEM func_4[];
extern const char PROGMEM func_5[];
extern const char PROGMEM func_6[];
extern const char * const func_frames[LAYER_FRAME_COUNT] PROGMEM;

extern const char PROGMEM ctrl_0[];
extern const char PROGMEM ctrl_1[];
extern const char PROGMEM ctrl_2[];
extern const char PROGMEM ctrl_3[];
extern const char PROGMEM ctrl_4[];
extern const char * const ctrl_frames[MOD_FRAME_COUNT] PROGMEM;

extern const char PROGMEM alt_0[];
extern const char PROGMEM alt_1[];
extern const char PROGMEM alt_2[];
extern const char PROGMEM alt_3[];
extern const char PROGMEM alt_4[];
extern const char * const alt_frames[MOD_FRAME_COUNT] PROGMEM;

extern const char PROGMEM shift_0[];
extern const char PROGMEM shift_1[];
extern const char PROGMEM shift_2[];
extern const char PROGMEM shift_3[];
extern const char PROGMEM shift_4[];
extern const char * const shift_frames[MOD_FRAME_COUNT] PROGMEM;

extern const char PROGMEM win_0[];
extern const char PROGMEM win_1[];
extern const char PROGMEM win_2[];
extern const char PROGMEM win_3[];
extern const char PROGMEM win_4[];
extern const char * const win_frames[MOD_FRAME_COUNT] PROGMEM;

extern const char PROGMEM digit_0[];
extern const char PROGMEM digit_1[];
extern const char PROGMEM digit_2[];
extern const char PROGMEM digit_3[];
extern const char PROGMEM digit_4[];
extern const char PROGMEM digit_5[];
extern const char PROGMEM digit_6[];
extern const char PROGMEM digit_7[];
extern const char PROGMEM digit_8[];
extern const char PROGMEM digit_9[];
extern const char * const digit_frames[10] PROGMEM;