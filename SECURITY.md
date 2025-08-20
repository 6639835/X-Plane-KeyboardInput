# Security Policy

## Overview

The ZIBO Keyboard Input Plugin is designed with security and user safety as top priorities. As an X-Plane plugin that handles keyboard input and integrates with flight simulation systems, we take security seriously to protect users' simulation environments and personal data.

## Supported Versions

We actively maintain and provide security updates for the following versions:

| Version | Supported          | Status | Notes |
| ------- | ------------------ | ------ | ----- |
| 1.0.7   | :white_check_mark: | Current stable release | Recommended |
| 1.0.6   | :white_check_mark: | Previous stable | Security updates only |
| 1.0.5   | :warning: | Legacy | Critical security fixes only |
| < 1.0.5 | :x: | Unsupported | Please upgrade immediately |

**Recommendation**: Always use the latest stable version (currently 1.0.7) for the best security posture and latest features.

## Security Considerations

### Plugin Design Security

**Input Validation**: The plugin implements strict input validation and filtering:
- Only captures relevant keyboard input when explicitly enabled
- Validates all dataref access before usage
- Implements bounds checking for all string operations
- Uses safe string functions (`strncpy`, `snprintf`) to prevent buffer overflows

**Minimal Privileges**: The plugin operates with minimal necessary privileges:
- Only accesses ZIBO 737-specific datarefs and commands
- No file system access beyond X-Plane's plugin directory
- No network communication capabilities
- No access to sensitive system resources

**Memory Safety**: Implemented defensive programming practices:
- All pointers checked for NULL before dereferencing
- Proper resource cleanup on plugin disable
- No dynamic memory allocation to prevent leaks
- Stack-based buffers with defined limits

### X-Plane Integration Security

**Aircraft Detection**: Secure aircraft identification:
- Only activates with verified ZIBO 737 aircraft (ICAO: B738)
- Continuously validates aircraft state
- Automatically disables if aircraft changes

**Command Safety**: Safe X-Plane command execution:
- Only sends commands to verified ZIBO FMC systems
- No arbitrary command execution
- Commands are validated before execution

## Security Best Practices for Users

### Installation Security

1. **Download from Official Sources**:
   - Only download from the official GitHub repository: https://github.com/6639835/ZIBOKeyboardInput
   - Verify the repository is legitimate before downloading
   - Check the commit history and contributor information

2. **Build from Source** (Recommended):
   - Build the plugin yourself using the provided build scripts
   - Verify the source code before compilation
   - Use official X-Plane SDK from developer.x-plane.com

3. **File Integrity**:
   - Check file sizes and dates after installation
   - Verify the plugin loads without errors in X-Plane
   - Monitor X-Plane's Log.txt for any suspicious activity

### Usage Security

1. **Enable Only When Needed**:
   - Only enable keyboard input when actively using the FMC
   - Disable the plugin when not needed to minimize attack surface
   - Be aware of which FMC position is active (visual indicators)

2. **Monitor Behavior**:
   - Watch for unexpected FMC input or behavior
   - Check that keyboard shortcuts still work normally in X-Plane
   - Report any unusual plugin behavior immediately

3. **System Security**:
   - Keep X-Plane updated to the latest version
   - Use updated operating systems with current security patches
   - Run antivirus software if on Windows platforms

## Reporting a Vulnerability

We take security vulnerabilities seriously and appreciate responsible disclosure. If you discover a security issue, please follow these steps:

### Reporting Process

1. **Do NOT** create public GitHub issues for security vulnerabilities
2. **Contact us directly** using one of these methods:
   - **GitHub Security Advisory**: Use GitHub's private vulnerability reporting feature
   - **Email**: Contact the maintainer directly (see repository for contact information)
   - **Direct Message**: Through GitHub private messaging

### Information to Include

When reporting a vulnerability, please provide:

- **Vulnerability Description**: Clear description of the security issue
- **Impact Assessment**: Potential impact and severity
- **Reproduction Steps**: Detailed steps to reproduce the vulnerability
- **Environment Details**:
  - X-Plane version and platform
  - Operating system and version
  - Plugin version affected
  - ZIBO aircraft version (if relevant)
