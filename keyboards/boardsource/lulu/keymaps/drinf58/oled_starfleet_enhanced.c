/**
 * @file oled_starfleet_enhanced.c
 * @brief Enhanced drinf58 OLED display using new slice system features
 * 
 * This file demonstrates how to use the comprehensive SLICE macro system
 * and add enhanced features while maintaining the original functionality.
 */

#include QMK_KEYBOARD_H
#include "oled_starfleet.h"
#include "progmem_starfleet.h"
#include "oled_utils.h"
#include "oled_unified_anim.h"  // Add animation capabilities

// ============================================================================
// Enhanced Slice Definitions using New Comprehensive System
// ============================================================================

// Main display elements (using standard macros from oled_slice.h)
static const slice_t SLICE_kbd = SLICE128x32(kbd);
static const slice_t SLICE_logo = SLICE24x32(logo);
static const slice_t SLICE_wpm_title = SLICE8x32(wpm_title);

// WPM frame with layer-specific variations
static const slice_t SLICE_wmp_frame[LAYER_COUNT] = {
    SLICE16x8(wpm_frame_0),  // Layer 0
    SLICE16x8(wpm_frame_1),  // Layer 1
    SLICE16x8(wpm_frame_2),  // Layer 2
    SLICE16x8(wpm_frame_2)   // Layer 3 (reuse layer 2)
};

// Digit display with leading/trailing variants
static const slice_t SLICE_digits[10][2] = {
    {SLICE16x8(digit_0_trail), SLICE16x8(digit_0_lead)},
    {SLICE16x8(digit_1_trail), SLICE16x8(digit_1_lead)},
    {SLICE16x8(digit_2_trail), SLICE16x8(digit_2_lead)},
    {SLICE16x8(digit_3_trail), SLICE16x8(digit_3_lead)},
    {SLICE16x8(digit_4_trail), SLICE16x8(digit_4_lead)},
    {SLICE16x8(digit_5_trail), SLICE16x8(digit_5_lead)},
    {SLICE16x8(digit_6_trail), SLICE16x8(digit_6_lead)},
    {SLICE16x8(digit_7_trail), SLICE16x8(digit_7_lead)},
    {SLICE16x8(digit_8_trail), SLICE16x8(digit_8_lead)},
    {SLICE16x8(digit_9_trail), SLICE16x8(digit_9_lead)},
};

// Layer indicators (5 frames × 4 layers)
static const slice_t SLICE_layer[LAYER_COUNT + 1][LAYER_COUNT] = {
    // Layer 0 states
    {SLICE8x32(layer_0_0), SLICE8x32(layer_0_1), SLICE8x32(layer_0_2), SLICE8x32(layer_0_3)},
    // Layer 1 states  
    {SLICE8x32(layer_1_0), SLICE8x32(layer_1_1), SLICE8x32(layer_1_2), SLICE8x32(layer_1_3)},
    // Layer 2 states
    {SLICE8x32(layer_2_0), SLICE8x32(layer_2_1), SLICE8x32(layer_2_2), SLICE8x32(layer_2_3)},
    // Layer 3 states
    {SLICE8x32(layer_3_0), SLICE8x32(layer_3_1), SLICE8x32(layer_3_2), SLICE8x32(layer_3_3)},
    // Bottom decoration
    {SLICE8x32(layer_bottom_0), SLICE8x32(layer_bottom_0), SLICE8x32(layer_bottom_0), SLICE8x32(layer_bottom_3)}
};

// Modifier indicators (2 states × 4 layers each)
static const slice_t SLICE_ctrl[NUM_MOD_STATES][LAYER_COUNT] = {
    {SLICE8x32(ctrl_0_0), SLICE8x32(ctrl_0_0), SLICE8x32(ctrl_0_0), SLICE8x32(ctrl_0_0)},
    {SLICE8x32(ctrl_1_0), SLICE8x32(ctrl_1_0), SLICE8x32(ctrl_1_0), SLICE8x32(ctrl_1_0)}
};

static const slice_t SLICE_shift[NUM_MOD_STATES][LAYER_COUNT] = {
    {SLICE8x32(shift_0_0), SLICE8x32(shift_0_1), SLICE8x32(shift_0_1), SLICE8x32(shift_0_1)},
    {SLICE8x32(shift_1_0), SLICE8x32(shift_1_1), SLICE8x32(shift_1_1), SLICE8x32(shift_1_1)}
};

static const slice_t SLICE_alt[NUM_MOD_STATES][LAYER_COUNT] = {
    {SLICE8x32(alt_0_0), SLICE8x32(alt_0_0), SLICE8x32(alt_0_2), SLICE8x32(alt_0_2)},
    {SLICE8x32(alt_1_0), SLICE8x32(alt_1_0), SLICE8x32(alt_1_2), SLICE8x32(alt_1_2)}
};

