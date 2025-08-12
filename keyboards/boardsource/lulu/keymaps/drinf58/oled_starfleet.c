#include QMK_KEYBOARD_H
#include "oled_starfleet.h"
#include "progmem_starfleet.h"
#include "oled_utils.h"

#define SLICE16x8(p) ((const slice_t){(const uint8_t PROGMEM *)(p), 16, 1})
#define SLICE8x32(p) ((const slice_t){(const uint8_t PROGMEM *)(p), 8, 4})
#define SLICE24x32(p) ((const slice_t){(const uint8_t PROGMEM *)(p), 24, 4})
#define SLICE128x32(p) ((const slice_t){(const uint8_t PROGMEM *)(p), 128, 4})

static const slice_t SLICE_kbd = SLICE128x32(kbd);

static const slice_t SLICE_wpm_title              = SLICE8x32(wpm_title);
static const slice_t SLICE_wpm_frame[LAYER_COUNT] = {SLICE16x8(wpm_frame_0), SLICE16x8(wpm_frame_1), SLICE16x8(wpm_frame_2), SLICE16x8(wpm_frame_2)};

static const slice_t SLICE_digits[10][2] = {
    {SLICE16x8(digit_0_trail), SLICE16x8(digit_0_lead)}, {SLICE16x8(digit_1_trail), SLICE16x8(digit_1_lead)}, {SLICE16x8(digit_2_trail), SLICE16x8(digit_2_lead)}, {SLICE16x8(digit_3_trail), SLICE16x8(digit_3_lead)}, {SLICE16x8(digit_4_trail), SLICE16x8(digit_4_lead)}, {SLICE16x8(digit_5_trail), SLICE16x8(digit_5_lead)}, {SLICE16x8(digit_6_trail), SLICE16x8(digit_6_lead)}, {SLICE16x8(digit_7_trail), SLICE16x8(digit_7_lead)}, {SLICE16x8(digit_8_trail), SLICE16x8(digit_8_lead)}, {SLICE16x8(digit_9_trail), SLICE16x8(digit_9_lead)},
};

static const slice_t SLICE_layer[LAYER_COUNT + 1][LAYER_COUNT] = {{SLICE8x32(layer_0_0), SLICE8x32(layer_0_1), SLICE8x32(layer_0_2), SLICE8x32(layer_0_3)}, {SLICE8x32(layer_1_0), SLICE8x32(layer_1_1), SLICE8x32(layer_1_2), SLICE8x32(layer_1_3)}, {SLICE8x32(layer_2_0), SLICE8x32(layer_2_1), SLICE8x32(layer_2_2), SLICE8x32(layer_2_3)}, {SLICE8x32(layer_3_0), SLICE8x32(layer_3_1), SLICE8x32(layer_3_2), SLICE8x32(layer_3_3)}, {SLICE8x32(layer_bottom_0), SLICE8x32(layer_bottom_0), SLICE8x32(layer_bottom_0), SLICE8x32(layer_bottom_3)}};

static const slice_t SLICE_ctrl[NUM_MOD_STATES][LAYER_COUNT]  = {{SLICE8x32(ctrl_0_0), SLICE8x32(ctrl_0_0), SLICE8x32(ctrl_0_0), SLICE8x32(ctrl_0_0)}, {SLICE8x32(ctrl_1_0), SLICE8x32(ctrl_1_0), SLICE8x32(ctrl_1_0), SLICE8x32(ctrl_1_0)}};
static const slice_t SLICE_shift[NUM_MOD_STATES][LAYER_COUNT] = {{SLICE8x32(shift_0_0), SLICE8x32(shift_0_1), SLICE8x32(shift_0_1), SLICE8x32(shift_0_1)}, {SLICE8x32(shift_1_0), SLICE8x32(shift_1_1), SLICE8x32(shift_1_1), SLICE8x32(shift_1_1)}};
static const slice_t SLICE_alt[NUM_MOD_STATES][LAYER_COUNT]   = {{SLICE8x32(alt_0_0), SLICE8x32(alt_0_0), SLICE8x32(alt_0_2), SLICE8x32(alt_0_2)}, {SLICE8x32(alt_1_0), SLICE8x32(alt_1_0), SLICE8x32(alt_1_2), SLICE8x32(alt_1_2)}};
static const slice_t SLICE_super[NUM_MOD_STATES][LAYER_COUNT] = {{SLICE8x32(super_0_0), SLICE8x32(super_0_0), SLICE8x32(super_0_0), SLICE8x32(super_0_3)}, {SLICE8x32(super_1_0), SLICE8x32(super_1_0), SLICE8x32(super_1_0), SLICE8x32(super_1_3)}};
static const slice_t SLICE_caps[NUM_MOD_STATES][LAYER_COUNT]  = {{SLICE8x32(caps_0_0), SLICE8x32(caps_0_0), SLICE8x32(caps_0_0), SLICE8x32(caps_0_0)}, {SLICE8x32(caps_1_0), SLICE8x32(caps_1_0), SLICE8x32(caps_1_0), SLICE8x32(caps_1_0)}};

