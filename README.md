# Universal FMC Keyboard Input Plugin

[![Version](https://img.shields.io/badge/version-2.0.0-brightgreen.svg)](VERSION)
[![License](https://img.shields.io/badge/license-MIT-blue.svg)](LICENSE)
[![Platform](https://img.shields.io/badge/platform-X--Plane%2011%2B-orange.svg)](https://www.x-plane.com/)
[![macOS](https://img.shields.io/badge/macOS-10.15%2B-green.svg)](https://www.apple.com/macos/)

**Universal FMC Keyboard Input** is a high-performance native C++ X-Plane plugin that revolutionizes flight management computer and GPS interaction across multiple aircraft types. This plugin eliminates the tedious point-and-click operation by enabling direct keyboard input, allowing pilots to type flight plans, waypoints, and navigation data directly into FMC/GPS systems using their physical keyboard.

The plugin features intelligent multi-aircraft detection, dual FMC support for both pilot positions, real-time visual feedback, and seamless integration with X-Plane's command system. Built from the ground up in C++ for optimal performance and stability, it provides a more realistic and efficient flight simulation experience across a wide range of aircraft.

## ✨ Features

### 🛩️ **Multi-Aircraft Support**
- **ZIBO 737**: Full dual FMC support with intelligent +/- state management
- **Default 737**: Native dual FMS support (FMS/FMS2 systems)
- **Default A330**: Complete dual FMS integration
- **Default SR22**: GPS GCU keyboard input support

### ⌨️ **Advanced Input System**
- **Direct Keyboard Input**: Type directly into FMC/GPS systems using your keyboard
- **Intelligent Key Mapping**: Automatic adaptation to each aircraft's command structure
- **Smart Character Handling**: Proper +/- support where available, automatic key filtering
- **Multi-System Support**: FMC, FMS, and GPS systems all supported

### 👨‍✈️ **Professional Flight Operations**
- **Dual FMC/FMS Support**: Switch between Captain and First Officer positions
- **Aircraft Auto-Detection**: Automatically detects and adapts to current aircraft type
- **Visual Indicators**: Real-time display showing active system and position
- **Seamless Switching**: Hot-swap between different aircraft without restart

### 🚀 **Technical Excellence**
- **Native Performance**: C++ implementation for optimal performance and stability
- **Cross-Platform**: Universal support for macOS (Intel + Apple Silicon), Windows, and Linux
- **Modern SDK**: X-Plane 11/12 compatible with XPLM 300+ features
- **Custom Commands**: X-Plane commands for easy key binding and joystick integration

## 🎯 How It Works

The plugin seamlessly integrates with X-Plane's input system to provide natural keyboard interaction across multiple aircraft:

### 🔍 **Intelligent Aircraft Detection**
1. **Automatic Recognition**: Monitors aircraft ICAO codes and command availability
2. **System Identification**: Distinguishes between ZIBO vs default aircraft automatically
3. **Real-time Adaptation**: Switches command mappings when you change aircraft

### ⚡ **Universal Input Processing**
1. **Smart Key Mapping**: Automatically adapts key names for each aircraft type
2. **System-Specific Commands**: Routes input to correct FMC/FMS/GPS system
3. **Position Awareness**: Handles Captain/FO distinction where applicable

### 🎮 **User Experience Flow**
1. **Load Any Supported Aircraft**: ZIBO 737, Default 737/A330, or SR22
2. **Toggle Activation**: Use keyboard shortcuts for Captain or First Officer position
3. **Direct Input**: Type naturally - characters appear in the active system
4. **Visual Feedback**: Status indicator shows current aircraft and position
5. **Seamless Operation**: Switch between aircraft without plugin restart

This universal approach eliminates tedious mouse clicking across all supported aircraft types, making flight planning faster and more realistic regardless of your aircraft choice.

## 🛠️ System Requirements

- **Operating System**: macOS 10.15 or higher, Windows 10+, or Linux
- **X-Plane**: X-Plane 11 or X-Plane 12 (supports XPLM 410+ SDK)
- **Supported Aircraft**:
  - **ZIBO 737** (ICAO: B738) - Full dual FMC support
  - **Default Boeing 737** (ICAO: B738) - Dual FMS support
  - **Default Airbus A330** (ICAO: A330) - Dual FMS support
  - **Default Cirrus SR22** (ICAO: SR22) - GPS GCU support
- **Development Tools** (for building from source): 
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

The plugin automatically adapts key mappings based on the detected aircraft:

| Keyboard Key | ZIBO 737 | Default 737/A330 | SR22 GPS | Description |
|--------------|----------|------------------|----------|-------------|
| 0-9 | Numbers 0-9 | numbers 0-9 | Numbers 0-9 | Numeric input |
| A-Z | Letters A-Z | letters a-z | Letters A-Z | Alphabetic input |
| Backspace | clr | clear | clr | Clear last character |
| Space | SP | space | spc | Space character |
| Delete | del | delete | bksp | Delete/Backspace function |
| Enter | ent | enter | ent | Enter/Execute |
| / (main or numpad) | slash | slash | *Not supported* | Forward slash |
| . | period | period | dot | Decimal point |
| - (main or numpad) | minus* | minus* | *Not supported* | Minus sign |
| Shift + = | plus* | plus* | *Not supported* | Plus sign |
| Numpad + | plus* | plus* | *Not supported* | Plus sign |

**Key Mapping Notes:**
- *Plus/Minus*: ZIBO and default aircraft support intelligent +/- state management
- *SR22 Limitations*: GPS systems don't support slash or +/- operations
- *Case Sensitivity*: Automatically handled per aircraft (ZIBO/SR22: uppercase, Default: lowercase)
- *Command Routing*: Automatically routes to correct FMC/FMS/GPS system based on aircraft

### Custom Commands

The plugin provides universal X-Plane commands that work across all supported aircraft:

- `Universal/FMC_Keyboard/Toggle_Keyboard_Input_Captain` - Toggle Captain FMC/FMS/GPS keyboard input
- `Universal/FMC_Keyboard/Toggle_Keyboard_Input_FO` - Toggle First Officer FMC/FMS keyboard input

**Command Behavior by Aircraft:**
- **Dual System Aircraft** (ZIBO 737, Default 737/A330): Commands toggle Captain vs First Officer systems independently
- **Single System Aircraft** (SR22): Both commands control the same GPS system for convenience

### Visual Indicators

The plugin provides intelligent visual feedback that adapts to each aircraft:

**Status Window Display:**
- **Dual FMC Aircraft** (ZIBO 737): Shows "KB:CAP" or "KB:FO" indicating active FMC position  
- **Dual FMS Aircraft** (Default 737/A330): Shows "KB:CAP" or "KB:FO" indicating active FMS position
- **Single GPS Aircraft** (SR22): Shows "KB:SR22" indicating GPS system is active

**Visual Features:**
- **Bottom-right corner**: Floating status window with bright green text
- **Real-time feedback**: Appears only when input is enabled, automatically hides when disabled
- **Aircraft-adaptive**: Only displays when a supported aircraft is loaded and plugin is active
- **Position-aware**: Clear indication of which pilot position or system is currently active

### Usage Instructions

1. **Load Any Supported Aircraft**: ZIBO 737, Default 737, Default A330, or SR22
2. **Configure Keybindings**: Assign keyboard shortcuts to the plugin commands in X-Plane Settings > Keyboard:
   - Search for commands starting with "Universal/FMC_Keyboard/"
3. **Activate Input**: Press your assigned key to toggle keyboard input
   - Captain command: Activates Captain-side system
   - FO command: Activates First Officer system (or same system for SR22)
4. **Type Naturally**: When enabled, characters appear directly in the active FMC/FMS/GPS
5. **Monitor Status**: Check the green status indicator to confirm active position
6. **Toggle Control**: Switch between positions or turn off input as needed

**Multi-Aircraft Workflow:**
- Load different aircraft without restarting X-Plane
- Plugin automatically adapts to each aircraft's system
- Keybindings remain consistent across all aircraft types

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

This plugin operates through a sophisticated multi-aircraft architecture:

### 🔍 **Multi-Aircraft Detection System**
1. **ICAO Monitoring**: Monitors `sim/aircraft/view/acf_ICAO` dataref for aircraft identification
2. **Command Probing**: Tests for aircraft-specific commands to distinguish variants (ZIBO vs Default)
3. **Configuration Mapping**: Automatically selects appropriate command sets and key mappings
4. **Real-time Updates**: Continuously adapts when aircraft changes

### ⚡ **Universal Command Translation**
| Aircraft Type | Command Format | Example |
|---------------|----------------|---------|
| ZIBO 737 | `laminar/B738/button/fmc%d_%s` | `laminar/B738/button/fmc1_A` |
| Default 737 | `sim/FMS/key_%s` / `sim/FMS2/key_%s` | `sim/FMS/key_a` |
| Default A330 | `sim/FMS/key_%s` / `sim/FMS2/key_%s` | `sim/FMS2/key_a` |
| SR22 GPS | `sim/GPS/gcu478/%s` | `sim/GPS/gcu478/A` |

### 🎯 **Intelligent Key Processing**
1. **Key Interception**: Registers a key sniffer callback to capture keyboard input
2. **Aircraft-Specific Mapping**: Dynamically converts key names based on detected aircraft
3. **System Routing**: Routes commands to correct FMC/FMS/GPS based on pilot position
4. **Smart Filtering**: Ignores modifier combinations and unsupported keys per aircraft

### 🎨 **Adaptive Visual Feedback**
- **Position-Aware Display**: Shows CAP/FO for dual systems, aircraft type for single systems
- **Aircraft-Specific Indicators**: Displays relevant information based on detected aircraft
- **Real-time Updates**: Status changes immediately when switching aircraft or positions

### 🔧 **Special Features**
- **Plus/Minus State Management**: Intelligent +/- handling for ZIBO and default aircraft
- **Case Sensitivity Handling**: Automatic uppercase/lowercase conversion per aircraft requirements
- **Dual Input Support**: Both main keyboard and numpad keys supported
- **Modifier Key Safety**: Prevents interference with X-Plane shortcuts

## 📝 Changelog

### v2.0.0 (Current) - Universal Multi-Aircraft Support
- 🛩️ **MAJOR UPDATE**: Multi-aircraft support across 4 aircraft types
- ✈️ **ZIBO 737**: Full dual FMC support with intelligent +/- state management
- ✈️ **Default 737**: Native dual FMS support (FMS/FMS2 systems)
- ✈️ **Default A330**: Complete dual FMS integration  
- ✈️ **SR22**: GPS GCU keyboard input support
- 🔍 **Intelligent Detection**: Automatic aircraft type recognition and adaptation
- 🎯 **Universal Commands**: New universal command structure works across all aircraft
- 🔧 **Smart Key Mapping**: Automatic key conversion based on aircraft requirements
- 📊 **Adaptive UI**: Status indicators show relevant information per aircraft type
- 🚀 **Seamless Operation**: Switch between aircraft without plugin restart

### v1.0.8 (Previous)
- 🚀 Stable C++ implementation with enhanced reliability
- 🎯 Advanced visual feedback system with floating status indicators
- 🔧 Comprehensive cross-platform support (macOS Universal, Windows x64, Linux x64)
- 🎛️ Intelligent key mapping with special character support
- 📱 Modern X-Plane SDK integration (XPLM 300+)

### v1.0.0 (Original Lua)
- 🎮 Initial Lua script implementation
- ⌨️ Basic keyboard input functionality for ZIBO 737 only
- 👥 Support for Captain and FO positions

## 📄 License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## 🙏 Acknowledgments

- **ZIBO 737**: Outstanding work by the Zibo aircraft development team
- **Laminar Research**: X-Plane simulator and comprehensive SDK development tools
- **Default Aircraft**: High-quality default 737, A330, and SR22 implementations
- **X-Plane Community**: Beta testers, contributors, and feedback providers
- **Multi-Aircraft Testing**: Pilots who helped test across different aircraft types

## 💬 Support

For issues, suggestions, or questions:

1. **Check Compatibility**: Ensure you're using a supported aircraft (ZIBO 737, Default 737/A330/SR22)
2. **Review Documentation**: Check the troubleshooting section and key mappings above
3. **Check Logs**: Review X-Plane's Log.txt for error messages starting with "Universal FMC Keyboard"
4. **Report Issues**: Create an issue on the project repository with:
   - X-Plane version and operating system
   - Aircraft type and ICAO code
   - Detailed description of the problem
   - Relevant log file entries

**Getting Help:**
- Search existing issues for similar problems
- Include system specifications and aircraft details
- Describe expected vs actual behavior clearly

---

## 🎯 **Multi-Aircraft Compatibility**

**✅ Fully Supported Aircraft:**
- **ZIBO 737** (ICAO: B738) - Complete dual FMC functionality with +/- support
- **Default Boeing 737** (ICAO: B738) - Dual FMS support with +/- functionality  
- **Default Airbus A330** (ICAO: A330) - Full dual FMS integration
- **Default Cirrus SR22** (ICAO: SR22) - GPS GCU input (no +/- or slash)

**🔄 Automatic Detection:** The plugin automatically detects your aircraft and adapts its functionality accordingly. Simply load any supported aircraft and the plugin will configure itself for optimal compatibility.