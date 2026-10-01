#include <stdbool.h>
#include <stdint.h>

#include QMK_KEYBOARD_H

#include "constants.h"
#include "anim.h"

#include "wpm_oled.h"
#include "oled_utils.h"
#include "elpekenin/indicators.h"
#include "elpekenin/colors.h"

static bool     sft_os_held = false;
static bool     sft_os_registered = false;
static uint16_t sft_os_timer = 0;

const indicator_t PROGMEM indicators[] = {
    // Initialize indicators
    ASSIGNED_KEYCODE_IN_LAYER_INDICATOR(_MIRROR, HUE(HUE_BLUE)),
    ASSIGNED_KEYCODE_IN_LAYER_INDICATOR(_NAVSYM, HUE(HUE_PURPLE)),
    ASSIGNED_KEYCODE_IN_LAYER_INDICATOR(_BASE, WHITE_COLOR),
    KEYCODE_INDICATOR(GUI_ESC, HUE(HUE_ORANGE)),
    KEYCODE_INDICATOR(KC_SPC, HUE(HUE_ORANGE)),
    KEYCODE_INDICATOR(KC_BSPC, HUE(HUE_GREEN)),
    KEYCODE_INDICATOR(SFT_OS, HUE(HUE_GREEN)),
    KEYCODE_INDICATOR(TAB_NAV, HUE(HUE_GREEN)),
    KEYCODE_INDICATOR(KC_BSLS, HUE(HUE_GREEN)),
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    // ----- STANDARD LAYERS -----
    [_BASE] = LAYOUT(
        CTL_GRV, KC_1   , KC_2   , KC_3   , KC_4   , ALT_5  ,
        KC_BSLS, KC_Q   , KC_W   , KC_E   , KC_R   , KC_T   ,
        TAB_NAV, KC_A   , KC_S   , KC_D   , KC_F   , KC_G   ,
        SFT_OS , KC_Z   , KC_X   , KC_C   , KC_V   , KC_B   , GUI_ESC,
                          NAVSYM , MIRROR , KC_BSPC, KC_SPC
    ),

    [_MIRROR] = LAYOUT(
        CTL_MIN, KC_0   , KC_9   , KC_8   , KC_7   , ALT_6  ,
        _______, KC_P   , KC_O   , KC_I   , KC_U   , KC_Y   ,
        KC_QUOT, KC_SCLN, KC_L   , KC_K   , KC_J   , KC_H   ,
        SFT_OS , KC_SLSH, KC_DOT , KC_COMM, KC_M   , KC_N   , GUI_ENT,
                          _______, _______, _______, _______
    ),

    [_NAVSYM] = LAYOUT(
        _______, _______, _______, _______, _______, _______,
        _______, KC_LPRN, KC_LBRC, KC_LCBR, KC_UP  , _______,
        _______, KC_RPRN, KC_RBRC, KC_RCBR, KC_DOWN, _______,
        SFT_OS , _______, KC_LEFT, _______, KC_RGHT, _______, LUMINO,
                          _______, _______, _______, _______
    )
};

#ifdef OLED_ENABLE
bool oled_task_user(void) {
    if (last_input_activity_elapsed() < OLED_TIMEOUT) {
        oled_on();
    } else {
        oled_off();
        return false;
    }

    tick_widgets();
    draw_wpm_frame();

    return false;
}

oled_rotation_t oled_init_user(oled_rotation_t rotation) {
    return rotation; // oriented correctly
}
#endif

void keyboard_post_init_user(void) {
    oled_clear();

    init_widgets();
}

layer_state_t layer_state_set_user(layer_state_t state) {
    tick_widgets();

    return state;
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case SFT_OS:
            if (record->event.pressed) {
                sft_os_timer = timer_read();
                sft_os_held = true;
                sft_os_registered = false;
            } else {
                if (sft_os_registered) {
                    unregister_mods(MOD_BIT(KC_LSFT));
                } else {
                    set_oneshot_mods(MOD_BIT(KC_LSFT));
                }

                sft_os_held = false;
                sft_os_registered = false;
            }

            return false;
    }

    if (record->event.pressed && sft_os_held && !sft_os_registered) {
        register_mods(MOD_BIT(KC_LSFT));
        sft_os_registered = true;
    }

    return true;
}
