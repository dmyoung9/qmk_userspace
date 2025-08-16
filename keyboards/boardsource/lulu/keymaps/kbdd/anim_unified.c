/**
 * @file anim_unified.c
 * @brief Migrated kbdd animations using the new unified animation system
 * 
 * This file demonstrates how to migrate from the old specialized animation
 * controllers to the new unified system while maintaining identical functionality.
 */

#include QMK_KEYBOARD_H
#include "anim.h"
#include "progmem_anim.h"
#include "oled_utils.h"
#include "oled_unified_anim.h"  // New unified animation system

#define LAYER_COUNT 4

// ============================================================================
// SLICE Macros (using new comprehensive system)
// ============================================================================

// Custom slice macros not provided by oled_slice.h
#define SLICE72x12(p) SLICE_CUSTOM_PX(p, 72, 12)
#define SLICE22x16(p) SLICE_CUSTOM_PX(p, 22, 16)
// SLICE32x16 and SLICE128x32 are now provided by oled_slice.h
#define SLICE106x16(p) SLICE_CUSTOM_PX(p, 106, 16)
#define SLICE86x16(p) SLICE_CUSTOM_PX(p, 86, 16)
#define SLICE41x16(p) SLICE_CUSTOM_PX(p, 41, 16)

// Modifier slice macros (9px high)
#define SLICE21x9(p) SLICE_CUSTOM_PX(p, 21, 9)
#define SLICE25x9(p) SLICE_CUSTOM_PX(p, 25, 9)
#define SLICE17x9(p) SLICE_CUSTOM_PX(p, 17, 9)
#define SLICE23x9(p) SLICE_CUSTOM_PX(p, 23, 9)

// ============================================================================
// Animation Sequences (same as before)
// ============================================================================

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

// Layer frame animation
DEFINE_SLICE_SEQ(layer_frame,
    SLICE86x16(layer_frame_0), SLICE86x16(layer_frame_1), SLICE86x16(layer_frame_2),
    SLICE86x16(layer_frame_3), SLICE86x16(layer_frame_4)
);

// Boot animation sequences
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

// Modifier sequences (re-enabled with unified system)
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
// NEW: Unified Animation Configurations
// ============================================================================

// Layer animations (oneshot pattern - boot then stay at end)
static const unified_anim_config_t layer_0_config = 
    UNIFIED_ONESHOT_CONFIG(&layer_0, 56, 0, STEADY_LAST, true);
static const unified_anim_config_t layer_1_config = 
    UNIFIED_ONESHOT_CONFIG(&layer_1, 56, 0, STEADY_LAST, false);
static const unified_anim_config_t layer_2_config = 
    UNIFIED_ONESHOT_CONFIG(&layer_2, 56, 0, STEADY_LAST, false);
static const unified_anim_config_t layer_3_config = 
    UNIFIED_ONESHOT_CONFIG(&layer_3, 56, 0, STEADY_LAST, false);

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
    UNIFIED_TOGGLE_CONFIG(&caps_seq, 10, 2, BLEND_OPAQUE);
static const unified_anim_config_t super_config = 
    UNIFIED_TOGGLE_CONFIG(&super_seq, 9, 22, BLEND_OPAQUE);
static const unified_anim_config_t alt_config = 
    UNIFIED_TOGGLE_CONFIG(&alt_seq, 35, 22, BLEND_OPAQUE);
static const unified_anim_config_t shift_config = 
    UNIFIED_TOGGLE_CONFIG(&shift_seq, 53, 22, BLEND_OPAQUE);
static const unified_anim_config_t ctrl_config = 
    UNIFIED_TOGGLE_CONFIG(&ctrl_seq, 77, 22, BLEND_OPAQUE);

// ============================================================================
// NEW: Unified Animation Instances
// ============================================================================

// Layer animations
static unified_anim_t layer_0_anim, layer_1_anim, layer_2_anim, layer_3_anim;
static unified_anim_t *layer_anims[] = {&layer_0_anim, &layer_1_anim, &layer_2_anim, &layer_3_anim};
static const unified_anim_config_t *layer_configs[] = {&layer_0_config, &layer_1_config, &layer_2_config, &layer_3_config};

