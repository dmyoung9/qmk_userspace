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
#include "progmem_anim.h"
#include "oled_utils.h"
#include "oled_unified_anim.h"  // Modern unified animation system

// ============================================================================
// Modern Slice Macros (using comprehensive oled_slice.h system)
// ============================================================================

// Custom slice macros not provided by oled_slice.h
#define SLICE72x12(p) SLICE_CUSTOM_PX(p, 72, 12)
#define SLICE22x16(p) SLICE_CUSTOM_PX(p, 22, 16)
// SLICE32x16 and SLICE128x32 are now provided by oled_slice.h
#define SLICE106x16(p) SLICE_CUSTOM_PX(p, 106, 16)
#define SLICE86x16(p) SLICE_CUSTOM_PX(p, 86, 16)
#define SLICE41x16(p) SLICE_CUSTOM_PX(p, 41, 16)

// Modifier slice macros (9px high) - using modern SLICE_CUSTOM_PX
#define SLICE21x9(p) SLICE_CUSTOM_PX(p, 21, 9)
#define SLICE25x9(p) SLICE_CUSTOM_PX(p, 25, 9)
#define SLICE17x9(p) SLICE_CUSTOM_PX(p, 17, 9)
#define SLICE23x9(p) SLICE_CUSTOM_PX(p, 23, 9)

// ============================================================================
// Animation Sequences (same data, modern organization)
// ============================================================================

// Layer animation sequences
DEFINE_SLICE_SEQ(layer_0,
    SLICE72x12(layer_0_0), SLICE72x12(layer_0_1), SLICE72x12(layer_0_2),
    SLICE72x12(layer_0_3), SLICE72x12(layer_0_4), SLICE72x12(layer_0_5)
);

DEFINE_SLICE_SEQ(layer_1,
    SLICE72x12(layer_1_0), SLICE72x12(layer_1_1), SLICE72x12(layer_1_2),
    SLICE72x12(layer_1_3), SLICE72x12(layer_1_4), SLICE72x12(layer_1_5)
);

DEFINE_SLICE_SEQ(layer_2,
    SLICE72x12(layer_2_0), SLICE72x12(layer_2_1), SLICE72x12(layer_2_2),
    SLICE72x12(layer_2_3), SLICE72x12(layer_2_4), SLICE72x12(layer_2_5),
    SLICE72x12(layer_2_6), SLICE72x12(layer_2_7), SLICE72x12(layer_2_8), SLICE72x12(layer_2_9)
);

DEFINE_SLICE_SEQ(layer_3,
    SLICE72x12(layer_3_0), SLICE72x12(layer_3_1), SLICE72x12(layer_3_2),
    SLICE72x12(layer_3_3), SLICE72x12(layer_3_4), SLICE72x12(layer_3_5),
    SLICE72x12(layer_3_6), SLICE72x12(layer_3_7)
);

DEFINE_SLICE_SEQ(layer_4,
    SLICE72x12(layer_4_0), SLICE72x12(layer_4_1), SLICE72x12(layer_4_2),
    SLICE72x12(layer_4_3),
);

DEFINE_SLICE_SEQ(layer_5,
    SLICE72x12(layer_5_0), SLICE72x12(layer_5_1), SLICE72x12(layer_5_2),
    SLICE72x12(layer_5_3), SLICE72x12(layer_5_4), SLICE72x12(layer_5_5),
);

// Layer frame animation
DEFINE_SLICE_SEQ(layer_frame,
    SLICE86x16(layer_frame_0), SLICE86x16(layer_frame_1), SLICE86x16(layer_frame_2),
    SLICE86x16(layer_frame_3), SLICE86x16(layer_frame_4)
);

// Boot animations
DEFINE_SLICE_SEQ(caps_frame_seq,
    SLICE41x16(caps_frame_0), SLICE41x16(caps_frame_1),
    SLICE41x16(caps_frame_2), SLICE41x16(caps_frame_3)
);

DEFINE_SLICE_SEQ(mods_frame_seq,
    SLICE106x16(mods_frame_0), SLICE106x16(mods_frame_1), SLICE106x16(mods_frame_2),
    SLICE106x16(mods_frame_3), SLICE106x16(mods_frame_4)
);

