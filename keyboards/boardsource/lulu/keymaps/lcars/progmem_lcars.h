#pragma once

#include QMK_KEYBOARD_H

extern const uint8_t PROGMEM kbd[];

extern const uint8_t PROGMEM wpm_title[];  // 8x32
extern const uint8_t PROGMEM wpm_frame_0[];  // 16x32
extern const uint8_t PROGMEM wpm_frame_1[];  // 16x32
extern const uint8_t PROGMEM wpm_frame_2[];  // 16x32

extern const uint8_t PROGMEM digit_0_lead[], digit_0_trail[];  // 16x8
extern const uint8_t PROGMEM digit_1_lead[], digit_1_trail[];  // 16x8
extern const uint8_t PROGMEM digit_2_lead[], digit_2_trail[];  // 16x8
extern const uint8_t PROGMEM digit_3_lead[], digit_3_trail[];  // 16x8
extern const uint8_t PROGMEM digit_4_lead[], digit_4_trail[];  // 16x8
extern const uint8_t PROGMEM digit_5_lead[], digit_5_trail[];  // 16x8
extern const uint8_t PROGMEM digit_6_lead[], digit_6_trail[];  // 16x8
extern const uint8_t PROGMEM digit_7_lead[], digit_7_trail[];  // 16x8
extern const uint8_t PROGMEM digit_8_lead[], digit_8_trail[];  // 16x8
extern const uint8_t PROGMEM digit_9_lead[], digit_9_trail[];  // 16x8

extern const uint8_t PROGMEM layer_0_0[], layer_0_1[], layer_0_2[], layer_0_3[];  // 8x32
extern const uint8_t PROGMEM layer_1_0[], layer_1_1[], layer_1_2[], layer_1_3[];  // 8x32
extern const uint8_t PROGMEM layer_2_0[], layer_2_1[], layer_2_2[], layer_2_3[];  // 8x32
extern const uint8_t PROGMEM layer_3_0[], layer_3_1[], layer_3_2[], layer_3_3[];  // 8x32
extern const uint8_t PROGMEM layer_bottom_0[], layer_bottom_3[];  // 8x32

extern const uint8_t PROGMEM ctrl_0_0[], ctrl_1_0[];  // 8x32
extern const uint8_t PROGMEM shift_0_0[], shift_0_1[], shift_1_0[], shift_1_1[];  // 8x32
extern const uint8_t PROGMEM alt_0_0[], alt_0_2[], alt_1_0[], alt_1_2[];  // 8x32
extern const uint8_t PROGMEM super_0_0[], super_0_3[], super_1_0[], super_1_3[];  // 8x32
extern const uint8_t PROGMEM caps_0_0[], caps_1_0[];  // 8x32

extern const uint8_t PROGMEM logo[];  // 24x32