- **Proof of Concept**: Code or screenshots demonstrating the issue (if safe to share)
- **Suggested Fix**: If you have ideas for mitigation or fixes

### Response Timeline

We are committed to responding to security reports promptly:

- **Initial Response**: Within 48 hours of report
- **Assessment**: Within 7 days for impact and severity evaluation
- **Fix Development**: Timeline depends on complexity, typically 1-4 weeks
- **Public Disclosure**: After fix is available and users have time to update

### Responsible Disclosure

We follow responsible disclosure practices:

- We will work with you to understand and validate the vulnerability
- We will develop and test fixes before public release
- We will coordinate disclosure timing with the reporter
- We will provide credit to the reporter in release notes (if desired)
- We may request a CVE ID for significant vulnerabilities

## Security Response Process

### For Confirmed Vulnerabilities

1. **Immediate Assessment**: Evaluate severity and impact
2. **User Communication**: Notify users of potential risks if critical
3. **Fix Development**: Develop and test security patches
4. **Testing**: Thorough testing across all supported platforms
5. **Release**: Expedited release for security fixes
6. **Documentation**: Update security documentation and advisories

### Severity Classification

We use the following severity levels:

- **Critical**: Remote code execution, privilege escalation, or system compromise
- **High**: Significant data exposure or unauthorized access
- **Medium**: Moderate security risk with limited impact
- **Low**: Minor security improvements or hardening

## Build and Distribution Security

### Source Code Integrity

- All source code is publicly available and auditable
- Changes are tracked through version control with signed commits
- Build process is documented and reproducible
- No obfuscated or binary-only components

### Build Environment

- Official builds use clean, controlled build environments
- All dependencies are from official sources (X-Plane SDK, system libraries)
- Build scripts are auditable and included in the repository
- Cross-platform builds follow identical security practices

### Distribution Channels

- **Primary**: Official GitHub repository releases
- **Verification**: Release checksums and signatures provided
- **Mirrors**: We do not endorse third-party distribution sites

## Security Updates

### Update Policy

- Security updates are prioritized and released as quickly as possible
- Users are notified through GitHub releases and repository announcements
- Critical security updates may skip normal release cycles
- Update instructions are provided with each security release

### Automatic Updates

The plugin does not include automatic update capabilities for security reasons:
- Prevents unauthorized code execution
- Allows users to review changes before updating
- Maintains user control over their X-Plane environment

## Contact Information

### Security Team

For security-related inquiries:
- **Repository**: https://github.com/6639835/ZIBOKeyboardInput
- **Security Issues**: Use GitHub's private vulnerability reporting
- **General Security Questions**: Create a discussion thread

### Response Team

Our security response team includes:
- Project maintainers with X-Plane development expertise
- Community members with security backgrounds
- External security researchers (when needed)

## Security Resources

### For Developers

- [X-Plane SDK Security Guidelines](https://developer.x-plane.com/)
- [C++ Security Best Practices](https://wiki.sei.cmu.edu/confluence/display/cplusplus)
- [OWASP Secure Coding Practices](https://owasp.org/www-project-secure-coding-practices-quick-reference-guide/)

### For Users

- [X-Plane Plugin Security](https://www.x-plane.com/)
- Plugin installation and verification guides
- System security recommendations for flight simulation

## Changelog

### Security Improvements by Version

**v1.0.7**:
- Enhanced input validation and bounds checking
- Improved memory safety and resource management
- Strengthened aircraft detection validation
- Added comprehensive error handling

**v1.0.6**:
- Initial security hardening implementation
- Basic input sanitization
- Memory leak prevention

## Acknowledgments

We thank the security research community and X-Plane development community for their ongoing efforts to improve plugin security. Special thanks to contributors who have reported security issues responsibly.

---

**Last Updated**: August 2024  
**Version**: 1.1  
**Next Review**: December 2024

For the most current security information, always refer to the latest version of this document in the official repository.