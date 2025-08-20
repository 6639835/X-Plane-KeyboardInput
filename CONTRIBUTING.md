# Contributing to ZIBO Keyboard Input Plugin

Thank you for your interest in contributing to the ZIBO Keyboard Input Plugin! This document provides guidelines and information for contributors.

## 📋 Code of Conduct

This project and everyone participating in it is governed by our [Code of Conduct](CODE_OF_CONDUCT.md). By participating, you are expected to uphold this code. Please report unacceptable behavior through the channels outlined in the Code of Conduct.

## 🤝 How to Contribute

We welcome contributions in various forms:
- **Bug reports** and feature requests
- **Code contributions** via pull requests
- **Documentation** improvements
- **Testing** and feedback on different platforms
- **Translations** (if applicable in the future)

## 🚀 Getting Started

### Development Environment Setup

1. **Fork and clone the repository**
   ```bash
   git clone https://github.com/6639835/ZIBOKeyboardInput.git
   cd ZIBOKeyboardInput
   ```

2. **Install development dependencies**

   **macOS:**
   ```bash
   # Install Xcode Command Line Tools
   xcode-select --install
   
   # Install CMake via Homebrew
   brew install cmake
   ```

   **Windows:**
   - Install Visual Studio 2019 or later (Community edition is sufficient)
   - Install CMake from cmake.org
   - Ensure Git is available

   **Linux:**
   ```bash
   # Ubuntu/Debian
   sudo apt update
   sudo apt install build-essential cmake libgl1-mesa-dev git
   
   # CentOS/RHEL/Fedora  
   sudo dnf install gcc-c++ cmake mesa-libGL-devel git
   ```

