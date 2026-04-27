#include <stdint.h>

#include QMK_KEYBOARD_H

enum layers { _QWERTY, _COLEMAK, _UNICODE, _NUM, _NAV, _FUNC };
enum { LAYER_COUNT = _FUNC + 1 };
enum { TD_CMD, TD_BLUETOOTH_MUTE };
enum custom_keycodes { CUS_TSK = SAFE_RANGE, CUS_SNT, CUS_SLK, CUS_CODE };

// simple layers, no tri-layer
#define NUM MO(_NUM)
#define NAV MO(_NAV)
#define FUNC MO(_FUNC)

#define QWE_CDH TG(_COLEMAK)
#define UNI_ON TT(_UNICODE)
#define UNI_OFF TG(_UNICODE)

// QWERTY
// left-hand GACS
#define QWE_HLG MT(MOD_LGUI, KC_A)
#define QWE_HLA MT(MOD_LALT, KC_S)
#define QWE_HLS MT(MOD_LSFT, KC_D)
#define QWE_HLC MT(MOD_LCTL, KC_F)

// right-hand SCAG
#define QWE_HRC MT(MOD_RCTL, KC_J)
#define QWE_HRS MT(MOD_RSFT, KC_K)
#define QWE_HRA MT(MOD_RALT, KC_L)
#define QWE_HRG MT(MOD_RGUI, KC_SCLN)

// COLEMAK
// left-hand GACS
#define CDH_HLG MT(MOD_LGUI, KC_A)
#define CDH_HLA MT(MOD_LALT, KC_R)
#define CDH_HLS MT(MOD_LSFT, KC_S)
#define CDH_HLC MT(MOD_LCTL, KC_T)

// right-hand SCAG
#define CDH_HRC MT(MOD_RCTL, KC_N)
#define CDH_HRS MT(MOD_RSFT, KC_E)
#define CDH_HRA MT(MOD_RALT, KC_I)
#define CDH_HRG MT(MOD_RGUI, KC_O)

// combos
#ifdef COMBO_ENABLE
enum combos {
    COMBO_LPAREN,
    COMBO_RPAREN,
    COMBO_LBRACK,
    COMBO_RBRACK,
    COMBO_LBRACE,
    COMBO_RBRACE,
};
#endif

// tap-dances
#define TD_BTTG TD(TD_BLUETOOTH_MUTE)
#define TD_FUNC TD(TD_CMD)

// shortcuts
#define CUS_GPT A(KC_SPC)

#define G_MIC LCS(KC_M)
#define G_CAM LCS(KC_O)
#define G_EMOJI G(KC_SCLN)
#define G_UP G(KC_UP)
#define G_DOWN G(KC_DOWN)
#define G_LEFT G(KC_LEFT)
#define G_RIGHT G(KC_RIGHT)
#define G_SWDSK LSG(KC_LEFT)
#define G_START G(KC_S)
#define G_DESK G(KC_D)
#define G_REC LSG(KC_R)
#define G_SNIP LSG(KC_S)
