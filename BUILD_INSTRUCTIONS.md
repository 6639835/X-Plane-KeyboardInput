# Universal FMC Keyboard Input Plugin - Build Instructions

This document provides detailed instructions for building the Universal FMC Keyboard Input plugin from source code. This plugin supports multiple aircraft types including ZIBO 737, Default 737/A330, and SR22.

## 🛩️ Multi-Aircraft Support

**Version 2.0.0 Major Update**: The plugin now supports multiple aircraft types with automatic detection and adaptation:

- **ZIBO 737** (ICAO: B738) - Full dual FMC support with intelligent +/- state management
- **Default Boeing 737** (ICAO: B738) - Dual FMS support (FMS/FMS2 systems)  
- **Default Airbus A330** (ICAO: A330) - Complete dual FMS integration
- **Default Cirrus SR22** (ICAO: SR22) - GPS GCU keyboard input support

The build process creates a single universal plugin that automatically detects and adapts to each aircraft type at runtime.

## Prerequisites

### Required Tools

#### macOS
- **Xcode Command Line Tools**: Required for compilers and build tools
- **CMake**: Version 3.16 or higher
- **X-Plane SDK**: Latest version from developer.x-plane.com

#### Windows
- **Visual Studio**: 2019 or later (Community edition is sufficient)
- **CMake**: Version 3.16 or higher
- **X-Plane SDK**: Latest version from developer.x-plane.com

#### Linux
- **GCC**: Version 7 or higher
- **CMake**: Version 3.16 or higher
- **Development libraries**: OpenGL development packages
- **X-Plane SDK**: Latest version from developer.x-plane.com

### Installing Prerequisites

#### macOS Setup
```bash
# Install Xcode Command Line Tools
xcode-select --install

# Install CMake using Homebrew
brew install cmake

# Verify installation
cmake --version
```

#### Windows Setup
1. Download and install Visual Studio 2019 or later
2. Download CMake from cmake.org and install
3. Verify installation in Command Prompt:
   ```cmd
   cmake --version
   ```

#### Linux Setup
```bash
# Ubuntu/Debian
sudo apt update
sudo apt install build-essential cmake libgl1-mesa-dev

# CentOS/RHEL/Fedora
sudo dnf install gcc-c++ cmake mesa-libGL-devel

# Verify installation
cmake --version
```

## X-Plane SDK Setup

### Download SDK
1. Go to [developer.x-plane.com](https://developer.x-plane.com/)
2. Download the latest X-Plane SDK
3. Extract the SDK archive

### Install SDK to Project
1. Extract the SDK to your project directory as `XPLM-SDK/`
2. Verify the directory structure:
   ```
   ZIBOKeyboardInput/
   └── XPLM-SDK/
       ├── CHeaders/
       │   ├── XPLM/
       │   └── Widgets/
       └── Libraries/
           ├── Mac/
           ├── Win/
           └── Lin/
   ```

## Building the Plugin

### Automated Build (Recommended)

The project includes automated build scripts for easy compilation:

#### macOS/Linux
```bash
# Navigate to project directory
cd ZIBOKeyboardInput

# Build the plugin
./build.sh

# Clean and rebuild (if needed)
./build.sh clean
```

#### Windows
```cmd
REM Navigate to project directory
cd ZIBOKeyboardInput

REM Build the plugin
build.bat
```

### Manual Build

If you prefer manual control over the build process:

#### Step 1: Create Build Directory
```bash
mkdir build
cd build
```

#### Step 2: Configure with CMake
```bash
# Debug build
cmake -DCMAKE_BUILD_TYPE=Debug ..

# Release build (recommended for distribution)
cmake -DCMAKE_BUILD_TYPE=Release ..
```

#### Step 3: Compile
```bash
# Build the project
cmake --build . --config Release

# Or use platform-specific build tools
# macOS/Linux: make
# Windows: msbuild or Visual Studio
```

## Platform-Specific Build Details

### macOS Build Configuration
- **Target architectures**: x86_64 (Intel) and arm64 (Apple Silicon)
- **Output file**: `mac.xpl`
- **Minimum macOS version**: 10.15 (Catalina)
- **Frameworks**: XPLM.framework, XPWidgets.framework

### Windows Build Configuration
- **Target architecture**: x64
- **Output file**: `win.xpl`
- **Compiler**: MSVC or MinGW
- **Libraries**: XPLM_64.lib, XPWidgets_64.lib

### Linux Build Configuration
- **Target architecture**: x64
- **Output file**: `lin.xpl`
- **Compiler**: GCC
- **Libraries**: XPLM_64.so, XPWidgets_64.so

## Build Output

After a successful build, you will find:

- **macOS**: `build/mac.xpl`
- **Windows**: `build/win.xpl`
- **Linux**: `build/lin.xpl`

The build process creates a universal binary on macOS that supports both Intel and Apple Silicon Macs.

## Installation

### Automated Installation
```bash
# Install to default X-Plane location
./install.sh

# Install to specific X-Plane path
./install.sh "/path/to/X-Plane"
```

### Manual Installation
1. Create the plugin directory:
   ```
   X-Plane/Resources/plugins/ZIBOKeyboardInput/
   ```

2. Copy the appropriate plugin file:
   - macOS: Copy `mac.xpl`
   - Windows: Copy `win.xpl`
   - Linux: Copy `lin.xpl`

3. Restart X-Plane

## Troubleshooting

### Common Build Issues

#### SDK Not Found
```
Error: X-Plane SDK not found. Please ensure XPLM-SDK directory exists.
```
**Solution**: Verify the SDK is extracted to `XPLM-SDK/` in the project root

#### CMake Version Too Old
```
CMake Error: CMake 3.16 or higher is required
```
**Solution**: Update CMake to version 3.16 or higher

#### Missing Compiler
- **macOS**: Install Xcode Command Line Tools
- **Windows**: Install Visual Studio
- **Linux**: Install build-essential package

#### Linker Errors
**Symptoms**: Undefined symbols or missing libraries
**Solution**: 
1. Verify SDK directory structure
2. Check that platform-specific libraries exist
3. Ensure export files are present

### Build Verification

To verify your build:
1. Check that the output file exists and has the correct extension
2. Verify file size (should be several hundred KB)
3. On macOS, check architecture support:
   ```bash
   file build/mac.xpl
   # Should show: Mach-O 64-bit bundle x86_64 arm64
   ```

### Performance Tips

- Use Release build configuration for distribution
- Enable compiler optimizations
- Use the automated build scripts for consistent results

## Development Environment

### Recommended IDE Settings

#### Visual Studio Code
- Install C/C++ extension
- Configure CMake integration
- Set up debugging configuration

#### Xcode (macOS)
- Create Xcode project from CMake
- Configure scheme for debugging

#### Visual Studio (Windows)
- Open folder with CMake support
- Configure debugging settings

### Debugging

To debug the plugin:
1. Build with Debug configuration
2. Launch X-Plane with debugger attached
3. Load any supported aircraft (ZIBO 737, Default 737/A330, or SR22)
4. Enable plugin functionality and test multi-aircraft detection

## Contributing to Build System

When modifying the build system:
1. Test on all target platforms
2. Update build scripts accordingly
3. Verify SDK compatibility
4. Update documentation

## Advanced Build Options

### Custom SDK Path
```bash
cmake -DXPLM_SDK_PATH="/custom/path/to/sdk" ..
```

### Debug Symbols
```bash
cmake -DCMAKE_BUILD_TYPE=Debug ..
```

### Verbose Output
```bash
cmake --build . --config Release --verbose
```