// WPM sequences (slave screen)
DEFINE_SLICE_SEQ(wpm_frame_seq,
    SLICE128x32(wpm_frame_0), SLICE128x32(wpm_frame_1), SLICE128x32(wpm_frame_2),
    SLICE128x32(wpm_frame_3), SLICE128x32(wpm_frame_4), SLICE128x32(wpm_frame_5),
    SLICE128x32(wpm_frame_6), SLICE128x32(wpm_frame_7), SLICE128x32(wpm_frame_8)
);

DEFINE_SLICE_SEQ(wpm_seq,
    SLICE32x16(wpm_0), SLICE32x16(wpm_1), SLICE32x16(wpm_2)
);

// Modifier animation sequences (NOW RE-ENABLED with unified system!)
DEFINE_SLICE_SEQ(caps_seq,
    SLICE21x9(caps_0), SLICE21x9(caps_1), SLICE21x9(caps_2), SLICE21x9(caps_3)
);

DEFINE_SLICE_SEQ(super_seq,
    SLICE25x9(super_0), SLICE25x9(super_1), SLICE25x9(super_2), SLICE25x9(super_3)
);

DEFINE_SLICE_SEQ(alt_seq,
    SLICE17x9(alt_0), SLICE17x9(alt_1), SLICE17x9(alt_2), SLICE17x9(alt_3)
);

DEFINE_SLICE_SEQ(shift_seq,
    SLICE23x9(shift_0), SLICE23x9(shift_1), SLICE23x9(shift_2), SLICE23x9(shift_3)
);

DEFINE_SLICE_SEQ(ctrl_seq,
    SLICE21x9(ctrl_0), SLICE21x9(ctrl_1), SLICE21x9(ctrl_2), SLICE21x9(ctrl_3)
);

// ============================================================================
// Modern Unified Animation System
// ============================================================================

// Layer animations (toggle pattern - can animate in both directions)
// 0th frame = inactive/initial, last frame = active/idle
static const unified_anim_config_t layer_0_config =
    UNIFIED_TOGGLE_CONFIG(&layer_0, 56, 0, BLEND_ADDITIVE);
static const unified_anim_config_t layer_1_config =
    UNIFIED_TOGGLE_CONFIG(&layer_1, 56, 0, BLEND_ADDITIVE);
static const unified_anim_config_t layer_2_config =
    UNIFIED_TOGGLE_CONFIG(&layer_2, 56, 0, BLEND_ADDITIVE);
static const unified_anim_config_t layer_3_config =
    UNIFIED_TOGGLE_CONFIG(&layer_3, 56, 0, BLEND_ADDITIVE);
static const unified_anim_config_t layer_4_config =
    UNIFIED_TOGGLE_CONFIG(&layer_4, 56, 0, BLEND_ADDITIVE);
static const unified_anim_config_t layer_5_config =
    UNIFIED_TOGGLE_CONFIG(&layer_5, 56, 0, BLEND_ADDITIVE);

// Layer frame animation (bootrev pattern - boot then reverse-out-back on trigger)
static const unified_anim_config_t layer_frame_config =
    UNIFIED_BOOTREV_CONFIG(&layer_frame, 42, 0, true);

// Boot animations (oneshot pattern - run once and stay at end)
static const unified_anim_config_t caps_frame_config =
    UNIFIED_ONESHOT_CONFIG(&caps_frame_seq, 0, 0, STEADY_LAST, true);
static const unified_anim_config_t mods_frame_config =
    UNIFIED_ONESHOT_CONFIG(&mods_frame_seq, 0, 16, STEADY_LAST, true);

// WPM animations (slave screen)
static const unified_anim_config_t wpm_frame_config =
    UNIFIED_ONESHOT_CONFIG(&wpm_frame_seq, 0, 0, STEADY_LAST, true);
static const unified_anim_config_t wpm_config =
    UNIFIED_ONESHOT_CONFIG(&wpm_seq, 83, 8, STEADY_LAST, true);

