#include QMK_KEYBOARD_H

#include "constants.h"
#include "anim.h"

#include "wpm_oled.h"
#include "oled_utils.h"
#include "dmyoung9/encoder_ledmap.h"
#include "elpekenin/indicators.h"

#define CAPS_WORD_LED_INDEX 24

// Task layer timeout functionality
static bool task_layer_active = false;
static uint32_t task_layer_timer = 0;
#define TASK_LAYER_TIMEOUT 3000  // 3000ms timeout

const indicator_t PROGMEM indicators[] = {
    // Initialize indicators
    ASSIGNED_KEYCODE_IN_LAYER_INDICATOR(_NUM, HUE(HUE_YELLOW)),
    ASSIGNED_KEYCODE_IN_LAYER_INDICATOR(_NAV, HUE(HUE_PURPLE)),
    ASSIGNED_KEYCODE_IN_LAYER_INDICATOR(_FUNC, HUE(HUE_ORANGE)),
    KEYCODE_INDICATOR(QK_BOOT, HUE(HUE_RED)),
    KEYCODE_INDICATOR(TD_FUNC, HUE(HUE_GREEN)),
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
    LAYER_INDICATOR(_TASK, HUE(HUE_PURPLE)),
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
// ----- STANDARD LAYERS -----
[_BASE] = LAYOUT(
    KC_GRV , KC_1   , KC_2   , KC_3   , KC_4   , KC_5   ,                   KC_6   , KC_7   , KC_8   , KC_9   , KC_0   , KC_MINS,
    KC_LBRC, KC_Q   , KC_W   , KC_E   , KC_R   , KC_T   ,                   KC_Y   , KC_U   , KC_I   , KC_O   , KC_P   , KC_RBRC,
    KC_TAB , MOD_HLG, MOD_HLA, MOD_HLS, MOD_HLC, KC_G   ,                   KC_H   , MOD_HRC, MOD_HRS, MOD_HRA, MOD_HRG, KC_QUOT,
    CW_TOGG, KC_Z   , KC_X   , KC_C   , KC_V   , KC_B   , KC_ESC , TD_BTTG, KC_N   , KC_M   , KC_COMM, KC_DOT , KC_SLSH, TD_FUNC,
                               _______, NUM    , KC_DEL , KC_BSPC, KC_SPC , KC_ENT , NAV    , CUS_GPT
),

[_NUM] = LAYOUT(
    _______, _______, _______, _______, _______, _______,                   XXXXXXX, XXXXXXX, KC_PSLS, KC_PAST, XXXXXXX, XXXXXXX,
    _______, _______, _______, G_EMOJI, _______, _______,                   KC_PMNS, KC_P7  , KC_P8  , KC_P9  , XXXXXXX, XXXXXXX,
    _______, _______, _______, _______, _______, _______,                   KC_PPLS, KC_P4  , KC_P5  , KC_P6  , XXXXXXX, XXXXXXX,
    _______, _______, _______, _______, _______, _______, _______, CUS_TSK, KC_PDOT, KC_P1  , KC_P2  , KC_P3  , KC_BSLS, XXXXXXX,
                               _______, _______, _______, _______, KC_P0  , KC_EQL , _______, KC_CALC
),

[_NAV] = LAYOUT(
    _______, _______, _______, _______, _______, _______,                   _______, _______, _______, _______, _______, _______,
    _______, _______, G_UP   , KC_MYCM, _______, G_SWDSK,                   KC_HOME, KC_PGDN, KC_PGUP, KC_END , _______, _______,
    _______, G_LEFT , G_DOWN , G_RIGHT, _______, G_START,                   KC_LEFT, KC_DOWN, KC_UP  , KC_RGHT, _______, _______,
    _______, _______, _______, _______, _______, G_DESK , _______, _______, _______, _______, _______, _______, _______, _______,
                               _______, _______, _______, _______, _______, _______, _______, _______
),

[_FUNC] = LAYOUT(
    QK_BOOT, _______, _______, _______, _______, _______,                   _______, _______, _______, _______, _______, _______,
    _______, _______, _______, _______, G_REC  , _______,                   G_SNIP , KC_F9  , KC_F10 , KC_F11 , KC_F12 , _______,
    _______, _______, _______, _______, _______, _______,                   _______, KC_F5  , KC_F6  , KC_F7  , KC_F8  , _______,
    _______, _______, _______, _______, _______, _______, LUMINO , _______, _______, KC_F1  , KC_F2  , KC_F3  , KC_F4  , _______,
                               _______, _______, _______, _______, _______, _______, _______, _______
),

[_TASK] = LAYOUT(
    _______, _______, _______, _______, _______, _______,                   _______, _______, _______, _______, _______, _______,
    _______, _______, _______, _______, _______, _______,                   _______, _______, _______, _______, _______, _______,
    _______, _______, _______, _______, _______, _______,                   _______, _______, _______, _______, _______, _______,
    _______, _______, _______, _______, _______, _______, KC_ESC ,  KC_TAB, _______, _______, _______, _______, _______, _______,
                               _______, _______, _______, _______, _______, KC_ENT , _______, _______
),
};