3. **Set up X-Plane SDK**
   - Download the latest X-Plane SDK from [developer.x-plane.com](https://developer.x-plane.com/)
   - Extract to project root as `XPLM-SDK/`
   - Verify directory structure matches the one shown in `BUILD_INSTRUCTIONS.md`

4. **Build the project**
   ```bash
   # Quick build
   ./build.sh
   
   # Or manual build
   mkdir build && cd build
   cmake -DCMAKE_BUILD_TYPE=Debug ..
   cmake --build . --config Debug
   ```

5. **Test your build**
   - Install the plugin using `./install.sh`
   - Load X-Plane with ZIBO 737
   - Test basic functionality

## 📝 Code Style Guidelines

### C++ Coding Standards

- **Formatting**: Use consistent indentation (4 spaces, no tabs)
- **Naming conventions**:
  - Variables: `snake_case` (e.g., `g_fmc_side`, `aircraft_icao`)
  - Functions: `snake_case` (e.g., `draw_callback`, `key_sniffer`)
  - Constants: `UPPER_CASE` (e.g., `PLUGIN_NAME`, `MAX_BUFFER_SIZE`)
  - Classes/Structs: `PascalCase` (if any are added)

- **Comments**: 
  - Use `//` for single-line comments
  - Use `/* */` for multi-line comments
  - Document complex logic and X-Plane specific interactions
  - Include function headers for non-trivial functions

- **File structure**:
  - Platform definitions at the top
  - XPLM includes after platform definitions
  - Standard library includes
  - Global variables clearly marked
  - Helper functions before main plugin functions

### Example Code Style

```cpp
// Good example
static int g_toggled = 0;           // Global state: 0 = disabled, 1 = enabled
static XPLMDataRef g_icao_dataref = NULL;

/**
 * Checks if the current aircraft is ZIBO 737
 * Returns 1 if ZIBO is loaded, 0 otherwise
 */
static int is_zibo_loaded() {
    if (!g_icao_dataref) {
        return 0;
    }
    
    char aircraft_icao[40];
    XPLMGetDatab(g_icao_dataref, aircraft_icao, 0, sizeof(aircraft_icao));
    
    return (strcmp(aircraft_icao, "B738") == 0);
}
```

### X-Plane Plugin Specific Guidelines

- **Memory management**: Always check for NULL pointers when working with datarefs
- **Error handling**: Use defensive programming - validate all X-Plane SDK inputs
- **Resource cleanup**: Properly unregister callbacks and release resources in plugin disable
- **Cross-platform compatibility**: Test `#ifdef` blocks for APL/IBM/LIN
- **Performance**: Minimize work in flight loop callbacks
- **SDK usage**: Prefer newer XPLM APIs when available, maintain backward compatibility

## 🐛 Bug Reports

When reporting bugs, please include:

### Required Information
- **X-Plane version** (11 or 12, exact build number)
- **Operating system** (macOS version, Windows version, Linux distribution)
- **ZIBO 737 version** (if relevant)
- **Plugin version** (from VERSION file)
- **Steps to reproduce** the issue
- **Expected vs actual behavior**

### Additional Helpful Information
- **X-Plane Log.txt** (especially relevant sections)
- **Screenshots or videos** if the issue is visual
- **Other plugins** loaded that might conflict
- **System specifications** (especially for performance issues)

### Example Bug Report

```markdown
**Environment:**
- X-Plane: 12.1.0 (build 12345)
- OS: macOS 13.5.2 (M2 MacBook Pro)
- ZIBO: 3.50.17
- Plugin: v1.0.7

**Issue:**
Keyboard input stops working after switching from Captain to FO FMC.

**Steps to Reproduce:**
1. Load ZIBO 737
2. Enable Captain FMC keyboard input
3. Type some text (works correctly)
4. Toggle to FO FMC input
5. Try typing - nothing appears

**Expected:** FO FMC should receive keyboard input
**Actual:** No input is registered

**Additional Info:**
- Visual indicator shows "KB:FO" correctly
- Log.txt shows no errors
- Restarting X-Plane fixes the issue temporarily
```

## 🔧 Pull Request Process

### Before Submitting

1. **Test thoroughly**:
   - Build on your target platform
   - Test with ZIBO 737 in X-Plane
   - Verify both Captain and FO FMC functionality
   - Test key edge cases (special characters, rapid input, etc.)

2. **Check code quality**:
   - Follow the coding standards above
   - Add comments for complex logic
   - Ensure no memory leaks or resource leaks
   - Verify cross-platform compatibility

3. **Update documentation**:
   - Update README.md if adding features
   - Add/update comments in code
   - Update VERSION file if applicable

### Pull Request Guidelines

1. **Create a focused PR**: One feature or bug fix per pull request
2. **Write a clear title**: Describe what the PR does in one line
3. **Provide detailed description**:
   ```markdown
   ## Changes Made
   - Fixed keyboard input state persistence across FMC switches
   - Added validation for dataref access before usage
   
   ## Testing Performed
   - Tested on macOS 13.5 with X-Plane 12.1.0
   - Verified both Captain and FO FMC switching
   - Tested special characters (+, -, /, .)
   
   ## Breaking Changes
   None
   
   ## Related Issues
   Fixes #123
   ```

4. **Keep commits clean**: Use meaningful commit messages
   ```bash
   # Good commit messages
   git commit -m "Fix: Resolve FMC state persistence across position switches"
   git commit -m "Add: Input validation for ICAO dataref access"
   git commit -m "Docs: Update README with new key mapping information"
   ```

### Review Process

- All PRs require review before merging
- Reviewers will check for code quality, functionality, and compatibility
- Address feedback promptly and professionally
- Be prepared to make changes based on review comments

## 🧪 Testing Guidelines

### Manual Testing Checklist

Before submitting contributions, please verify:

- [ ] **Build verification**: Plugin builds successfully on target platform(s)
- [ ] **Installation**: Plugin installs correctly via install script
- [ ] **Aircraft detection**: Plugin only activates with ZIBO 737 (ICAO: B738)
- [ ] **Command registration**: Custom commands appear in X-Plane keyboard settings
- [ ] **Basic functionality**: 
  - [ ] Captain FMC keyboard input works
  - [ ] First Officer FMC keyboard input works
  - [ ] Toggle between positions works correctly
  - [ ] Visual indicators display properly
- [ ] **Key mapping**: All documented keys function correctly
  - [ ] Numbers (0-9)
  - [ ] Letters (A-Z)
  - [ ] Special characters (/, ., -, +)
  - [ ] Backspace (CLR)
  - [ ] Space (SP)
  - [ ] Delete (DEL)
- [ ] **Edge cases**:
  - [ ] Rapid key input
  - [ ] Special character combinations
  - [ ] Plugin disable/re-enable
  - [ ] Aircraft change scenarios

### Platform Testing

If you have access to multiple platforms, please test on:
- **macOS**: Both Intel and Apple Silicon if possible
- **Windows**: Windows 10/11 with X-Plane 11 and 12
- **Linux**: At least one major distribution

### Performance Testing

- Monitor X-Plane frame rate impact
- Test with other plugins loaded
- Verify no memory leaks during extended use

## 📋 Feature Requests

When proposing new features:

1. **Check existing issues** to avoid duplicates
2. **Describe the use case** clearly
3. **Explain the expected behavior**
4. **Consider implementation complexity**
5. **Think about backward compatibility**

### Good Feature Request Example

```markdown
**Feature:** Support for additional ZIBO variants

**Use Case:** 
Some users fly ZIBO 737 MAX variants that use different ICAO codes.

**Proposed Solution:**
Extend aircraft detection to recognize additional ICAO codes like B38M, B39M.

**Implementation Notes:**
- Modify `is_zibo_loaded()` function to check multiple ICAO codes
- Maintain compatibility with existing B738 detection
- Consider configuration file for user-defined aircraft codes

**Backward Compatibility:**
Fully backward compatible - existing B738 functionality unchanged.
```

## 🔒 Security Guidelines

While this is a flight simulator plugin, please still follow security best practices:

- **Input validation**: Always validate external inputs (datarefs, file paths)
- **Buffer safety**: Use safe string functions (`strncpy`, `snprintf`)
- **Memory safety**: Check for NULL pointers and array bounds
- **Resource management**: Properly clean up allocated resources

## 📚 Documentation Guidelines

When contributing documentation:

- **Be clear and concise**: Write for users of all experience levels
- **Use consistent formatting**: Follow existing markdown style
- **Include examples**: Show concrete examples where helpful
- **Keep it updated**: Ensure documentation matches current functionality
- **Consider non-native English speakers**: Use simple, clear language

## 🌍 Platform-Specific Considerations

### macOS
- Test on both Intel and Apple Silicon Macs when possible
- Be aware of Gatekeeper and code signing requirements
- Consider macOS version compatibility (minimum 10.15)

### Windows
- Test on both X-Plane 11 and 12 if possible
- Be aware of Windows Defender potentially flagging unsigned plugins
- Consider Visual Studio version compatibility

### Linux
- Test on major distributions (Ubuntu, CentOS/RHEL, Arch)
- Be aware of library dependency differences
- Consider both X11 and Wayland environments

## 📞 Getting Help

If you need help with development:

1. **Check existing documentation**: README.md, BUILD_INSTRUCTIONS.md
2. **Search existing issues**: Someone may have faced the same problem
3. **X-Plane SDK documentation**: [developer.x-plane.com](https://developer.x-plane.com/)
4. **Create a discussion**: Use GitHub Discussions for questions
5. **Join the community**: Participate in X-Plane development forums

## 📄 License

By contributing to this project, you agree that your contributions will be licensed under the same MIT License that covers the project. See the [LICENSE](LICENSE) file for details.

## 🙏 Recognition

Contributors are recognized in:
- Git commit history
- Release notes for significant contributions
- README.md acknowledgments section

Thank you for helping make the ZIBO Keyboard Input Plugin better for the X-Plane community!

---

**Questions?** Don't hesitate to ask by creating an issue or discussion. We're here to help!