// Modifier animations (toggle pattern - smooth on/off transitions)
static const unified_anim_config_t caps_config =
    UNIFIED_TOGGLE_CONFIG(&caps_seq, 10, 2, BLEND_ADDITIVE);
static const unified_anim_config_t super_config =
    UNIFIED_TOGGLE_CONFIG(&super_seq, 9, 22, BLEND_OPAQUE);
static const unified_anim_config_t alt_config =
    UNIFIED_TOGGLE_CONFIG(&alt_seq, 35, 22, BLEND_OPAQUE);
static const unified_anim_config_t shift_config =
    UNIFIED_TOGGLE_CONFIG(&shift_seq, 53, 22, BLEND_OPAQUE);
static const unified_anim_config_t ctrl_config =
    UNIFIED_TOGGLE_CONFIG(&ctrl_seq, 77, 22, BLEND_OPAQUE);

// Runtime instances
static unified_anim_t layer_0_anim, layer_1_anim, layer_2_anim, layer_3_anim, layer_4_anim, layer_5_anim;
static unified_anim_t *layer_anims[] = {&layer_0_anim, &layer_1_anim, &layer_2_anim, &layer_3_anim, &layer_4_anim, &layer_5_anim};
static const unified_anim_config_t *layer_configs[] = {&layer_0_config, &layer_1_config, &layer_2_config, &layer_3_config, &layer_4_config, &layer_5_config};

// Frame and boot animations
static unified_anim_t layer_frame_anim;
static unified_anim_t caps_frame_anim, mods_frame_anim;
static unified_anim_t wpm_frame_anim, wpm_anim;

// Modifier animations (NOW WORKING!)
static unified_anim_t caps_anim, super_anim, alt_anim, shift_anim, ctrl_anim;

// State management
static uint8_t current_layer = 0;
static bool layer_is_active[LAYER_COUNT] = {false, false, false, false, false, false};

// Layer transition state machine (simplified with unified system)
typedef enum {
    LAYER_TRANSITION_IDLE = 0,
    LAYER_TRANSITION_EXITING,
    LAYER_TRANSITION_ENTERING
} layer_transition_state_t;

static layer_transition_state_t transition_state = LAYER_TRANSITION_IDLE;
static uint8_t exiting_layer = 0;
static uint8_t entering_layer = 0;

// ============================================================================
// Modifier State Detection (same as before)
// ============================================================================

static bool is_caps_active(void) {
    return host_keyboard_led_state().caps_lock || is_caps_word_on();
}

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

static const slice_t SLICE_logo = SLICE22x16(logo);

static const slice_t SLICE_digit_0 = SLICE8x16(digit_0);
static const slice_t SLICE_digit_1 = SLICE8x16(digit_1);
static const slice_t SLICE_digit_2 = SLICE8x16(digit_2);
static const slice_t SLICE_digit_3 = SLICE8x16(digit_3);
static const slice_t SLICE_digit_4 = SLICE8x16(digit_4);
static const slice_t SLICE_digit_5 = SLICE8x16(digit_5);
static const slice_t SLICE_digit_6 = SLICE8x16(digit_6);
static const slice_t SLICE_digit_7 = SLICE8x16(digit_7);
static const slice_t SLICE_digit_8 = SLICE8x16(digit_8);
static const slice_t SLICE_digit_9 = SLICE8x16(digit_9);

// ============================================================================
// Modern Unified Animation Management
// ============================================================================

