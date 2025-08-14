#pragma once

#include QMK_KEYBOARD_H
#include "oled_driver.h"
#include "wpm_bar_graph.h"

/**
 * @brief Initialize the WPM graph display
 * @return true if initialization successful, false otherwise
 */
bool wpm_graph_init(void);

/**
 * @brief Render the WPM graph using current typing statistics
 * @return true if rendering successful, false otherwise
 */
bool render_wpm_graph(void);
