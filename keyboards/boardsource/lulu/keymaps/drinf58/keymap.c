// Copyright 2022 Cole Smith <cole@boadsource.xyz>
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H
#include "oled_starfleet.h"
#include "constants.h"


#include "typing_stats_public.h"

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

enum custom_keycodes {
    TS_PRNT = SAFE_RANGE,
    ALT_SPC
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
	[_QWERTY] = LAYOUT(
	  KC_GRV , KC_1   , KC_2   , KC_3   , KC_4   , KC_5,                      KC_6   , KC_7   , KC_8   , KC_9   , KC_0   , KC_MINS,
	  KC_TAB , KC_Q   , KC_W   , KC_E   , KC_R   , KC_T,                      KC_Y   , KC_U   , KC_I   , KC_O   , KC_P   , KC_EQL ,
	  ADJUST , MOD_HLG, MOD_HLA, MOD_HLS, MOD_HLC, KC_G,                      KC_H   , MOD_HRC, MOD_HRS, MOD_HRA, MOD_HRG, KC_QUOT,
	  CW_TOGG, KC_Z   , KC_X   , KC_C   , KC_V   , KC_B,    KC_ESC , KC_MUTE, KC_N   , KC_M   , KC_COMM, KC_DOT , KC_SLSH, KC_BSLS,
								 KC_LGUI, LOWER  , KC_DEL , KC_BSPC, KC_SPC , KC_ENT , RAISE  , ALT_SPC
	),

