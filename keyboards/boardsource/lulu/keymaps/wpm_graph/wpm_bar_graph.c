#include "wpm_bar_graph.h"
#include "oled_driver.h"

// =============================================================================
// STATIC VARIABLES
// =============================================================================

static wpm_bar_config_t g_config;
static bool g_initialized = false;

// =============================================================================
// PRIVATE FUNCTIONS
// =============================================================================

/**
 * @brief Calculate the X position for a WPM value within the bar
 * @param wpm WPM value to position
 * @param max_wpm Maximum WPM value for scaling
 * @return X position within the bar (relative to bar start)
 */
static uint16_t calculate_wpm_position(uint16_t wpm, uint16_t max_wpm) {
    if (max_wpm == 0) {
        return 0;
    }

    // Calculate position within the inner area (excluding border)
    uint16_t inner_width = g_config.width - 2;  // Subtract 2 for left and right borders

    // Position markers proportionally between 0 and max_wpm
    // If wpm equals max_wpm, position should be at inner_width (100% of bar)
    // If wpm is half of max_wpm, position should be at inner_width/2 (50% of bar)
    uint32_t position = ((uint32_t)wpm * inner_width) / max_wpm;

    // Ensure we don't exceed the inner width (this handles cases where wpm > max_wpm due to rounding)
    if (position > inner_width) {
        position = inner_width;
    }

    return (uint16_t)position;
}

/**
 * @brief Set a pixel on the OLED display
 * @param x X coordinate
 * @param y Y coordinate
 * @param on true for white pixel, false for black
 */
static void set_pixel(uint16_t x, uint16_t y, bool on) {
    if (x >= OLED_DISPLAY_WIDTH || y >= OLED_DISPLAY_HEIGHT) {
        return;
    }

    // Use the OLED driver's pixel function
    oled_write_pixel(x, y, on);
}

/**
 * @brief Draw a horizontal line
 * @param x1 Start X coordinate
 * @param x2 End X coordinate
 * @param y Y coordinate
 * @param on true for white line, false for black
 */
static void draw_hline(uint16_t x1, uint16_t x2, uint16_t y, bool on) {
    for (uint16_t x = x1; x <= x2; x++) {
        set_pixel(x, y, on);
    }
}

/**
 * @brief Draw a vertical line
 * @param x X coordinate
 * @param y1 Start Y coordinate
 * @param y2 End Y coordinate
 * @param on true for white line, false for black
 */
static void draw_vline(uint16_t x, uint16_t y1, uint16_t y2, bool on) {
    for (uint16_t y = y1; y <= y2; y++) {
        set_pixel(x, y, on);
    }
}

/**
 * @brief Draw the bar border
 * @return true if successful, false otherwise
 */
static bool draw_bar_border(void) {
    // Draw border rectangle (unfilled)
    uint16_t x1 = g_config.x;
    uint16_t y1 = g_config.y;
    uint16_t x2 = g_config.x + g_config.width - 1;
    uint16_t y2 = g_config.y + g_config.height - 1;

    // Top and bottom lines
    draw_hline(x1, x2, y1, true);
    draw_hline(x1, x2, y2, true);

    // Left and right lines
    draw_vline(x1, y1, y2, true);
    draw_vline(x2, y1, y2, true);

    return true;
}

/**
 * @brief Clear the inner area of the bar
 * @return true if successful, false otherwise
 */
static bool clear_bar_inner(void) {
    // Clear inner area (fill with black)
    for (uint16_t y = g_config.y + 1; y < g_config.y + g_config.height - 1; y++) {
        for (uint16_t x = g_config.x + 1; x < g_config.x + g_config.width - 1; x++) {
            set_pixel(x, y, false);
        }
    }
    return true;
}

/**
 * @brief Draw a vertical line for WPM indicator
 * @param wpm_position X position within the bar (relative to bar start)
 * @param line_width Width of the line (1 or 3 pixels)
 * @return true if successful, false otherwise
 */
static bool draw_wpm_line(uint16_t wpm_position, uint8_t line_width) {
    uint16_t line_x = g_config.x + 1 + wpm_position;  // +1 for border offset
    uint16_t line_y_start = g_config.y + 1;           // +1 for border offset
    uint16_t line_y_end = g_config.y + g_config.height - 2;  // -2 for border offset

    // Draw the line(s) based on width
    for (uint8_t i = 0; i < line_width; i++) {
        uint16_t current_x = line_x + i;

        // Make sure we don't draw outside the bar
        if (current_x >= g_config.x + g_config.width - 1) {
            break;
        }

        draw_vline(current_x, line_y_start, line_y_end, true);
    }

    return true;
}

