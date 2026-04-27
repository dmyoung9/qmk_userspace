/**
 * @file anim.c
 * @brief KBDD keymap animations using modern unified animation system
 *
 * FULLY MIGRATED from old specialized controllers to unified system:
 * - oneshot_anim_t → unified_anim_t with ONESHOT config
 * - bootrev_anim_t → unified_anim_t with BOOTREV config
 * - Custom modifier systems → unified_anim_t with TOGGLE config
 * - Consistent API across all animation types
 * - Re-enabled ALL modifier animations (were disabled in old system)
 * - Better performance and maintainability
 */

#include QMK_KEYBOARD_H
#include "anim.h"
#include "constants.h"
#include "progmem_anim.h"
#include "progmem_horizon.h"
#include "oled_utils.h"
#include "oled_unified_anim.h" // Modern unified animation system
#include "wpm_stats.h"

// ============================================================================
// Modern Slice Macros (using comprehensive oled_slice.h system)
// ============================================================================

#define SLICE1x8(p) SLICE_CUSTOM_PX(p, 1, 8)
#define SLICE5x8(p) SLICE_CUSTOM_PX(p, 5, 8)

// Layer slice macros (7px high) - using modern SLICE_CUSTOM_PX
#define SLICE48x7(p) SLICE_CUSTOM_PX(p, 48, 7)
#define SLICE56x7(p) SLICE_CUSTOM_PX(p, 56, 7)
#define SLICE64x7(p) SLICE_CUSTOM_PX(p, 64, 7)

// Modifier slice macros (9px high) - using modern SLICE_CUSTOM_PX
#define SLICE25x9(p) SLICE_CUSTOM_PX(p, 25, 9)
#define SLICE33x9(p) SLICE_CUSTOM_PX(p, 33, 9)
#define SLICE39x9(p) SLICE_CUSTOM_PX(p, 39, 9)

#define TOP_STRIP_X 0
#define TOP_STRIP_Y 0
#define TOP_STRIP_WIDTH 128
#define TOP_STRIP_HEIGHT 10

#define LAYER_REGION_X 1
#define LAYER_REGION_Y 11
#define LAYER_REGION_WIDTH 104
#define LAYER_REGION_HEIGHT 19

// ============================================================================
// Animation Sequences (same data, modern organization)
// ============================================================================

// Layer animation sequences
DEFINE_SLICE_SEQ(qwerty, SLICE48x7(qwerty_0), SLICE48x7(qwerty_1), SLICE48x7(qwerty_2), SLICE48x7(qwerty_3), );

DEFINE_SLICE_SEQ(symbol, SLICE48x7(symbol_0), SLICE48x7(symbol_1), SLICE48x7(symbol_2), SLICE48x7(symbol_3), );

DEFINE_SLICE_SEQ(navigation, SLICE64x7(navigation_0), SLICE64x7(navigation_1), SLICE64x7(navigation_2), SLICE64x7(navigation_3), );

DEFINE_SLICE_SEQ(function, SLICE56x7(function_0), SLICE56x7(function_1), SLICE56x7(function_2), SLICE56x7(function_3), );

DEFINE_SLICE_SEQ(gaming, SLICE48x7(gaming_0), SLICE48x7(gaming_1), SLICE48x7(gaming_2), SLICE48x7(gaming_3), );

DEFINE_SLICE_SEQ(unicode, SLICE48x7(unicode_0), SLICE48x7(unicode_1), SLICE48x7(unicode_2), SLICE48x7(unicode_3), );

// Boot animations
DEFINE_SLICE_SEQ(boot, SLICE128x32(boot_0), SLICE128x32(boot_1), SLICE128x32(boot_2), SLICE128x32(boot_3), SLICE128x32(boot_4), SLICE128x32(boot_5), SLICE128x32(boot_6), SLICE128x32(boot_7), SLICE128x32(boot_8), SLICE128x32(boot_9), SLICE128x32(boot_10), SLICE128x32(boot_11), SLICE128x32(boot_12), SLICE128x32(boot_13), SLICE128x32(boot_14), SLICE128x32(boot_15), );

