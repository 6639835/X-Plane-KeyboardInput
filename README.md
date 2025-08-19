# ZIBO Keyboard Input Plugin

[![License](https://img.shields.io/badge/license-MIT-blue.svg)](LICENSE)
[![Platform](https://img.shields.io/badge/platform-X--Plane%2011%2B-orange.svg)](https://www.x-plane.com/)
[![macOS](https://img.shields.io/badge/macOS-10.15%2B-green.svg)](https://www.apple.com/macos/)

A native C++ X-Plane plugin that enables direct keyboard input for the ZIBO 737 Flight Management Computer (FMC), providing seamless keyboard-to-FMC interaction with visual feedback and dual FMC support.

## ✨ Features

- ⌨️ **Direct Keyboard Input**: Type directly into ZIBO 737 FMC using your keyboard
- 👨‍✈️ **Dual FMC Support**: Switch between Captain and First Officer FMC positions
- 🎯 **Visual Indicators**: Real-time display showing active FMC position and status
- 🚁 **Aircraft Detection**: Automatically enables only when ZIBO 737 is loaded
- 🔧 **Custom Commands**: X-Plane commands for easy key binding and joystick integration
- 🚀 **Native Performance**: C++ implementation for optimal performance and stability

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
| / | Slash | Forward slash |
| . | Period | Decimal point |
| - | Minus | Minus sign |

### Custom Commands

The plugin provides two X-Plane commands that you can bind to keyboard shortcuts or joystick buttons:

- `3370Tech/ZIBO_Keyboard/Toggle_Keyboard_Input_Captain` - Toggle Captain FMC keyboard input
- `3370Tech/ZIBO_Keyboard/Toggle_Keyboard_Input_FO` - Toggle First Officer FMC keyboard input

### Visual Indicators

When keyboard input is active, you will see:
- **Top-left corner**: Red text showing which FMC is active ("CAP Keyboard Input Enabled" or "FO Keyboard Input Enabled")
- **Near cursor**: Small indicator showing "CAP" or "FO" following your mouse

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
- Search for commands starting with "3370Tech" in Settings > Keyboard

## 🤝 Contributing

Issues and Pull Requests are welcome!

1. Fork this repository
2. Create feature branch (`git checkout -b feature/AmazingFeature`)
3. Commit changes (`git commit -m 'Add some AmazingFeature'`)
4. Push branch (`git push origin feature/AmazingFeature`)
5. Open Pull Request

## 📝 Changelog

### v2.0.0 (C++ Plugin)
- ✨ Complete rewrite as native C++ X-Plane plugin
- 🚀 Improved performance and stability
- 🎯 Enhanced visual feedback system
- 🔧 Better integration with X-Plane plugin system
- 📱 Universal binary support for all Mac architectures

### v1.0.0 (Original Lua)
- 🎮 Initial Lua script implementation
- ⌨️ Basic keyboard input functionality
- 👥 Support for Captain and FO positions

## 📄 License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## 🙏 Acknowledgments

- **Original Lua Script**: 3370Tech and Justin
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