#pragma once

#include QMK_KEYBOARD_H
#include "wpm_stats.h"

// =============================================================================
// CONFIGURATION DEFAULTS
// =============================================================================

#ifndef WPM_BAR_X
#define WPM_BAR_X 10
#endif

#ifndef WPM_BAR_Y
#define WPM_BAR_Y 10
#endif

#ifndef WPM_BAR_WIDTH
#define WPM_BAR_WIDTH 100
#endif

#ifndef WPM_BAR_HEIGHT
#define WPM_BAR_HEIGHT 20
#endif

// =============================================================================
// DATA STRUCTURES
// =============================================================================

/**
 * @brief Configuration structure for WPM bar graph
 */
typedef struct {
    uint16_t x;                    // X position of the bar
    uint16_t y;                    // Y position of the bar
    uint16_t width;                // Width of the bar
    uint16_t height;               // Height of the bar
} wpm_bar_config_t;

// =============================================================================
// PUBLIC API
// =============================================================================

/**
 * @brief Initialize the WPM bar graph with default configuration
 * @return true if initialization successful, false otherwise
 */
bool wpm_bar_graph_init(void);

/**
 * @brief Initialize the WPM bar graph with custom configuration
 * @param config Custom configuration structure
 * @return true if initialization successful, false otherwise
 */
bool wpm_bar_graph_init_config(const wpm_bar_config_t *config);

/**
 * @brief Render the WPM bar graph
 * @param wpm_data Current WPM statistics
 * @return true if rendering successful, false otherwise
 */
bool wpm_bar_graph_render(const wpm_stats_t *wpm_data);

/**
 * @brief Update the configuration of the WPM bar graph
 * @param config New configuration structure
 * @return true if update successful, false otherwise
 */
bool wpm_bar_graph_update_config(const wpm_bar_config_t *config);

/**
 * @brief Get the current configuration
 * @param config Pointer to store the current configuration
 * @return true if successful, false otherwise
 */
bool wpm_bar_graph_get_config(wpm_bar_config_t *config);

/**
 * @brief Clear the WPM bar graph area
 * @return true if successful, false otherwise
 */
bool wpm_bar_graph_clear(void);

/**
 * @brief Get default configuration structure
 * @return Default configuration structure
 */
wpm_bar_config_t wpm_bar_graph_get_default_config(void);