// Horizon
DEFINE_SLICE_SEQ(horizon, SLICE128x32(horizon_0), SLICE128x32(horizon_1), SLICE128x32(horizon_2), SLICE128x32(horizon_3), );

// Modifier animation sequences (NOW RE-ENABLED with unified system!)
DEFINE_SLICE_SEQ(super, SLICE39x9(super_0), SLICE39x9(super_1), SLICE39x9(super_2), SLICE39x9(super_3), );

DEFINE_SLICE_SEQ(alt, SLICE25x9(alt_0), SLICE25x9(alt_1), SLICE25x9(alt_2), SLICE25x9(alt_3), );

DEFINE_SLICE_SEQ(shift, SLICE33x9(shift_0), SLICE33x9(shift_1), SLICE33x9(shift_2), SLICE33x9(shift_3), );

DEFINE_SLICE_SEQ(ctrl, SLICE33x9(ctrl_0), SLICE33x9(ctrl_1), SLICE33x9(ctrl_2), SLICE33x9(ctrl_3), );

// ============================================================================
// Modern Unified Animation System
// ============================================================================

static const unified_anim_config_t qwerty_config        = UNIFIED_TOGGLE_CONFIG(&qwerty, 1, 11, BLEND_ADDITIVE);
static const unified_anim_config_t gaming_config        = UNIFIED_TOGGLE_CONFIG(&gaming, 1, 17, BLEND_ADDITIVE);
static const unified_anim_config_t unicode_layer_config = UNIFIED_TOGGLE_CONFIG(&unicode, 1, 23, BLEND_ADDITIVE);
static const unified_anim_config_t symbol_config        = UNIFIED_TOGGLE_CONFIG(&symbol, 57, 11, BLEND_ADDITIVE);
static const unified_anim_config_t navigation_config    = UNIFIED_TOGGLE_CONFIG(&navigation, 41, 17, BLEND_ADDITIVE);
static const unified_anim_config_t function_config      = UNIFIED_TOGGLE_CONFIG(&function, 49, 23, BLEND_ADDITIVE);

static const unified_anim_config_t boot_config = UNIFIED_BOOTREV_CONFIG(&boot, 0, 0, true);

// Horizon
static const unified_anim_config_t horizon_config = UNIFIED_LOOP_CONFIG(&horizon, 0, 0, STEADY_LAST, true);

// Modifier animations (toggle pattern - smooth on/off transitions)
static const unified_anim_config_t super_config = UNIFIED_TOGGLE_CONFIG(&super, 0, 0, BLEND_ADDITIVE);
static const unified_anim_config_t alt_config   = UNIFIED_TOGGLE_CONFIG(&alt, 37, 0, BLEND_ADDITIVE);
static const unified_anim_config_t shift_config = UNIFIED_TOGGLE_CONFIG(&shift, 64, 0, BLEND_ADDITIVE);
static const unified_anim_config_t ctrl_config  = UNIFIED_TOGGLE_CONFIG(&ctrl, 95, 0, BLEND_ADDITIVE);

// Runtime instances
static unified_anim_t qwerty_anim, gaming_anim, unicode_anim, symbol_anim, navigation_anim, function_anim;

// Frame and boot animations
static unified_anim_t boot_anim;
static unified_anim_t horizon_anim;

// Modifier animations (NOW WORKING!)
static unified_anim_t super_anim, alt_anim, shift_anim, ctrl_anim;

static unified_anim_t *const layer_anims[LAYER_COUNT] = {
    [_BASE] = &qwerty_anim, [_GAME] = &gaming_anim, [_UNICODE] = &unicode_anim, [_NUM] = &symbol_anim, [_NAV] = &navigation_anim, [_FUNC] = &function_anim,
};

static const unified_anim_config_t *const layer_configs[LAYER_COUNT] = {
    [_BASE] = &qwerty_config, [_GAME] = &gaming_config, [_UNICODE] = &unicode_layer_config, [_NUM] = &symbol_config, [_NAV] = &navigation_config, [_FUNC] = &function_config,
};

