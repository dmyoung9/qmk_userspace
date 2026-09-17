#include <stdint.h>

#include QMK_KEYBOARD_H

enum layers { _BASE, _MIRROR, _NAVSYM };
enum { LAYER_COUNT = _NAVSYM + 1 };
enum { TD_CMD, TD_BLUETOOTH_MUTE };
enum custom_keycodes { CUS_TSK = SAFE_RANGE, CUS_SNT, CUS_SLK, CUS_CODE, SFT_OS };

// simple layers, no tri-layer
#define MIRROR MO(_MIRROR)
#define NAVSYM  MO(_NAVSYM)
#define BASE TO(_BASE)

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

#define CTL_GRV LCTL_T(KC_GRV)
#define CTL_MIN LCTL_T(KC_MINS)

#define ALT_5   LALT_T(KC_5)
#define ALT_6   LALT_T(KC_6)

#define GUI_ESC LGUI_T(KC_ESC)
#define GUI_ENT LGUI_T(KC_ENT)


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
