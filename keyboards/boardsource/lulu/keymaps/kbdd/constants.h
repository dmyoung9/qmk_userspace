#include QMK_KEYBOARD_H

enum layers { _BASE, _NUM, _NAV, _FUNC };
enum { TD_CMD, TD_BLUETOOTH_MUTE };

// simple layers, no tri-layer
#define NUM MO(_NUM)
#define NAV MO(_NAV)
#define FUNC MO(_FUNC)

// mouse macros to keep layout consistent
#define KC_MB1 KC_MS_BTN1
#define KC_MB2 KC_MS_BTN2
#define KC_MUP KC_MS_UP
#define KC_MDWN KC_MS_DOWN
#define KC_MLFT KC_MS_LEFT
#define KC_MRGT KC_MS_RIGHT

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

enum combos {
    COMBO_LPAREN,
    COMBO_RPAREN,
    COMBO_LBRACK,
    COMBO_RBRACK,
    COMBO_LBRACE,
    COMBO_RBRACE,
};

