#include "wpm.h"
#include "wpm_bar_graph.h"
#include "wpm_stats.h"
#include "oled_driver.h"

// =============================================================================
// STATIC VARIABLES
// =============================================================================

static bool g_wpm_graph_initialized = false;

// =============================================================================
// PUBLIC API IMPLEMENTATION
// =============================================================================

bool wpm_graph_init(void) {
    // Initialize the WPM bar graph with default configuration
    if (!wpm_bar_graph_init()) {
        return false;
    }

    g_wpm_graph_initialized = true;
    return true;
}

bool render_wpm_graph(void) {
    if (!g_wpm_graph_initialized) {
        return false;
    }

    // Get current WPM statistics from wpm_stats module
    wpm_stats_t wpm_data = {
        .current_wpm = wpm_stats_get_current(),
        .average_wpm = wpm_stats_get_avg(),
        .session_max_wpm = wpm_stats_get_max()  // Using lifetime max as session max
    };

    // Render the bar graph
    return wpm_bar_graph_render(&wpm_data);
}
