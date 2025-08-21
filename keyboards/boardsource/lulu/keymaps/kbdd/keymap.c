#include QMK_KEYBOARD_H

#include "constants.h"
#include "anim.h"
#include "wpm_stats.h"
#include "oled_utils.h"
#include "elpekenin/indicators.h"
#include "encoder_led.h"

const indicator_t PROGMEM indicators[] = {
    // Initialize indicators
    KEYCODE_INDICATOR(NUM, HUE(HUE_YELLOW)),
    KEYCODE_INDICATOR(KC_ESC, HUE(HUE_YELLOW)),
    KEYCODE_INDICATOR(NAV, HUE(HUE_PURPLE)),
    KEYCODE_INDICATOR(FUNC, HUE(HUE_ORANGE)),
    KEYCODE_INDICATOR(KC_BSPC, HUE(HUE_RED)),
    KEYCODE_INDICATOR(KC_W, HUE(HUE_CYAN)),
    KEYCODE_INDICATOR(MOD_HLG, HUE(HUE_CYAN)),
    KEYCODE_INDICATOR(MOD_HLA, HUE(HUE_CYAN)),
    KEYCODE_INDICATOR(MOD_HLS, HUE(HUE_CYAN)),
    KEYCODE_INDICATOR(KC_H, HUE(HUE_CYAN)),
    KEYCODE_INDICATOR(MOD_HRC, HUE(HUE_CYAN)),
    KEYCODE_INDICATOR(MOD_HRS, HUE(HUE_CYAN)),
    KEYCODE_INDICATOR(MOD_HRA, HUE(HUE_CYAN)),
    KEYCODE_INDICATOR(TD(TD_SUPER_BRACKET), HUE(HUE_CYAN)),
    ASSIGNED_KEYCODE_IN_LAYER_INDICATOR(_NUM, HUE(HUE_YELLOW)),
    ASSIGNED_KEYCODE_IN_LAYER_INDICATOR(_NAV, HUE(HUE_PURPLE)),
    ASSIGNED_KEYCODE_IN_LAYER_INDICATOR(_FUNC, HUE(HUE_ORANGE)),
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
// ----- STANDARD LAYERS -----
[_BASE] = LAYOUT(
    KC_GRV , KC_1   , KC_2   , KC_3   , KC_4   , KC_5   ,                   KC_6   , KC_7   , KC_8   , KC_9   , KC_0   , KC_MINS,
    KC_TAB , KC_Q   , KC_W   , KC_E   , KC_R   , KC_T   ,                   KC_Y   , KC_U   , KC_I   , KC_O   , KC_P   , KC_BSLS,
    FUNC   , MOD_HLG, MOD_HLA, MOD_HLS, MOD_HLC, KC_G   ,                   KC_H   , MOD_HRC, MOD_HRS, MOD_HRA, MOD_HRG, KC_QUOT,
    CW_TOGG, KC_Z   , KC_X   , KC_C   , KC_V   , KC_B   , KC_ESC , KC_MUTE, KC_N   , KC_M   , KC_COMM, KC_DOT , KC_SLSH, TD(TD_CMD),
                  TD(TD_SUPER_BRACKET), NUM    , KC_DEL , KC_BSPC, KC_SPC , KC_ENT , NAV    , A(KC_SPC)
),

[_NUM] = LAYOUT(
    _______, _______, _______, _______, _______, _______,                   XXXXXXX, XXXXXXX, KC_PSLS, KC_PAST, XXXXXXX, XXXXXXX,
    _______, _______, _______, _______, _______, _______,                   KC_PMNS, KC_P7  , KC_P8  , KC_P9  , XXXXXXX, XXXXXXX,
    _______, _______, _______, _______, _______, _______,                   KC_PPLS, KC_P4  , KC_P5  , KC_P6  , XXXXXXX, XXXXXXX,
    _______, _______, _______, _______, _______, _______, _______, _______, KC_PDOT, KC_P1  , KC_P2  , KC_P3  , XXXXXXX, XXXXXXX,
                               _______, _______, _______, _______, KC_P0  , KC_EQL , _______, KC_CALC
),

[_NAV] = LAYOUT(
    _______, _______, _______, _______, _______, _______,                   _______, _______, _______, _______, _______, _______,
    _______, _______, _______, KC_MYCM, _______, LSG(KC_LEFT),              KC_HOME, KC_PGDN, KC_PGUP, KC_END , _______, _______,
    _______, _______, _______, _______, _______, _______,                   KC_LEFT, KC_DOWN, KC_UP  , KC_RGHT, _______, _______,
    _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
                               _______, _______, _______, _______, _______, _______, _______, _______
),

[_FUNC] = LAYOUT(
    _______, _______, _______, _______, _______, _______,                   _______, _______, _______, _______, _______, _______,
    _______, _______, _______, _______, LSG(KC_R), _______,               LSG(KC_S), KC_F9  , KC_F10 , KC_F11 , KC_F12 , _______,
    _______, _______, _______, _______, _______, _______,                   _______, KC_F5  , KC_F6  , KC_F7  , KC_F8  , _______,
    _______, _______, _______, _______, _______, _______, _______, _______, _______, KC_F1  , KC_F2  , KC_F3  , KC_F4  , _______,
                               _______, _______, _______, _______, _______, _______, _______, _______
),
};