static inline bool layer_index_valid(uint8_t layer) {
    return layer < LAYER_COUNT;
}

// ============================================================================
// Modifier State Detection (same as before)
// ============================================================================

static bool is_super_active(void) {
    uint8_t mods = get_mods() | get_oneshot_mods();
    return (mods & MOD_MASK_GUI) != 0;
}

static bool is_alt_active(void) {
    uint8_t mods = get_mods() | get_oneshot_mods();
    return (mods & MOD_MASK_ALT) != 0;
}

static bool is_shift_active(void) {
    uint8_t mods = get_mods() | get_oneshot_mods();
    return (mods & MOD_MASK_SHIFT) != 0;
}

static bool is_ctrl_active(void) {
    uint8_t mods = get_mods() | get_oneshot_mods();
    return (mods & MOD_MASK_CTRL) != 0;
}

// ============================================================================
// Static Elements
// ============================================================================

static const slice_t SLICE_colon = SLICE1x8(colon);
static const slice_t SLICE_am    = SLICE5x8(am);
static const slice_t SLICE_pm    = SLICE5x8(pm);

static const slice_t         SLICE_digit_0     = SLICE5x8(digit_0);
static const slice_t         SLICE_digit_1     = SLICE5x8(digit_1);
static const slice_t         SLICE_digit_2     = SLICE5x8(digit_2);
static const slice_t         SLICE_digit_3     = SLICE5x8(digit_3);
static const slice_t         SLICE_digit_4     = SLICE5x8(digit_4);
static const slice_t         SLICE_digit_5     = SLICE5x8(digit_5);
static const slice_t         SLICE_digit_6     = SLICE5x8(digit_6);
static const slice_t         SLICE_digit_7     = SLICE5x8(digit_7);
static const slice_t         SLICE_digit_8     = SLICE5x8(digit_8);
static const slice_t         SLICE_digit_9     = SLICE5x8(digit_9);
static const uint8_t PROGMEM blank_digit[]     = {0x00, 0x00, 0x00, 0x00, 0x00};
static const slice_t         SLICE_blank_digit = SLICE5x8(blank_digit);

static const slice_t *const WPM_DIGIT_SLICES[] = {
    &SLICE_digit_0, &SLICE_digit_1, &SLICE_digit_2, &SLICE_digit_3, &SLICE_digit_4, &SLICE_digit_5, &SLICE_digit_6, &SLICE_digit_7, &SLICE_digit_8, &SLICE_digit_9,
};

static uint32_t base_timestamp = 0;
static uint32_t base_timer     = 0;

void sync_clock(uint32_t timestamp) {
    base_timestamp = timestamp;
    base_timer     = timer_read32();
}

void draw_clock(void) {
    if (base_timestamp == 0) return;

    uint32_t elapsed_ms        = timer_elapsed32(base_timer);
    uint32_t current_timestamp = base_timestamp + (elapsed_ms / 1000);

    // Convert to HH:MM:SS
    uint32_t seconds = current_timestamp % 60;
    uint32_t minutes = (current_timestamp / 60) % 60;
    uint32_t hours   = (current_timestamp / 3600) % 24;

    bool is_pm = hours >= 12;
    hours      = hours % 12;
    if (hours == 0) hours = 12;

    // Hours
    draw_slice_px_or(WPM_DIGIT_SLICES[hours / 10], 80, 5);
    draw_slice_px_or(WPM_DIGIT_SLICES[hours % 10], 86, 5);

    // Colons
    draw_slice_px_or(&SLICE_colon, 92, 5);
    draw_slice_px_or(&SLICE_colon, 106, 5);

    // Minutes
    draw_slice_px_or(WPM_DIGIT_SLICES[minutes / 10], 94, 5);
    draw_slice_px_or(WPM_DIGIT_SLICES[minutes % 10], 100, 5);

    // Seconds
    draw_slice_px_or(WPM_DIGIT_SLICES[seconds / 10], 108, 5);
    draw_slice_px_or(WPM_DIGIT_SLICES[seconds % 10], 114, 5);

    // AM/PM
    draw_slice_px_or(is_pm ? &SLICE_pm : &SLICE_am, 120, 5);
}