static const slice_t SLICE_super[NUM_MOD_STATES][LAYER_COUNT] = {
    {SLICE8x32(super_0_0), SLICE8x32(super_0_0), SLICE8x32(super_0_0), SLICE8x32(super_0_3)},
    {SLICE8x32(super_1_0), SLICE8x32(super_1_0), SLICE8x32(super_1_0), SLICE8x32(super_1_3)}
};

static const slice_t SLICE_caps[NUM_MOD_STATES][LAYER_COUNT] = {
    {SLICE8x32(caps_0_0), SLICE8x32(caps_0_0), SLICE8x32(caps_0_0), SLICE8x32(caps_0_0)},
    {SLICE8x32(caps_1_0), SLICE8x32(caps_1_0), SLICE8x32(caps_1_0), SLICE8x32(caps_1_0)}
};

// ============================================================================
// NEW: Enhanced Features using Unified Animation System
// ============================================================================

// WPM threshold animation sequences (demonstrate arbitrary heights)
DEFINE_SLICE_SEQ(wpm_glow_seq,
    SLICE16x10(wpm_glow_0),  // Using arbitrary height macros
    SLICE16x10(wpm_glow_1),
    SLICE16x10(wpm_glow_2),
    SLICE16x10(wpm_glow_3)
);

// Layer transition animation
DEFINE_SLICE_SEQ(layer_transition_seq,
    SLICE8x12(layer_trans_0),  // 12-pixel high transition effect
    SLICE8x12(layer_trans_1),
    SLICE8x12(layer_trans_2)
);

// Boot animation for slave side
DEFINE_SLICE_SEQ(boot_seq,
    SLICE128x32(boot_0),
    SLICE128x32(boot_1), 
    SLICE128x32(boot_2),
    SLICE128x32(boot_3)
);

// Animation configurations
static const unified_anim_config_t wpm_glow_config = 
    UNIFIED_TOGGLE_CONFIG(&wpm_glow_seq, 104, 14, BLEND_ADDITIVE);

static const unified_anim_config_t layer_transition_config = 
    UNIFIED_OUTBACK_CONFIG(&layer_transition_seq, 64, 10, STEADY_FIRST, false);

static const unified_anim_config_t boot_config = 
    UNIFIED_ONESHOT_CONFIG(&boot_seq, 0, 0, STEADY_LAST, true);

// Animation instances
static unified_anim_t wpm_glow_anim;
static unified_anim_t layer_transition_anim;
static unified_anim_t boot_anim;

// ============================================================================
// Enhanced Rendering Functions
// ============================================================================

void render_wpm_enhanced(void) {
    const uint8_t         x                      = 104;
    const uint8_t         y_slot[WPM_MAX_DIGITS] = {24, 16, 8};
    static const uint16_t place[3]               = {1, 10, 100};
    
    uint8_t wpm = get_current_wpm();
    uint8_t sel = get_highest_layer(layer_state);
    uint32_t now = timer_read32();
    
    // Calculate number of digits
    uint8_t nd = (wpm >= 100) ? 3 : (wpm >= 10) ? 2 : 1;
    
    // Clear unused digit positions
    for (uint8_t s = nd; s < WPM_MAX_DIGITS; s++) {
        clear_span16(x, y_slot[s]);
    }
    
    // Draw digits
    for (uint8_t i = 0; i < nd; i++) {
        uint8_t digit = (wpm / place[i]) % 10;
        bool leading = (i == nd - 1) ? 1 : 0;
        
        draw_slice_px(&SLICE_digits[digit][leading], x, y_slot[i]);
    }
    
    // Draw WPM frame and title
    draw_slice_px(&SLICE_wpm_title, 120, 0);
    draw_slice_px(&SLICE_wpm_frame[sel], x, 0);
    
    // NEW: Add WPM glow effect for high typing speeds
    bool high_wpm = (wpm > 60);
    unified_anim_trigger(&wpm_glow_anim, high_wpm ? 1 : 0, now);
    unified_anim_render(&wpm_glow_anim, now);
}

void render_layers_enhanced(void) {
    uint8_t sel = get_highest_layer(layer_state);
    uint8_t x = 64;
    uint32_t now = timer_read32();
    
    // Track layer changes for transition animation
    static uint8_t last_layer = 0;
    if (sel != last_layer) {
        unified_anim_trigger(&layer_transition_anim, 0, now);
        last_layer = sel;
    }
    
    // Draw layer indicators (5 frames)
    for (uint8_t f = 0; f < 5; f++) {
        draw_slice_px(&SLICE_layer[4 - f][sel], x + (f * 8), 0);
    }
    
    // NEW: Add transition animation overlay
    unified_anim_render(&layer_transition_anim, now);
}