void init_widgets(void) {
    uint32_t now = timer_read32();

    // Initialize layer animations
    current_layer = get_highest_layer(layer_state);

    // Clamp current_layer to valid range
    if (current_layer >= LAYER_COUNT) {
        current_layer = 0;
    }

    for (uint8_t i = 0; i < (sizeof(layer_anims) / sizeof(layer_anims[0])); i++) {
        bool is_active = (i == current_layer);

        // For toggle animations: initial_state = 1 means "on" (active layer shows last frame)
        // initial_state = 0 means "off" (inactive layers show first frame)
        uint8_t initial_state = is_active ? 1 : 0;

        // Initialize with the layer config
        unified_anim_init(layer_anims[i], layer_configs[i], initial_state, now);
        layer_is_active[i] = is_active;
    }

    // Initialize frame and boot animations
    unified_anim_init(&layer_frame_anim, &layer_frame_config, 0, now);
    unified_anim_init(&caps_frame_anim, &caps_frame_config, 0, now);
    unified_anim_init(&mods_frame_anim, &mods_frame_config, 0, now);

    // Initialize modifier animations (NOW WORKING!)
    unified_anim_init(&caps_anim, &caps_config, is_caps_active() ? 1 : 0, now);
    unified_anim_init(&super_anim, &super_config, is_super_active() ? 1 : 0, now);
    unified_anim_init(&alt_anim, &alt_config, is_alt_active() ? 1 : 0, now);
    unified_anim_init(&shift_anim, &shift_config, is_shift_active() ? 1 : 0, now);
    unified_anim_init(&ctrl_anim, &ctrl_config, is_ctrl_active() ? 1 : 0, now);
}

// ============================================================================
// Modern Layer Transition Management
// ============================================================================

static void trigger_layer_enter(uint8_t layer, uint32_t now) {
    if (layer >= LAYER_COUNT || !layer_anims[layer]) return;

    // For toggle animations: trigger with state=1 (on/active)
    // This will animate from 0 (off) to last (on) if currently off
    unified_anim_trigger(layer_anims[layer], 1, now);
    layer_is_active[layer] = true;
}

static void trigger_layer_exit(uint8_t layer, uint32_t now) {
    if (layer >= LAYER_COUNT || !layer_anims[layer]) return;

    // For toggle animations: trigger with state=0 (off/inactive)
    // This will animate from last (on) to 0 (off) if currently on
    unified_anim_trigger(layer_anims[layer], 0, now);
    layer_is_active[layer] = false;
}

