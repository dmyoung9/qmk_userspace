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
    /*
    ASSIGNED_KEYCODE_IN_LAYER_INDICATOR(_NUM, HUE(HUE_YELLOW)),
    ASSIGNED_KEYCODE_IN_LAYER_INDICATOR(_NAV, HUE(HUE_PURPLE)),
    ASSIGNED_KEYCODE_IN_LAYER_INDICATOR(_FUNC, HUE(HUE_ORANGE)),
    ASSIGNED_KEYCODE_IN_LAYER_INDICATOR(_UNICODE, HUE(HUE_BLUE)),
    */
    KEYCODE_INDICATOR(QK_BOOT, HUE(HUE_RED)),
    KEYCODE_INDICATOR(CW_TOGG, HUE(HUE_MAGENTA)),
    //KEYCODE_INDICATOR(NUM, WHITE_COLOR),
    KEYCODE_INDICATOR(KC_ESC, HUE(HUE_MAGENTA)),
    KEYCODE_INDICATOR(BASE, WHITE_COLOR),
    //KEYCODE_INDICATOR(UNICODE, HUE(HUE_CYAN)),
    KEYCODE_INDICATOR(TD_FUNC, HUE(HUE_CYAN)),
    KEYCODE_INDICATOR(TD_BTTG, HUE(HUE_CYAN)),
    //KEYCODE_INDICATOR(NAV, WHITE_COLOR),
    KEYCODE_INDICATOR(KC_W, HUE(HUE_MAGENTA)),
    KEYCODE_INDICATOR(MOD_HLG, HUE(HUE_MAGENTA)),
    KEYCODE_INDICATOR(MOD_HLA, HUE(HUE_MAGENTA)),
    KEYCODE_INDICATOR(MOD_HLS, HUE(HUE_MAGENTA)),
    KEYCODE_INDICATOR(KC_H, HUE(HUE_CYAN)),
    KEYCODE_INDICATOR(MOD_HRC, HUE(HUE_CYAN)),
    KEYCODE_INDICATOR(MOD_HRS, HUE(HUE_CYAN)),
    KEYCODE_INDICATOR(MOD_HRA, HUE(HUE_CYAN)),
    // LAYER_INDICATOR(_TASK, HUE(HUE_PURPLE)),
    //LAYER_INDICATOR(_GAME, HUE(HUE_GREEN)),
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    // ----- STANDARD LAYERS -----
    [_BASE] = LAYOUT(
        CTL_GRV, KC_1   , KC_2   , KC_3   , KC_4   , ALT_5  ,
        KC_BSLS, KC_Q   , KC_W   , KC_E   , KC_R   , KC_T   ,
        KC_TAB , KC_A   , KC_S   , KC_D   , KC_F   , KC_G   ,
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
        SFT_OS , _______, KC_LEFT, _______, KC_RGHT, _______, _______,
                          _______, _______, _______, _______
    )
};

#ifdef COMBO_ENABLE
const uint16_t PROGMEM lp_combo[] = {KC_Y, KC_U, COMBO_END};
const uint16_t PROGMEM rp_combo[] = {KC_N, KC_M, COMBO_END};
const uint16_t PROGMEM lb_combo[] = {KC_U, KC_I, COMBO_END};
const uint16_t PROGMEM rb_combo[] = {KC_M, KC_COMM, COMBO_END};
const uint16_t PROGMEM lc_combo[] = {KC_I, KC_O, COMBO_END};
const uint16_t PROGMEM rc_combo[] = {KC_COMM, KC_DOT, COMBO_END};

combo_t key_combos[] = {
    [COMBO_LPAREN] = COMBO(lp_combo, KC_LPRN), // (
    [COMBO_RPAREN] = COMBO(rp_combo, KC_RPRN), // )
    [COMBO_LBRACK] = COMBO(lb_combo, KC_LBRC), // [
    [COMBO_RBRACK] = COMBO(rb_combo, KC_RBRC), // ]
    [COMBO_LBRACE] = COMBO(lc_combo, KC_LCBR), // {
    [COMBO_RBRACE] = COMBO(rc_combo, KC_RCBR), // }
};
#endif

#ifdef OLED_ENABLE
bool oled_task_user(void) {
    if (last_input_activity_elapsed() < OLED_TIMEOUT) {
        oled_on();
    } else {
        oled_off();
        return false;
    }

    if (is_keyboard_master()) {
        tick_widgets();
        draw_wpm_frame();
    }

    return false;
}

oled_rotation_t oled_init_user(oled_rotation_t rotation) {
    return rotation; // oriented correctly
}
#endif

void keyboard_post_init_user(void) {
    oled_clear();

    if (is_keyboard_master()) {
        init_widgets();
    }
}

layer_state_t layer_state_set_user(layer_state_t state) {
    if (is_keyboard_master()) {
        tick_widgets();
    }

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

void td_bluetooth_mute_finished(tap_dance_state_t *state, void *user_data) {
    if (state->count == 1) {
        // single tap
        tap_code(KC_MUTE);
    } else if (state->count == 2) {
        tap_code16(G(KC_A));
        wait_ms(500);
        tap_code(KC_RIGHT);
        wait_ms(500);
        tap_code(KC_SPC);
        wait_ms(500);
        tap_code(KC_ESC);
    }
}

void td_super_paren_finished(tap_dance_state_t *state, void *user_data) {
    if (state->count == 1) {
        // single tap
        tap_code16(S(KC_9));
    } else if (state->count == 2) {
        // double tap
        tap_code16(S(KC_0));
    }
}

tap_dance_action_t tap_dance_actions[] = {
    [TD_CMD]            = ACTION_TAP_DANCE_DOUBLE(C(KC_A), KC_COLN),
    [TD_BLUETOOTH_MUTE] = ACTION_TAP_DANCE_FN(td_bluetooth_mute_finished),
};
