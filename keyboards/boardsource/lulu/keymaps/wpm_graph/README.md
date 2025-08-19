# WPM Bar Graph OLED Display

This keymap implements a real-time WPM (Words Per Minute) bar graph display using the OLED driver and the typing_stats module.

## Features

- **Real-time WPM visualization** with a horizontal bar graph
- **Dual WPM indicators**:
  - Current WPM: 1-pixel wide white line
  - Average WPM: 3-pixel wide white line (session average)
- **Proportional scaling** based on session maximum WPM
- **Configurable positioning and sizing**
- **Optimized for SSD1306 128x32 monochrome OLED displays**

## Hardware Requirements

- **Controller**: RP2040-based Lulu keyboard
- **Display**: SSD1306 128x32 monochrome OLED (I2C)
- **I2C Address**: Standard OLED driver auto-detection

## Display Layout

```
┌─────────────────────────────────────────────────────────┐
│                    (Text area)                          │
│ ┌─────────────────────────────────────────────────────┐ │
│ │0                                              MAX   │ │
│ │ ▌  ▐                                                │ │
│ │ ▌  ▐                                                │ │
│ └─────────────────────────────────────────────────────┘ │
└─────────────────────────────────────────────────────────┘
```

- **Thick line (3px)**: Average session WPM
- **Thin line (1px)**: Current WPM
- **Scale**: 0 WPM (left) to Session Max WPM (right)
- **Minimum scale**: 60 WPM for reasonable display

## Configuration

All settings can be customized in `config.h`:

### Position and Size
```c
#define WPM_BAR_X 5          // X position (pixels from left)
#define WPM_BAR_Y 15         // Y position (pixels from top)
#define WPM_BAR_WIDTH 118    // Bar width in pixels
#define WPM_BAR_HEIGHT 12    // Bar height in pixels
```

### Colors (Monochrome)
```c
#define WPM_BAR_BORDER_COLOR_H 255    // White border
#define WPM_BAR_CURRENT_COLOR_H 255   // White current WPM line
#define WPM_BAR_AVERAGE_COLOR_H 255   // White average WPM line
#define WPM_BAR_BACKGROUND_COLOR_H 0  // Black background
```

## How It Works

1. **Data Collection**: Gets real-time WPM data from typing_stats module:
   - `ts_get_current_wpm()` - Current typing speed
   - `ts_get_avg_wpm()` - Session average (exponential moving average)
   - `ts_get_session_max_wpm()` - Session maximum for scaling

2. **Scaling**: Uses session max WPM as the right edge of the bar
   - Minimum scale of 60 WPM ensures reasonable display even for slow typing
   - Lines are positioned proportionally within the bar

3. **Rendering**: Updates at ~30fps with proper throttling
   - Clears display area
   - Draws 1-pixel border
   - Draws average WPM line (3px wide)
   - Draws current WPM line (1px wide)
   - Handles overlap by offsetting current line when too close to average

4. **Monochrome Optimization**: 
   - Uses line width differences to distinguish current vs average
   - Automatically offsets lines when they overlap for better visibility

## Installation

1. **Hardware Setup**: Connect your SSD1306 display via I2C to your Lulu keyboard
2. **Compilation**: Build with `qmk compile -kb boardsource/lulu/rp2040 -km wpm_graph`
3. **Flash**: Flash the resulting .uf2 file to your keyboard
4. **Enjoy**: Start typing to see your real-time WPM bar graph!

## Files Structure

- `wpm_bar_graph.h/c` - Core bar graph implementation
- `wpm.h/c` - Integration layer with typing_stats
- `config.h` - Configuration options
- `rules.mk` - Build configuration
- `keymap.c` - Main keymap with display initialization

## Customization

### Different Display Sizes
For other display sizes, update the configuration in `config.h`:
- Adjust `WPM_BAR_X`, `WPM_BAR_Y` for positioning
- Modify `WPM_BAR_WIDTH`, `WPM_BAR_HEIGHT` for sizing

### Different I2C Address
Update the address in `keymap.c`:
```c
display = qp_sh1106_make_i2c_device(128, 32, YOUR_ADDRESS);
```

### Color Displays
For color displays, modify the HSV color values in `config.h` and update the display driver in `rules.mk`.

## Troubleshooting

- **Display not working**: Check I2C connections and address
- **Compilation errors**: Ensure you're using RP2040 version (AVR doesn't support Quantum Painter)
- **Lines not visible**: Adjust `WPM_BAR_HEIGHT` for better visibility
- **Scale issues**: The bar automatically scales to session max, minimum 60 WPM

## Integration with Typing Stats

This implementation leverages the full typing_stats module for accurate WPM tracking:
- Real-time WPM calculation
- Session-based statistics
- Exponential moving average for smooth average display
- Automatic session management