// =============================================================================
// PUBLIC API IMPLEMENTATION
// =============================================================================

wpm_bar_config_t wpm_bar_graph_get_default_config(void) {
    wpm_bar_config_t config = {
        .x = WPM_BAR_X,
        .y = WPM_BAR_Y,
        .width = WPM_BAR_WIDTH,
        .height = WPM_BAR_HEIGHT
    };
    return config;
}

bool wpm_bar_graph_init(void) {
    wpm_bar_config_t default_config = wpm_bar_graph_get_default_config();
    return wpm_bar_graph_init_config(&default_config);
}

bool wpm_bar_graph_init_config(const wpm_bar_config_t *config) {
    if (!config) {
        return false;
    }

    g_config = *config;
    g_initialized = true;

    return true;
}

bool wpm_bar_graph_render(const wpm_stats_t *wpm_data) {
    if (!g_initialized || !wpm_data) {
        return false;
    }

    // Use the user's session max WPM as the scale for consistent proportional positioning
    uint16_t max_wpm = wpm_data->session_max_wpm;

    // Set a reasonable minimum scale for when the user hasn't established a max yet
    uint16_t min_scale = 60;
    if (max_wpm < min_scale) {
        max_wpm = min_scale;
    }

    // Clear the area above the bar for text
    for (uint16_t y = 0; y < g_config.y; y++) {
        for (uint16_t x = 0; x < OLED_DISPLAY_WIDTH; x++) {
            set_pixel(x, y, false);
        }
    }

    // // Display WPM text values above the graph
    // oled_set_cursor(0, 0);
    // oled_write_P(PSTR("WPM: "), false);
    // oled_write(get_u16_str(wpm_data->current_wpm, ' '), false);

    // oled_set_cursor(0, 1);
    // oled_write_P(PSTR("Avg: "), false);
    // oled_write(get_u16_str(wpm_data->average_wpm, ' '), false);
    // oled_write_P(PSTR(" Max: "), false);
    // oled_write(get_u16_str(wpm_data->session_max_wpm, ' '), false);

    // Clear the inner area
    if (!clear_bar_inner()) {
        return false;
    }

    // Draw the border
    if (!draw_bar_border()) {
        return false;
    }

    // Calculate positions for current and average WPM
    uint16_t current_pos, average_pos;

    // If no stats recorded yet (session_max_wpm is 0), draw both lines at position 0
    if (wpm_data->session_max_wpm == 0) {
        current_pos = 0;
        average_pos = 0;
    } else {
        current_pos = calculate_wpm_position(wpm_data->current_wpm, max_wpm);
        average_pos = calculate_wpm_position(wpm_data->average_wpm, max_wpm);
    }

    // For monochrome displays, we need to make the lines distinguishable
    // Draw average WPM line first (3px wide, solid white)
    if (!draw_wpm_line(average_pos, 3)) {
        return false;
    }

    // Draw current WPM line (1px wide, on top)
    // If current and average are close, offset current line by 1 pixel to make it visible
    uint16_t current_draw_pos = current_pos;
    if (current_pos >= average_pos && current_pos <= average_pos + 2) {
        // Current is within the average line area, offset it
        current_draw_pos = (average_pos > 2) ? average_pos - 2 : average_pos + 4;
        // Make sure we don't go outside the bar
        uint16_t inner_width = g_config.width - 2;
        if (current_draw_pos >= inner_width) {
            current_draw_pos = inner_width - 1;
        }
    }

    if (!draw_wpm_line(current_draw_pos, 1)) {
        return false;
    }

    return true;
}

bool wpm_bar_graph_update_config(const wpm_bar_config_t *config) {
    if (!config) {
        return false;
    }

    g_config = *config;
    return true;
}

bool wpm_bar_graph_get_config(wpm_bar_config_t *config) {
    if (!config) {
        return false;
    }

    *config = g_config;
    return true;
}

bool wpm_bar_graph_clear(void) {
    if (!g_initialized) {
        return false;
    }

    // Clear the entire bar area with background color (black)
    for (uint16_t y = g_config.y; y < g_config.y + g_config.height; y++) {
        for (uint16_t x = g_config.x; x < g_config.x + g_config.width; x++) {
            set_pixel(x, y, false);
        }
    }

    return true;
}