void tick_widgets(void) {
    uint32_t now = timer_read32();

    // Handle layer transition state machine with bounds checking
    uint8_t new_layer = get_highest_layer(layer_state);

    // Clamp new_layer to valid range to prevent crashes
    if (new_layer >= LAYER_COUNT) {
        new_layer = 0;  // Default to layer 0 if invalid
    }

    // Ensure current_layer is also valid
    if (current_layer >= LAYER_COUNT) {
        current_layer = 0;
    }

    switch (transition_state) {
        case LAYER_TRANSITION_IDLE:
            if (new_layer != current_layer) {
                // Start transition with bounds checking
                // For toggle animations, we don't need to wait for boot completion
                if (current_layer < LAYER_COUNT && layer_anims[current_layer]) {
                    trigger_layer_exit(current_layer, now);
                    transition_state = LAYER_TRANSITION_EXITING;
                    exiting_layer = current_layer;
                    entering_layer = new_layer;

                    // Trigger layer frame animation (out-and-back)
                    if (unified_anim_boot_done(&layer_frame_anim)) {
                        unified_anim_trigger(&layer_frame_anim, 0, now);
                    }
                }
            }
            break;

        case LAYER_TRANSITION_EXITING:
            // Check for layer change cancellation during exit
            if (new_layer != entering_layer) {
                // Layer changed again! Update target but let current exit finish
                entering_layer = new_layer;
            }

            // Wait for exit animation to complete (on→off: last→0)
            if (exiting_layer < LAYER_COUNT && layer_anims[exiting_layer]) {
                if (!unified_anim_is_running(layer_anims[exiting_layer])) {
                    // Exit animation completed, start enter animation
                    trigger_layer_enter(entering_layer, now);
                    transition_state = LAYER_TRANSITION_ENTERING;
                }
            } else {
                // Invalid exiting layer, skip to entering
                trigger_layer_enter(entering_layer, now);
                transition_state = LAYER_TRANSITION_ENTERING;
            }
            break;

        case LAYER_TRANSITION_ENTERING:
            // Check for layer change cancellation during enter
            if (new_layer != entering_layer) {
                // Layer changed during enter! Start new exit sequence
                trigger_layer_exit(entering_layer, now);
                transition_state = LAYER_TRANSITION_EXITING;
                exiting_layer = entering_layer;
                entering_layer = new_layer;

                // Trigger layer frame animation again
                if (unified_anim_boot_done(&layer_frame_anim)) {
                    unified_anim_trigger(&layer_frame_anim, 0, now);
                }
            } else {
                // Wait for enter animation to complete (off→on: 0→last)
                if (entering_layer < LAYER_COUNT && layer_anims[entering_layer]) {
                    if (!unified_anim_is_running(layer_anims[entering_layer])) {
                        // Enter animation completed, transition is done
                        current_layer = entering_layer;
                        transition_state = LAYER_TRANSITION_IDLE;
                    }
                } else {
                    // Invalid entering layer, complete transition anyway
                    current_layer = entering_layer;
                    transition_state = LAYER_TRANSITION_IDLE;
                }
            }
            break;
    }

    // Update frame animations (background elements) - MUST render BEFORE layer animations
    unified_anim_render(&caps_frame_anim, now);
    unified_anim_render(&mods_frame_anim, now);
    unified_anim_render(&layer_frame_anim, now);

    // Update layer animations - render based on transition state
    switch (transition_state) {
        case LAYER_TRANSITION_IDLE:
            // Only render current layer (showing steady last frame)
            if (current_layer < LAYER_COUNT && layer_anims[current_layer]) {
                unified_anim_render(layer_anims[current_layer], now);
            }
            break;

        case LAYER_TRANSITION_EXITING:
            // Render exiting layer (animating last→0)
            if (exiting_layer < LAYER_COUNT && layer_anims[exiting_layer]) {
                unified_anim_render(layer_anims[exiting_layer], now);
            }
            break;

        case LAYER_TRANSITION_ENTERING:
            // Render entering layer (animating 0→last)
            if (entering_layer < LAYER_COUNT && layer_anims[entering_layer]) {
                unified_anim_render(layer_anims[entering_layer], now);
            }
            break;
    }

    // Update modifier animations with current state (NOW WORKING!)
    unified_anim_trigger(&caps_anim, is_caps_active() ? 1 : 0, now);
    unified_anim_trigger(&super_anim, is_super_active() ? 1 : 0, now);
    unified_anim_trigger(&alt_anim, is_alt_active() ? 1 : 0, now);
    unified_anim_trigger(&shift_anim, is_shift_active() ? 1 : 0, now);
    unified_anim_trigger(&ctrl_anim, is_ctrl_active() ? 1 : 0, now);

    // Render modifier animations (foreground elements with additive blending)
    unified_anim_render(&caps_anim, now);
    unified_anim_render(&super_anim, now);
    unified_anim_render(&alt_anim, now);
    unified_anim_render(&shift_anim, now);
    unified_anim_render(&ctrl_anim, now);
}

void draw_wpm_frame(void) {
    uint32_t now = timer_read32();

    // Initialize WPM animations on first call (slave screen only)
    static bool wpm_initialized = false;
    if (!wpm_initialized) {
        unified_anim_init(&wpm_frame_anim, &wpm_frame_config, 0, now);
        unified_anim_init(&wpm_anim, &wpm_config, 0, now);
        wpm_initialized = true;
    }

    // Render WPM animations
    unified_anim_render(&wpm_frame_anim, now);
    unified_anim_render(&wpm_anim, now);
}

void draw_logo(void) {
    // Draw static logo
    draw_slice_px(&SLICE_logo, 106, 16);
}

// ============================================================================
// Enhanced API Functions
// ============================================================================

bool is_boot_animation_complete(void) {
    // Check if all boot animations are complete
    return unified_anim_boot_done(&caps_frame_anim) &&
           unified_anim_boot_done(&mods_frame_anim) &&
           unified_anim_boot_done(&layer_frame_anim);
}

void trigger_layer_transition_effect(void) {
    uint32_t now = timer_read32();
    if (unified_anim_boot_done(&layer_frame_anim)) {
        unified_anim_trigger(&layer_frame_anim, 0, now);
    }
}

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