#ifdef ENCODER_MAP_ENABLE
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][NUM_DIRECTIONS] = {
    [0] = { ENCODER_CCW_CW(KC_VOLD, KC_VOLU) },
    [1] = { ENCODER_CCW_CW(S(KC_TAB), KC_TAB) },
    [2] = { ENCODER_CCW_CW(KC_PGUP, KC_PGDN) },
    [3] = { ENCODER_CCW_CW(_______, _______) },
};
#endif

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
    if (!is_keyboard_master()) {
        draw_wpm_frame();
        wpm_stats_oled_render();
    } else {
        draw_logo();
        tick_widgets();
    }

    return false;
}

oled_rotation_t oled_init_user(oled_rotation_t rotation) {
    return rotation; // oriented correctly
}
#endif

void keyboard_post_init_user(void) {
    wpm_stats_init();
    wpm_stats_init_split_sync();
    wpm_stats_oled_init();

    encoder_led_sync_init_split_sync();

    oled_clear();

    init_widgets();
}

layer_state_t layer_state_set_user(layer_state_t state) {
    tick_widgets();
    return state;
}

void matrix_scan_user(void) {
    if (is_keyboard_master()) {
        wpm_stats_task();
    }
}

void housekeeping_task_user(void) {
    wpm_stats_housekeeping_task();
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    wpm_stats_on_keyevent(record);
    encoder_led_sync_on_keyevent(record);

    return true;
}

void td_super_bracket_finished(tap_dance_state_t *state, void *user_data) {
    uint8_t mods = get_mods();

    bool ctrl  = (mods & MOD_MASK_CTRL) != 0;
    bool shift = (mods & MOD_MASK_SHIFT) != 0;

    clear_mods();

    if (state->count == 1) {
        // single tap
        if (ctrl && !shift) {
            tap_code(KC_LBRC); // [
        } else if (shift && !ctrl) {
            tap_code16(S(KC_LBRC)); // {
        } else {
            tap_code16(S(KC_9)); // (
        }
    } else if (state->count == 2) {
        // double tap
        if (ctrl && !shift) {
            tap_code(KC_RBRC); // ]
        } else if (shift && !ctrl) {
            tap_code16(S(KC_RBRC)); // }
        } else {
            tap_code16(S(KC_0)); // )
        }
    }

    set_mods(mods);
}

bool rgb_matrix_indicators_user(void) {
    encoder_led_sync_rgb_task();

    return false;
}

tap_dance_action_t tap_dance_actions[] = {
    [TD_CMD]           = ACTION_TAP_DANCE_DOUBLE(C(KC_A), KC_COLN),
    [TD_SUPER_BRACKET] = ACTION_TAP_DANCE_FN(td_super_bracket_finished),
};