static const slice_t SLICE_logo = SLICE24x32(logo);

void render_wpm(void) {
    const uint8_t         x                      = 104;         // fixed X
    const uint8_t         y_slot[WPM_MAX_DIGITS] = {24, 16, 8}; // ones, tens, hundreds, thousands (if ever)
    static const uint16_t place[3]               = {1, 10, 100};

    // 1) Read WPM (0..255 on QMK)
    uint8_t wpm = get_current_wpm();
    uint8_t sel = get_highest_layer(layer_state);

    // 2) How many digits?
    uint8_t nd = (wpm >= 100) ? 3 : (wpm >= 10) ? 2 : 1;

    // 3) Clear the whole column area (so shrinking 3→1 digits erases old glyphs)
    for (uint8_t s = nd; s < WPM_MAX_DIGITS; s++) {
        clear_span16(x, y_slot[s]);
    }

    // 4) Draw ones (bottom), tens (above), hundreds (topmost we use)
    for (uint8_t i = 0; i < nd; i++) {
        uint8_t digit   = (wpm / place[i]) % 10;
        bool    leading = (i == nd - 1) ? 1 : 0;

        draw_slice_px(&SLICE_digits[digit][leading], x, y_slot[i]);
    }

    draw_slice_px(&SLICE_wpm_title, 120, 0);
    draw_slice_px(&SLICE_wpm_frame[sel], x, 0);
}

void render_logo(void) {
    draw_slice_px(&SLICE_logo, 0, 0);
}

void render_slave(void) {
    draw_slice_px(&SLICE_kbd, 0, 0);
}

void render_layers(void) {
    uint8_t sel = get_highest_layer(layer_state);
    uint8_t x   = 64;

    for (uint8_t f = 0; f < 5; f++) {
        draw_slice_px(&SLICE_layer[4 - f][sel], x + (f * 8), 0);
    }
}

void render_modifiers(void) {
    uint8_t sel  = get_highest_layer(layer_state);
    uint8_t mods = get_mods() | get_oneshot_mods();

    bool ctrl_on  = mods & MOD_MASK_CTRL;
    bool shift_on = mods & MOD_MASK_SHIFT;
    bool alt_on   = mods & MOD_MASK_ALT;
    bool super_on = mods & MOD_MASK_GUI;
    bool caps_on  = host_keyboard_led_state().caps_lock || is_caps_word_on();

    const uint8_t x = 24;

    draw_slice_px(ctrl_on ? &SLICE_ctrl[1][sel] : &SLICE_ctrl[0][sel], x + 4 * 8, 0);
    draw_slice_px(shift_on ? &SLICE_shift[1][sel] : &SLICE_shift[0][sel], x + 3 * 8, 0);
    draw_slice_px(alt_on ? &SLICE_alt[1][sel] : &SLICE_alt[0][sel], x + 2 * 8, 0);
    draw_slice_px(super_on ? &SLICE_super[1][sel] : &SLICE_super[0][sel], x + 8, 0);
    draw_slice_px(caps_on ? &SLICE_caps[1][sel] : &SLICE_caps[0][sel], x, 0);
}