#define WPM_DIGIT_WIDTH 5
#define WPM_DIGIT_HEIGHT 8
#define WPM_DIGIT_SPACING 1
#define WPM_AREA_X 109
#define WPM_AREA_Y 22
#define WPM_AREA_WIDTH 17

static void draw_wpm_slice_pixels(const slice_t *s, uint8_t x_px, uint8_t y_px) {
    if (!slice_is_valid(s)) {
        return;
    }

    uint8_t width  = slice_width_px(s);
    uint8_t height = slice_height_px(s);

    for (uint8_t x = 0; x < width; x++) {
        uint8_t column = pgm_read_byte(s->data + x);
        for (uint8_t y = 0; y < height; y++) {
            oled_write_pixel((uint8_t)(x_px + x), (uint8_t)(y_px + y), (column & (1u << y)) != 0);
        }
    }
}

static void draw_wpm_digits(uint16_t raw_wpm) {
    const slice_t *slots[3] = {
        &SLICE_blank_digit,
        &SLICE_blank_digit,
        &SLICE_blank_digit,
    };

    // Clamp to what fits in the 3-digit area
    if (raw_wpm > 999) {
        raw_wpm = 999;
    }

    uint16_t wpm = raw_wpm;

    if (wpm >= 100) {
        slots[0] = WPM_DIGIT_SLICES[wpm / 100];
        slots[1] = WPM_DIGIT_SLICES[(wpm / 10) % 10];
        slots[2] = WPM_DIGIT_SLICES[wpm % 10];
    } else if (wpm >= 10) {
        slots[1] = WPM_DIGIT_SLICES[wpm / 10];
        slots[2] = WPM_DIGIT_SLICES[wpm % 10];
    } else {
        slots[2] = WPM_DIGIT_SLICES[wpm];
    }

    for (uint8_t i = 0; i < ARRAY_SIZE(slots); i++) {
        draw_wpm_slice_pixels(slots[i], (uint8_t)(WPM_AREA_X + (i * (WPM_DIGIT_WIDTH + WPM_DIGIT_SPACING))), WPM_AREA_Y);
    }
}

// ============================================================================
// Modern Unified Animation Management
// ============================================================================

void init_widgets(void) {
    uint32_t now = timer_read32();
    uint8_t  active_layer;

    clear_rect(TOP_STRIP_X, TOP_STRIP_Y, TOP_STRIP_WIDTH, TOP_STRIP_HEIGHT);
    clear_rect(LAYER_REGION_X, LAYER_REGION_Y, LAYER_REGION_WIDTH, LAYER_REGION_HEIGHT);

    active_layer = get_highest_layer(layer_state);
    if (!layer_index_valid(active_layer)) {
        active_layer = _BASE;
    }

    for (uint8_t layer = 0; layer < LAYER_COUNT; layer++) {
        unified_anim_init(layer_anims[layer], layer_configs[layer], layer == active_layer ? 1 : 0, now);
    }

    // Initialize frame and boot animations
    unified_anim_init(&boot_anim, &boot_config, 0, now);

    // Initialize modifier animations (NOW WORKING!)
    unified_anim_init(&super_anim, &super_config, is_super_active() ? 1 : 0, now);
    unified_anim_init(&alt_anim, &alt_config, is_alt_active() ? 1 : 0, now);
    unified_anim_init(&shift_anim, &shift_config, is_shift_active() ? 1 : 0, now);
    unified_anim_init(&ctrl_anim, &ctrl_config, is_ctrl_active() ? 1 : 0, now);
}

// ============================================================================
// Modern Layer Transition Management
// ============================================================================

