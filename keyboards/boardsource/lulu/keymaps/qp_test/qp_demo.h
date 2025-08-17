/**
 * @file qp_demo.h
 * @brief QP Utils demonstration functions for qp_test keymap
 */

#pragma once

#include QMK_KEYBOARD_H
#include "qp_utils.h"
#include "qp_image.h"
#include "qp_anim.h"
#include "qp_controllers.h"
#include "qp_declarative.h"

// ============================================================================
// Demo Configuration
// ============================================================================

// Display dimensions for SH1106
#define DEMO_DISPLAY_WIDTH  128
#define DEMO_DISPLAY_HEIGHT 64

// Demo areas
#define DEMO_HEADER_HEIGHT  16
#define DEMO_STATUS_HEIGHT  12
#define DEMO_MAIN_Y         (DEMO_HEADER_HEIGHT + 2)
#define DEMO_MAIN_HEIGHT    (DEMO_DISPLAY_HEIGHT - DEMO_HEADER_HEIGHT - DEMO_STATUS_HEIGHT - 4)
#define DEMO_STATUS_Y       (DEMO_DISPLAY_HEIGHT - DEMO_STATUS_HEIGHT)

// ============================================================================
// Demo Functions
// ============================================================================

/**
 * @brief Initialize QP display and demo
 * @return True if initialization successful
 */
bool qp_demo_init(void);

/**
 * @brief Update demo display
 */
void qp_demo_update(void);

/**
 * @brief Draw basic test patterns
 */
void qp_demo_draw_test_patterns(void);

/**
 * @brief Draw layer indicator
 * @param layer Current layer index
 */
void qp_demo_draw_layer_indicator(uint8_t layer);

/**
 * @brief Draw modifier status
 * @param mods Current modifier mask
 */
void qp_demo_draw_modifier_status(uint8_t mods);

/**
 * @brief Draw WPM indicator
 * @param wpm Current WPM value
 */
void qp_demo_draw_wpm(uint8_t wpm);

/**
 * @brief Draw caps lock indicator
 * @param caps_on True if caps lock is on
 */
void qp_demo_draw_caps_lock(bool caps_on);

/**
 * @brief Cycle through different demo modes
 */
void qp_demo_cycle_mode(void);

/**
 * @brief Get current demo mode
 * @return Current demo mode index
 */
uint8_t qp_demo_get_mode(void);

// ============================================================================
// Demo Modes
// ============================================================================

typedef enum {
    DEMO_MODE_BASIC,        ///< Basic shapes and indicators
    DEMO_MODE_ANIMATION,    ///< Animation demonstrations
    DEMO_MODE_WIDGETS,      ///< Declarative widget demo
    DEMO_MODE_STRESS,       ///< Performance stress test
    DEMO_MODE_COUNT         ///< Number of demo modes
} qp_demo_mode_t;