// Frame and boot animations
static unified_anim_t layer_frame_anim;
static unified_anim_t caps_frame_anim, mods_frame_anim;
static unified_anim_t wpm_frame_anim, wpm_anim;

// Modifier animations (now re-enabled!)
static unified_anim_t caps_anim, super_anim, alt_anim, shift_anim, ctrl_anim;

// ============================================================================
// State Management
// ============================================================================

static uint8_t current_layer = 0;
static bool layer_is_active[LAYER_COUNT] = {false, false, false, false};

// Layer transition state machine
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

// ============================================================================
// NEW: Unified Animation Management
// ============================================================================

void init_widgets_unified(void) {
    uint32_t now = timer_read32();
    
    // Initialize layer animations
    current_layer = get_highest_layer(layer_state);
    
    for (uint8_t i = 0; i < LAYER_COUNT; i++) {
        bool is_active = (i == current_layer);
        bool run_boot = is_active;  // Only current layer runs boot animation
        
        // Create a modified config for this specific layer
        unified_anim_config_t layer_config = *layer_configs[i];
        layer_config.run_boot_anim = run_boot;
        
        unified_anim_init(layer_anims[i], &layer_config, 0, now);
        layer_is_active[i] = is_active;
    }
    
    // Initialize frame and boot animations
    unified_anim_init(&layer_frame_anim, &layer_frame_config, 0, now);
    unified_anim_init(&caps_frame_anim, &caps_frame_config, 0, now);
    unified_anim_init(&mods_frame_anim, &mods_frame_config, 0, now);
    
    // Initialize modifier animations (now working!)
    unified_anim_init(&caps_anim, &caps_config, is_caps_active() ? 1 : 0, now);
    unified_anim_init(&super_anim, &super_config, is_super_active() ? 1 : 0, now);
    unified_anim_init(&alt_anim, &alt_config, is_alt_active() ? 1 : 0, now);
    unified_anim_init(&shift_anim, &shift_config, is_shift_active() ? 1 : 0, now);
    unified_anim_init(&ctrl_anim, &ctrl_config, is_ctrl_active() ? 1 : 0, now);
}

// ============================================================================
// NEW: Layer Transition Management with Unified Controllers
// ============================================================================

static void trigger_layer_enter_unified(uint8_t layer, uint32_t now) {
    if (layer >= LAYER_COUNT) return;

    // Trigger the unified animation for this layer
    unified_anim_trigger(layer_anims[layer], 0, now);  // State ignored for oneshot
    layer_is_active[layer] = true;
}

static void trigger_layer_exit_unified(uint8_t layer, uint32_t now) {
    if (layer >= LAYER_COUNT) return;

    // For oneshot animations, we don't have a specific exit trigger
    // The animation will naturally return to steady state
    layer_is_active[layer] = false;
}

