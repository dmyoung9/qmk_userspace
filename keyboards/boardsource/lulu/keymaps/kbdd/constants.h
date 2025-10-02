#include QMK_KEYBOARD_H

enum layers { _BASE, _NUM, _NAV, _FUNC, _TASK };
enum { TD_CMD, TD_BLUETOOTH_MUTE, TD_SUPER_PAREN };
enum custom_keycodes { CUS_TSK = SAFE_RANGE };

// simple layers, no tri-layer
#define NUM MO(_NUM)
#define NAV MO(_NAV)
#define FUNC MO(_FUNC)

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
#define OSM_SFT OSM(MOD_LSFT)

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