void render_modifiers_enhanced(void) {
    uint8_t layer = get_highest_layer(layer_state);
    uint8_t mods = get_mods() | get_oneshot_mods();
    
    // Determine modifier states
    uint8_t ctrl_state = (mods & MOD_MASK_CTRL) ? 1 : 0;
    uint8_t shift_state = (mods & MOD_MASK_SHIFT) ? 1 : 0;
    uint8_t alt_state = (mods & MOD_MASK_ALT) ? 1 : 0;
    uint8_t super_state = (mods & MOD_MASK_GUI) ? 1 : 0;
    uint8_t caps_state = (host_keyboard_led_state().caps_lock || is_caps_word_on()) ? 1 : 0;
    
    // Draw modifier indicators
    draw_slice_px(&SLICE_ctrl[ctrl_state][layer], 0, 0);
    draw_slice_px(&SLICE_shift[shift_state][layer], 8, 0);
    draw_slice_px(&SLICE_alt[alt_state][layer], 16, 0);
    draw_slice_px(&SLICE_super[super_state][layer], 24, 0);
    draw_slice_px(&SLICE_caps[caps_state][layer], 32, 0);
}

void render_logo_enhanced(void) {
    uint32_t now = timer_read32();
    
    // Show boot animation first
    if (!unified_anim_boot_done(&boot_anim)) {
        unified_anim_render(&boot_anim, now);
        return;
    }
    
    // Draw static logo after boot
    draw_slice_px(&SLICE_logo, 0, 0);
}

void render_slave_enhanced(void) {
    uint32_t now = timer_read32();
    
    // Show boot animation first
    if (!unified_anim_boot_done(&boot_anim)) {
        unified_anim_render(&boot_anim, now);
        return;
    }
    
    // Draw keyboard graphic after boot
    draw_slice_px(&SLICE_kbd, 0, 0);
}

// ============================================================================
// Enhanced Initialization
// ============================================================================

void init_starfleet_enhanced(void) {
    uint32_t now = timer_read32();
    
    // Initialize animations
    unified_anim_init(&wpm_glow_anim, &wpm_glow_config, 0, now);
    unified_anim_init(&layer_transition_anim, &layer_transition_config, 0, now);
    unified_anim_init(&boot_anim, &boot_config, 0, now);
}

// ============================================================================
// Backward Compatibility Functions (original API)
// ============================================================================

void render_wpm(void) {
    render_wpm_enhanced();
}

void render_layers(void) {
    render_layers_enhanced();
}

void render_modifiers(void) {
    render_modifiers_enhanced();
}

void render_logo(void) {
    render_logo_enhanced();
}

void render_slave(void) {
    render_slave_enhanced();
}

// ============================================================================
// Migration Notes and Examples
// ============================================================================

/*
 * SLICE MACRO MIGRATION EXAMPLES:
 * 
 * OLD WAY (manual definitions):
 *   #define SLICE16x8(p) ((const slice_t){(const uint8_t PROGMEM *)(p), 16, 1})
 *   #define SLICE8x32(p) ((const slice_t){(const uint8_t PROGMEM *)(p), 8, 4})
 * 
 * NEW WAY (comprehensive system):
 *   // Standard macros provided by oled_slice.h
 *   SLICE16x8(my_data)   // 16×8 bitmap
 *   SLICE8x32(my_data)   // 8×32 bitmap
 *   
 *   // Arbitrary heights now supported
 *   SLICE16x10(my_data)  // 16×10 bitmap (needs 2 pages)
 *   SLICE8x12(my_data)   // 8×12 bitmap (needs 2 pages)
 *   
 *   // Custom sizes
 *   SLICE_CUSTOM_PX(my_data, 20, 14)  // 20×14 bitmap
 * 
 * ENHANCED FEATURES ADDED:
 * 
 * 1. WPM Glow Effect:
 *    - Animated glow when typing speed > 60 WPM
 *    - Uses additive blending for overlay effect
 * 
 * 2. Layer Transition Animation:
 *    - Smooth transition effect when changing layers
 *    - Out-and-back animation pattern
 * 
 * 3. Boot Animation:
 *    - Animated startup sequence on both master and slave
 *    - Transitions to normal display after completion
 * 
 * 4. Arbitrary Height Support:
 *    - Can now use 10px, 12px, 14px high bitmaps
 *    - Proper clipping to exact pixel heights
 * 
 * BENEFITS:
 * - More SLICE macros available (8×8 to 128×32)
 * - Arbitrary height support for precise layouts
 * - Enhanced visual effects with animations
 * - Backward compatibility maintained
 * - Better code organization and documentation
 */