void tick_widgets_unified(void) {
    uint32_t now = timer_read32();

    // Handle layer transition state machine
    uint8_t new_layer = get_highest_layer(layer_state);

    switch (transition_state) {
        case LAYER_TRANSITION_IDLE:
            if (new_layer != current_layer) {
                // Start transition
                if (unified_anim_boot_done(layer_anims[current_layer])) {
                    trigger_layer_exit_unified(current_layer, now);
                    transition_state = LAYER_TRANSITION_EXITING;
                    exiting_layer = current_layer;
                    entering_layer = new_layer;

                    // Trigger layer frame animation
                    if (unified_anim_boot_done(&layer_frame_anim)) {
                        unified_anim_trigger(&layer_frame_anim, 0, now);
                    }
                }
            }
            break;

        case LAYER_TRANSITION_EXITING:
            // For unified oneshot animations, we can immediately start entering
            trigger_layer_enter_unified(entering_layer, now);
            transition_state = LAYER_TRANSITION_ENTERING;
            break;

        case LAYER_TRANSITION_ENTERING:
            // Check if entering animation is complete
            if (!unified_anim_is_running(layer_anims[entering_layer])) {
                current_layer = entering_layer;
                transition_state = LAYER_TRANSITION_IDLE;
            }
            break;
    }

    // Update all layer animations
    for (uint8_t i = 0; i < LAYER_COUNT; i++) {
        unified_anim_render(layer_anims[i], now);
    }

    // Update frame animations
    unified_anim_render(&layer_frame_anim, now);
    unified_anim_render(&caps_frame_anim, now);
    unified_anim_render(&mods_frame_anim, now);

    // Update modifier animations with current state
    unified_anim_trigger(&caps_anim, is_caps_active() ? 1 : 0, now);
    unified_anim_trigger(&super_anim, is_super_active() ? 1 : 0, now);
    unified_anim_trigger(&alt_anim, is_alt_active() ? 1 : 0, now);
    unified_anim_trigger(&shift_anim, is_shift_active() ? 1 : 0, now);
    unified_anim_trigger(&ctrl_anim, is_ctrl_active() ? 1 : 0, now);

    unified_anim_render(&caps_anim, now);
    unified_anim_render(&super_anim, now);
    unified_anim_render(&alt_anim, now);
    unified_anim_render(&shift_anim, now);
    unified_anim_render(&ctrl_anim, now);
}

void draw_wpm_frame_unified(void) {
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

void draw_logo_unified(void) {
    // Draw static logo
    draw_slice_px(&SLICE_logo, 10, 8);
}

// ============================================================================
// Migration Comparison Comments
// ============================================================================

/*
 * MIGRATION COMPARISON:
 *
 * OLD WAY (specialized controllers):
 *
 *   // Multiple controller types
 *   oneshot_anim_t layer_0_anim;
 *   bootrev_anim_t layer_frame_anim;
 *   toggle_anim_t caps_anim;
 *
 *   // Different initialization functions
 *   oneshot_anim_init(&layer_0_anim, &layer_0, 56, 0, true, true, now);
 *   bootrev_anim_init(&layer_frame_anim, &layer_frame, 42, 0, true, now);
 *   toggle_anim_init(&caps_anim, &caps_seq, 10, 2, false, now);
 *
 *   // Different render functions
 *   oneshot_anim_render(&layer_0_anim, now);
 *   bootrev_anim_render(&layer_frame_anim, now);
 *   toggle_anim_render(&caps_anim, now);
 *
 * NEW WAY (unified controllers):
 *
 *   // Single controller type
 *   unified_anim_t layer_0_anim;
 *   unified_anim_t layer_frame_anim;
 *   unified_anim_t caps_anim;
 *
 *   // Configuration-driven behavior
 *   static const unified_anim_config_t layer_0_config =
 *       UNIFIED_ONESHOT_CONFIG(&layer_0, 56, 0, STEADY_LAST, true);
 *   static const unified_anim_config_t layer_frame_config =
 *       UNIFIED_BOOTREV_CONFIG(&layer_frame, 42, 0, true);
 *   static const unified_anim_config_t caps_config =
 *       UNIFIED_TOGGLE_CONFIG(&caps_seq, 10, 2, BLEND_OPAQUE);
 *
 *   // Single initialization function
 *   unified_anim_init(&layer_0_anim, &layer_0_config, 0, now);
 *   unified_anim_init(&layer_frame_anim, &layer_frame_config, 0, now);
 *   unified_anim_init(&caps_anim, &caps_config, 0, now);
 *
 *   // Single render function
 *   unified_anim_render(&layer_0_anim, now);
 *   unified_anim_render(&layer_frame_anim, now);
 *   unified_anim_render(&caps_anim, now);
 *
 * BENEFITS:
 * - Consistent API across all animation types
 * - Configuration-driven behavior (easier to modify)
 * - Less code duplication
 * - Easier to understand and maintain
 * - All modifier animations now work (were disabled before)
 */
