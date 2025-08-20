# ZIBO Keyboard Input Plugin

[![Version](https://img.shields.io/badge/version-1.0.7-brightgreen.svg)](VERSION)
[![License](https://img.shields.io/badge/license-MIT-blue.svg)](LICENSE)
[![Platform](https://img.shields.io/badge/platform-X--Plane%2011%2B-orange.svg)](https://www.x-plane.com/)
[![macOS](https://img.shields.io/badge/macOS-10.15%2B-green.svg)](https://www.apple.com/macos/)

**ZIBOKeyboardInput** is a high-performance native C++ X-Plane plugin that revolutionizes flight management computer interaction for the popular ZIBO 737 aircraft. This plugin eliminates the tedious point-and-click FMC operation by enabling direct keyboard input, allowing pilots to type flight plans, waypoints, and navigation data directly into the FMC using their physical keyboard.

The plugin features intelligent aircraft detection, dual FMC support for both pilot positions, real-time visual feedback, and seamless integration with X-Plane's command system. Built from the ground up in C++ for optimal performance and stability, it provides a more realistic and efficient flight simulation experience.

## ✨ Features

- ⌨️ **Direct Keyboard Input**: Type directly into ZIBO 737 FMC using your keyboard
- 👨‍✈️ **Dual FMC Support**: Switch between Captain and First Officer FMC positions
- 🎯 **Visual Indicators**: Real-time display showing active FMC position and status
- 🔍 **Aircraft Auto-Detection**: Automatically activates only when ZIBO 737 (ICAO: B738) is loaded
- 🔧 **Custom Commands**: X-Plane commands for easy key binding and joystick integration
- 🚀 **Native Performance**: C++ implementation for optimal performance and stability
- 🎛️ **Intelligent Key Mapping**: Advanced key mapping with special character support (plus signs, minus signs, etc.)
- 💾 **Cross-Platform**: Universal support for macOS (Intel + Apple Silicon), Windows, and Linux

## 🎯 How It Works

The plugin seamlessly integrates with X-Plane's input system to provide natural keyboard-to-FMC interaction:

1. **Auto-Detection**: When you load the ZIBO 737, the plugin automatically detects the aircraft and becomes available
2. **Toggle Activation**: Use custom keyboard shortcuts to toggle input for Captain or First Officer FMC
3. **Direct Input**: Once enabled, simply type on your keyboard - letters, numbers, and symbols appear directly in the FMC
4. **Visual Feedback**: A small status indicator shows which FMC position is active
5. **Smart Filtering**: The plugin only captures relevant keys, allowing normal X-Plane shortcuts to work unchanged

This eliminates the need for tedious mouse clicking on FMC buttons, making flight planning faster and more realistic.

## 🛠️ System Requirements

- **Operating System**: macOS 10.15 or higher, Windows 10+, or Linux
- **X-Plane**: X-Plane 11 or X-Plane 12 (supports XPLM 410+ SDK)
- **Aircraft**: ZIBO 737 (ICAO: B738)
- **Development Tools**: 
  - Xcode Command Line Tools (macOS)
  - CMake 3.16 or higher

## 🚀 Quick Start

### 1. Install Dependencies

```bash
# Install Xcode Command Line Tools (macOS)
xcode-select --install

# Install CMake (using Homebrew)
brew install cmake
```

### 2. Build Plugin

```bash
# Clone or download this project
cd ZIBOKeyboardInput

# Build plugin
./build.sh

# Or clean and rebuild
./build.sh clean
```

### 3. Install Plugin

```bash
# Auto-install to X-Plane directory
./install.sh

# Or manually specify X-Plane path
./install.sh "/Applications/X-Plane 12"
```

After successful build, the plugin file `mac.xpl` will be installed to:
```
X-Plane/Resources/plugins/ZIBOKeyboardInput/mac.xpl
```

## 📖 Detailed Documentation

### Key Mappings

When keyboard input is enabled, the following keys are mapped to FMC buttons:

| Keyboard Key | FMC Function | Description |
|--------------|-------------|-------------|
| 0-9 | Numbers 0-9 | Numeric input |
| A-Z | Letters A-Z | Alphabetic input |
| Backspace | CLR | Clear last character |
| Space | SP | Space character |
| Delete | DEL | Delete function |
| / (main or numpad) | Slash | Forward slash |
| . | Period | Decimal point |
| - (main or numpad) | Minus | Minus sign |
| Shift + = | Plus* | Plus sign (special handling) |
| Numpad + | Plus* | Plus sign (special handling) |

*_Note: Plus signs use a special double-minus technique to work with ZIBO FMC's internal logic_

### Custom Commands

The plugin provides two X-Plane commands that you can bind to keyboard shortcuts or joystick buttons:

- `Zibo/ZIBO_Keyboard/Toggle_Keyboard_Input_Captain` - Toggle Captain FMC keyboard input
- `Zibo/ZIBO_Keyboard/Toggle_Keyboard_Input_FO` - Toggle First Officer FMC keyboard input

### Visual Indicators

When keyboard input is active, you will see:
- **Bottom-right corner**: Floating status window with green "KB:CAP" or "KB:FO" indicator
- **Real-time feedback**: The indicator appears only when input is enabled and automatically hides when disabled
- **Aircraft-specific**: Only displays when ZIBO 737 is loaded and plugin is active

### Usage Instructions

1. Load the ZIBO 737 aircraft in X-Plane
2. Assign keyboard shortcuts to the plugin commands in X-Plane Settings > Keyboard
3. Press your assigned key to toggle keyboard input for Captain or First Officer FMC
4. When enabled, type directly - characters will appear in the active FMC
5. Toggle off when finished or to switch between positions

## 📁 Project Structure

```
ZIBOKeyboardInput/
├── README.md                     # Project documentation
├── BUILD_INSTRUCTIONS.md        # Detailed build instructions
├── LICENSE                      # License file
├── CMakeLists.txt              # CMake configuration file
├── build.sh                    # Build script
├── install.sh                  # Installation script
├── mac_exports.txt             # macOS symbol export list
├── win_exports.def             # Windows symbol export list
├── linux_exports.txt           # Linux symbol export list
├── .gitignore                  # Git ignore file list
├── src/                        # Source code directory
│   └── zibo_keyboard_input.cpp # Main plugin code
└── XPLM-SDK/                   # X-Plane SDK
    ├── CHeaders/               # C/C++ header files
    └── Libraries/              # Platform-specific library files
```

## 🔧 Development Notes

### Manual Build

If you prefer to manually build using CMake:

```bash
mkdir build && cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
cmake --build . --config Release
```

### Architecture Support

The plugin supports the following architectures:
- **macOS**: Universal Binary (Intel x86_64 + Apple Silicon arm64)
- **Windows**: x64 architecture
- **Linux**: x64 architecture

### Setting up X-Plane SDK

1. Download the X-Plane SDK from [developer.x-plane.com](https://developer.x-plane.com/)
2. Extract the SDK to the project directory as `XPLM-SDK/`
3. Ensure the directory structure matches:
   ```
   XPLM-SDK/
   ├── CHeaders/
   │   ├── XPLM/
   │   └── Widgets/
   └── Libraries/
       ├── Mac/
       ├── Win/
       └── Lin/
   ```

## 🎮 Usage Tips

### Best Practices
- Use the plugin only when actively entering data into the FMC
- Toggle off keyboard input when not needed to avoid interference
- Bind commands to easily accessible keys for quick toggling

### Troubleshooting

#### Plugin Not Loading
- Verify the plugin is in the correct directory: `X-Plane/Resources/plugins/ZIBOKeyboardInput/`
- Check X-Plane's Log.txt for error messages
- Ensure the plugin file has the correct extension (`.xpl`)

#### Keyboard Input Not Working
- Confirm you're using the ZIBO 737 (other 737s are not supported)
- Verify keyboard input is enabled (look for the red indicator)
- Check that the FMC is powered and operational
- Try toggling the input off and on again

#### Commands Not Appearing
- Restart X-Plane after installing the plugin
- Search for commands starting with "Zibo" in Settings > Keyboard

## 🤝 Contributing

We welcome contributions from the community! Please see our [Contributing Guidelines](CONTRIBUTING.md) for detailed information on:

- Setting up the development environment
- Code style and standards
- Submitting bug reports and feature requests
- Pull request process and testing requirements
- Platform-specific development considerations

**Quick Start for Contributors:**
1. Fork this repository
2. Follow the setup instructions in [CONTRIBUTING.md](CONTRIBUTING.md)
3. Make your changes and test thoroughly
4. Submit a pull request with a clear description

For questions or discussions, please create an issue or use GitHub Discussions.

**Code of Conduct:** This project follows our [Code of Conduct](CODE_OF_CONDUCT.md) to ensure a welcoming environment for all contributors.

**Security:** For security-related concerns, please review our [Security Policy](SECURITY.md) and follow the responsible disclosure process outlined there.

## 🔧 Technical Overview

This plugin operates by:
1. **Aircraft Detection**: Monitors the `sim/aircraft/view/acf_ICAO` dataref to detect ZIBO 737 (B738)
2. **Key Interception**: Registers a key sniffer callback to capture keyboard input
3. **Command Translation**: Maps virtual key codes to ZIBO FMC button commands (`laminar/B738/button/fmc1_*` or `laminar/B738/button/fmc2_*`)
4. **Visual Feedback**: Creates floating status windows showing active FMC position
5. **Smart Filtering**: Ignores modifier key combinations to prevent interference with X-Plane shortcuts

### Special Character Handling
- **Plus Signs**: Uses a sophisticated double-minus technique since ZIBO FMC toggles between minus and plus on repeated presses
- **Dual Input Support**: Both main keyboard and numpad keys are supported for maximum compatibility
- **Modifier Key Safety**: Prevents accidental FMC input when using keyboard shortcuts

## 📝 Changelog

### v1.0.7 (Current)
- 🚀 Stable C++ implementation with enhanced reliability
- 🎯 Advanced visual feedback system with floating status indicators
- 🔧 Comprehensive cross-platform support (macOS Universal, Windows x64, Linux x64)
- 🎛️ Intelligent key mapping with special character support
- 📱 Modern X-Plane SDK integration (XPLM 300+)

### v1.0.0 (Original Lua)
- 🎮 Initial Lua script implementation
- ⌨️ Basic keyboard input functionality
- 👥 Support for Captain and FO positions

## 📄 License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## 🙏 Acknowledgments

- **ZIBO 737**: Outstanding work by the Zibo aircraft development team
- **X-Plane SDK**: Laminar Research for the comprehensive development tools
- **Community**: All beta testers and contributors

## 💬 Support

For issues, suggestions, or questions:

1. Check the troubleshooting section above
2. Review X-Plane's Log.txt for error messages
3. Create an issue on the project repository
4. Include your X-Plane version, operating system, and error details

---

**Note**: This plugin is specifically designed for the ZIBO 737 and will not work with other aircraft. The plugin automatically detects the ZIBO 737 and enables functionality only when this aircraft is loaded.