#ifdef ENCODER_MAP_ENABLE
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][NUM_DIRECTIONS] = {
    [0] = { ENCODER_CCW_CW(KC_VOLD, KC_VOLU) },
    [1] = { ENCODER_CCW_CW(S(KC_TAB), KC_TAB) },
    [2] = { ENCODER_CCW_CW(KC_PGUP, KC_PGDN) },
    [3] = { ENCODER_CCW_CW(_______, _______) },
    [4] = { ENCODER_CCW_CW(KC_LEFT, KC_RGHT) },
};

const uint8_t encoder_leds[NUM_ENCODERS] = { 65 };
const color_t PROGMEM encoder_ledmap[][NUM_ENCODERS][NUM_DIRECTIONS] = {
    [0] = { { HUE(HUE_RED), HUE(HUE_GREEN) } },
    [1] = { { HUE(HUE_YELLOW), HUE(HUE_YELLOW) } },
    [2] = { { HUE(HUE_PURPLE), HUE(HUE_PURPLE) } },
    [3] = { { HUE(HUE_ORANGE), HUE(HUE_ORANGE) } },
    [4] = { { HUE(HUE_PURPLE), HUE(HUE_PURPLE) } },
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
    [COMBO_LPAREN] = COMBO(lp_combo, KC_LPRN),   // (
    [COMBO_RPAREN] = COMBO(rp_combo, KC_RPRN),   // )
    [COMBO_LBRACK] = COMBO(lb_combo, KC_LBRC),   // [
    [COMBO_RBRACK] = COMBO(rb_combo, KC_RBRC),   // ]
    [COMBO_LBRACE] = COMBO(lc_combo, KC_LCBR),   // {
    [COMBO_RBRACE] = COMBO(rc_combo, KC_RCBR),   // }
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

    if (!is_keyboard_master()) {
        draw_wpm_frame();
        wpm_stats_oled_render();
    } else {
        tick_widgets();
        draw_logo();
    }

    return false;
}

oled_rotation_t oled_init_user(oled_rotation_t rotation) {
    return rotation;  // oriented correctly
}
#endif

void keyboard_post_init_user(void) {
    oled_clear();

    init_widgets();
}

layer_state_t layer_state_set_user(layer_state_t state) {
    tick_widgets();
#ifdef TRI_LAYER_ENABLE
    return update_tri_layer_state(state, _NUM, _NAV, _FUNC);
#endif
    return state;
}

void matrix_scan_user(void) {
    if (task_layer_active && timer_elapsed32(task_layer_timer) > TASK_LAYER_TIMEOUT) {
        tap_code(KC_ESC);
        layer_off(_TASK);
        task_layer_active = false;
    }
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {

    if (record->event.pressed) {
        if (task_layer_active) {
            task_layer_timer = timer_read32();
        }
    }

    switch (keycode) {
        case CUS_TSK:
            if (record->event.pressed) {
                tap_code16(G(KC_TAB));
                layer_on(_TASK);
                task_layer_active = true;
                task_layer_timer = timer_read32();
            }
            return false;

        case KC_ESC:
        case KC_ENT:
            if (record->event.pressed && task_layer_active) {
                layer_off(_TASK);
                task_layer_active = false;
            }
            break;
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

bool rgb_matrix_indicators_user(void) {
    if (is_caps_word_on()) {
        rgb_matrix_set_color(CAPS_WORD_LED_INDEX, 0, 0xff, 0);
    } else {
        rgb_matrix_set_color(CAPS_WORD_LED_INDEX, 0, 0, 0);
    }

    return true;
}

tap_dance_action_t tap_dance_actions[] = {
    [TD_CMD] = ACTION_TAP_DANCE_DOUBLE(C(KC_A), KC_COLN),
    [TD_BLUETOOTH_MUTE] = ACTION_TAP_DANCE_FN(td_bluetooth_mute_finished),
    [TD_SUPER_PAREN] = ACTION_TAP_DANCE_FN(td_super_paren_finished),
};