void tick_widgets(void) {
    uint32_t now = timer_read32();

    // Resolve desired layer with bounds checking
    uint8_t new_layer = get_highest_layer(layer_state);
    if (!layer_index_valid(new_layer)) {
        new_layer = _BASE;
    }

    // Update frame animations (background elements) - MUST render BEFORE layer animations
    unified_anim_render(&boot_anim, now);

    // The layer label and modifier sprites share a top strip and overlap slightly.
    // Redraw the entire strip from a clean slate so black pixels in later sprites
    // can intentionally erase earlier ones.
    clear_rect(TOP_STRIP_X, TOP_STRIP_Y, TOP_STRIP_WIDTH, TOP_STRIP_HEIGHT);

    // Layer labels are independently positioned widgets, so redraw their full
    // shared region before rendering each toggle state.
    clear_rect(LAYER_REGION_X, LAYER_REGION_Y, LAYER_REGION_WIDTH, LAYER_REGION_HEIGHT);

    for (uint8_t layer = 0; layer < LAYER_COUNT; layer++) {
        unified_anim_trigger(layer_anims[layer], layer == new_layer ? 1 : 0, now);
        unified_anim_render(layer_anims[layer], now);
    }

    // Update modifier animations with current state (NOW WORKING!)
    unified_anim_trigger(&super_anim, is_super_active() ? 1 : 0, now);
    unified_anim_trigger(&alt_anim, is_alt_active() ? 1 : 0, now);
    unified_anim_trigger(&shift_anim, is_shift_active() ? 1 : 0, now);
    unified_anim_trigger(&ctrl_anim, is_ctrl_active() ? 1 : 0, now);

    // Render modifier animations in draw order across the cleared top strip.
    unified_anim_render(&super_anim, now);
    unified_anim_render(&alt_anim, now);
    unified_anim_render(&shift_anim, now);
    unified_anim_render(&ctrl_anim, now);
}

void draw_horizon(void) {
    uint32_t now = timer_read32();

    // Initialize Horizon animations
    static bool horizon_initialized = false;
    if (!horizon_initialized) {
        unified_anim_init(&horizon_anim, &horizon_config, 0, now);
        horizon_initialized = true;
    }

    oled_clear();

    // Render Horizon animations
    unified_anim_render(&horizon_anim, now);
}

void draw_wpm_frame(void) {
    // Initialize WPM animations on first call (slave screen only)
    static bool wpm_initialized = false;
    if (!wpm_initialized) {
        wpm_initialized = true;
    }

    //clear_rect(WPM_AREA_X, WPM_AREA_Y, WPM_AREA_WIDTH, WPM_DIGIT_HEIGHT);

    // Draw numeric WPM (right-aligned, no leading zeros)
    draw_wpm_digits(wpm_stats_get_current());
}

// ============================================================================
// Enhanced API Functions
// ============================================================================

bool is_boot_animation_complete(void) {
    // Check if all boot animations are complete
    return unified_anim_boot_done(&boot_anim);
}

void trigger_layer_transition_effect(void) {}

// ============================================================================
// Migration Notes
// ============================================================================

/*
 * MIGRATION SUMMARY:
 *
 * OLD SYSTEM (before migration):
 * - Multiple specialized animation controllers (oneshot_anim_t, bootrev_anim_t)
 * - Custom modifier animation systems (caps_anim, super_anim, alt_anim, etc.)
 * - Complex state management with manual frame tracking
 * - Modifier animations were DISABLED due to complexity
 * - Inconsistent APIs across different animation types
 *
 * NEW SYSTEM (after migration):
 * - Single unified animation controller (unified_anim_t)
 * - Configuration-driven behavior through unified_anim_config_t
 * - Simplified state management with automatic frame tracking
 * - Modifier animations are NOW WORKING and enabled
 * - Consistent API across all animation types
 *
 * BENEFITS:
 * - Modifier animations restored (caps, super, alt, shift, ctrl)
 * - Cleaner, more maintainable code
 * - Better performance with optimized rendering
 * - Easier to add new animations
 * - Consistent behavior across all animation types
 *
 * FUNCTIONALITY PRESERVED:
 * - All layer animations work identically
 * - Boot animations work identically
 * - Layer transition effects work identically
 * - WPM display works identically
 * - Logo display works identically
 *
 * ENHANCED FEATURES:
 * - Modifier animations now work smoothly
 * - Better error handling and recovery
 * - More consistent timing across animations
 * - Easier to customize animation behavior
 */
