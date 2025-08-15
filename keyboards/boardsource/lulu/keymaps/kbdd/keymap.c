#include QMK_KEYBOARD_H
#include "constants.h"
#include "anim.h"
#include "wpm_stats.h"
#include "oled_utils.h"

// left-hand GACS
#define MOD_HLG MT(MOD_LGUI, KC_A)
#define MOD_HLA MT(MOD_LALT, KC_S)
#define MOD_HLS MT(MOD_LSFT, KC_D)
#define MOD_HLC MT(MOD_LCTL, KC_F)

// right-hand SCAG
#define MOD_HRC MT(MOD_RCTL, KC_J)
#define MOD_HRS MT(MOD_RSFT, KC_K)
#define MOD_HRA MT(MOD_RALT, KC_L)
#define MOD_HRG MT(MOD_RGUI, KC_SCLN)

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
	[_QWERTY] = LAYOUT(
	  KC_GRV , KC_1   , KC_2   , KC_3   , KC_4   , KC_5,                      KC_6   , KC_7   , KC_8   , KC_9   , KC_0   , KC_MINS,
	  KC_TAB , KC_Q   , KC_W   , KC_E   , KC_R   , KC_T,                      KC_Y   , KC_U   , KC_I   , KC_O   , KC_P   , KC_EQL ,
	  ADJUST , MOD_HLG, MOD_HLA, MOD_HLS, MOD_HLC, KC_G,                      KC_H   , MOD_HRC, MOD_HRS, MOD_HRA, MOD_HRG, KC_QUOT,
	  CW_TOGG, KC_Z   , KC_X   , KC_C   , KC_V   , KC_B,    KC_ESC , KC_MUTE, KC_N   , KC_M   , KC_COMM, KC_DOT , KC_SLSH, KC_BSLS,
								 KC_LGUI, LOWER  , KC_DEL , KC_BSPC, KC_SPC , KC_ENT , RAISE  , QK_MACRO_0
	),

	[_LOWER] = LAYOUT(
	  _______, _______, _______, _______, _______, _______,                   KC_LBRC, KC_P7  , KC_P8  , KC_P9  , KC_RBRC, _______,
	  _______, _______, _______, _______, _______, _______,                   KC_PMNS, KC_P4  , KC_P5  , KC_P6  , KC_PSLS, _______,
	  _______, _______, _______, _______, _______, _______,                   KC_PPLS, KC_P1  , KC_P2  , KC_P3  , KC_PAST, _______,
	  _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
								 _______, _______, _______, _______, KC_P0  , KC_PENT, KC_PDOT, _______
	),

	[_RAISE] = LAYOUT(
	  _______, _______, _______, _______, _______, _______,                   _______, _______, _______, _______, _______, _______,
	  _______, KC_MB2 , KC_MUP , KC_MB1 , _______, _______,                   KC_HOME, KC_PGDN, KC_PGUP, KC_END , _______, _______,
	  _______, KC_MLFT, KC_MDWN, KC_MRGT, _______, _______,                   KC_LEFT, KC_DOWN, KC_UP  , KC_RGHT, _______, _______,
	  _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
								 _______, _______, _______, _______, _______, _______, _______, _______
	),

	[_ADJUST] = LAYOUT(
	  _______, _______, _______, _______, _______, _______,                   _______, KC_F9  , KC_F10 , KC_F11 , KC_F12 , _______,
	  _______, _______, _______, _______, _______, _______,                   _______, KC_F5  , KC_F6  , KC_F7  , KC_F8  , _______,
	  _______, _______, _______, _______, _______, _______,                   _______, KC_F1  , KC_F2  , KC_F3  , KC_F4  , _______,
	  _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
								 _______, _______, _______, _______, _______, _______, _______, _______
	)
};

#ifdef ENCODER_MAP_ENABLE
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][NUM_DIRECTIONS] = {
    [0] = { ENCODER_CCW_CW(KC_VOLD, KC_VOLU) },
    [1] = { ENCODER_CCW_CW(KC_PGUP, KC_PGDN) },
    [2] = { ENCODER_CCW_CW(_______, _______) },
    [3] = { ENCODER_CCW_CW(_______, _______) },
};
#endif

#ifdef OLED_ENABLE
bool oled_task_user(void) {
    tick_widgets();

    if (!is_keyboard_master()) {
        draw_wpm_frame();
        wpm_stats_oled_render();
    } else {
        draw_logo();
    }

    return false;
}

oled_rotation_t oled_init_user(oled_rotation_t rotation) {
    return rotation;  // oriented correctly
}
#endif

void keyboard_post_init_user(void) {
    wpm_stats_init();
    wpm_stats_init_split_sync();
    wpm_stats_oled_init();
    oled_clear();

    init_widgets();
}

layer_state_t layer_state_set_user(layer_state_t state) {
    // Force immediate widget update on layer changes to prevent animation getting stuck
    // This ensures the widget sees layer changes immediately rather than waiting for OLED refresh
    tick_widgets();
    return state;
}

void matrix_scan_user(void) {
    // Handle WPM statistics periodic tasks (only on master)
    if (is_keyboard_master()) {
        wpm_stats_task();
    }
}

void housekeeping_task_user(void) {
    wpm_stats_housekeeping_task();
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    wpm_stats_on_keyevent(record);
    return true;
}