	[_LOWER] = LAYOUT(
	  TS_PRNT, _______, _______, _______, _______, _______,                   KC_LBRC, KC_P7  , KC_P8  , KC_P9  , KC_RBRC, _______,
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

// =============================================================================
// TYPING STATISTICS INTEGRATION
// =============================================================================

void keyboard_post_init_user(void) {
    // Initialize typing statistics module
    ts_init();
}

void matrix_scan_user(void) {
    // Handle typing statistics periodic tasks
    ts_task_10ms();
}


static void ts_print_all_stats(void);

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    // Track key events for typing statistics
    ts_on_keyevent(record, keycode);

    // Handle custom keycodes and macros
    if (record->event.pressed) {
        switch (keycode) {
            case TS_PRNT:  // Handle the macro that was in keymap.json
                ts_print_all_stats();
                return false;
            case ALT_SPC:  // Alt+Space macro (was previously in keymap.json)
                tap_code16(LALT(KC_SPC));
                return false;
            default:
                break;
        }
    }

    return true;
}

layer_state_t layer_state_set_user(layer_state_t state) {
    // Track layer changes for typing statistics
    return ts_on_layer_change(state);
}

void eeconfig_init_user(void) {
    // Handle EEPROM initialization for typing statistics
    ts_eeconfig_init_user();
}

#ifdef OLED_ENABLE

bool oled_task_user(void) {
  if (!is_keyboard_master()) {
	  render_slave();
  } else {
      render_logo();
	  render_modifiers();
	  render_layers();
	  render_wpm();
  }

  return false;
}

oled_rotation_t oled_init_user(oled_rotation_t rotation) {
    return rotation;  // oriented correctly
}
#endif

static void ts_print_all_stats(void) {
    // Use the public API to get summary
    ts_summary_t s;
    if (!ts_get_summary(&s)) {
        uprintf("[TS] Not initialized\n");
        return;
    }

    uint16_t left_pct = (uint16_t)(s.left_hand_ratio * 100.0f + 0.5f);

    uprintf("\n========== TYPING STATS ==========\n");
    uprintf("[TS] lifetime=%lu  session=%lu\n", (unsigned long)s.total_lifetime_presses, (unsigned long)s.session_presses);
    uprintf("[TS] wpm_cur=%u  wpm_avg=%u  wpm_max=%u  wpm_session_max=%u\n",
            s.current_wpm, s.avg_wpm, s.max_wpm, s.session_max_wpm);
    uprintf("[TS] left_ratio=%u%%  most_used_layer=%u  most_used_mod=%u  most_used_pos=%u\n",
            left_pct, s.most_used_layer, s.most_used_mod, s.most_used_pos_index);

#if TS_ENABLE_ADVANCED_ANALYSIS
    float entropy_f = ts_calculate_key_entropy();
    uint32_t entropy_m = (uint32_t)(entropy_f * 1000.0f + 0.5f); // print milli-entropy to avoid float printf
    uprintf("[TS] key_entropy(milli-bits)=%lu\n", (unsigned long)entropy_m);
#endif

    // ---------- Session/time ----------
    uint32_t mins = ts_get_session_time_minutes();
    uprintf("[TS] session_time_min=%lu\n", (unsigned long)mins);

    // ---------- Hand balance ----------
    float right_ratio = ts_get_right_hand_ratio();
    uint16_t right_pct = (uint16_t)(right_ratio * 100.0f + 0.5f);
    uprintf("[TS] right_ratio=%u%%\n", right_pct);

#if TS_ENABLE_ADVANCED_ANALYSIS
    // ---------- Patterns ----------
    uint32_t same_finger = ts_get_same_finger_presses();
    uint32_t finger_rolls = ts_get_finger_rolls();
    uprintf("[TS] same_finger=%lu  finger_rolls=%lu\n",
            (unsigned long)same_finger, (unsigned long)finger_rolls);
#endif

    // ---------- Layers ----------
    uprintf("[TS] Layers:\n");
    for (uint8_t i = 0; i < 8; i++) {  // Check first 8 layers
        uint32_t presses = ts_get_layer_presses(i);
        if (presses > 0) {
#if TS_ENABLE_LAYER_TIME
            uint32_t ms = ts_get_layer_time_ms(i);
            float ratio_f = ts_get_layer_time_ratio(i);
            uint16_t ratio_pct = (uint16_t)(ratio_f * 100.0f + 0.5f);
            uprintf("  [L%u] presses=%lu  time_ms=%lu  time_ratio=%u%%\n",
                    i, (unsigned long)presses, (unsigned long)ms, ratio_pct);
#else
            uprintf("  [L%u] presses=%lu\n", i, (unsigned long)presses);
#endif
        }
    }

    // ---------- Modifiers ----------
    uprintf("[TS] Modifiers:\n");
    for (uint8_t i = 0; i < 8; i++) {
        uint32_t cnt = ts_get_modifier_presses(i);
        if (cnt > 0) {
            const char *name = ts_get_modifier_name(i);
            uprintf("  [%u:%s] presses=%lu\n", i, name ? name : "?", (unsigned long)cnt);
        }
    }

    // ---------- Most used key position ----------
    uint8_t most_row, most_col;
    uint32_t most_count;
    if (ts_find_most_used_key(&most_row, &most_col, &most_count)) {
        uprintf("[TS] most_used_key=(%u,%u) count=%lu\n", most_row, most_col, (unsigned long)most_count);
    }

#if TS_ENABLE_BIGRAM_STATS
    // ---------- Most common bigram ----------
    uint8_t p1, p2;
    uint16_t bigram_count;
    if (ts_find_most_common_bigram(&p1, &p2, &bigram_count)) {
        uprintf("[TS] most_common_bigram=(%u -> %u) count=%u\n", p1, p2, bigram_count);
    }
#endif

#if TS_ENABLE_ADVANCED_ANALYSIS
    // ---------- Advanced analysis ----------
    float hb = ts_calculate_hand_balance_score();
    uint16_t hb_pct = (uint16_t)(hb * 100.0f + 0.5f);
    uprintf("[TS] hand_balance_score=%u%%\n", hb_pct);

    uint32_t alternating = ts_count_alternating_hands();
    uprintf("[TS] alternating_hands=%lu\n", (unsigned long)alternating);
#endif

    uprintf("========== END TYPING STATS ==========\n\n");
}

