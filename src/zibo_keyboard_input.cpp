// Platform definitions must be set before including any XPLM headers
#ifndef APL
#if defined(__APPLE__)
#define APL 1
#define IBM 0
#define LIN 0
#elif defined(_WIN32) || defined(_WIN64)
#define APL 0
#define IBM 1
#define LIN 0
#elif defined(__linux__)
#define APL 0
#define IBM 0
#define LIN 1
#endif
#endif

#define XPLM300 1  // Enable X-Plane 11+ APIs
#define XPLM301 1  // Enable window decoration features
#include "XPLMDefs.h"
#include "XPLMDisplay.h"
#include "XPLMGraphics.h"
#include "XPLMProcessing.h"
#include "XPLMDataAccess.h"
#include "XPLMMenus.h"
#include "XPLMUtilities.h"
#include "XPLMPlanes.h"
#include "XPLMPlugin.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <map>

// OpenGL headers not needed - using X-Plane SDK graphics functions only

#if IBM
#include <windows.h>
#endif

// Plugin information
#define PLUGIN_NAME "ZIBO Keyboard Input"
#define PLUGIN_SIG "justin.zibo.keyboard"
#define PLUGIN_DESC "Keyboard input handler for ZIBO 737 FMC"

// Global state variables
static int g_toggled = 0;           // 0 = disabled, 1 = enabled
static int g_fmc_side = 1;          // 1 = Captain, 2 = First Officer
static XPLMDataRef g_icao_dataref = NULL;
static XPLMCommandRef g_captain_command = NULL;
static XPLMCommandRef g_fo_command = NULL;
static XPLMWindowID g_status_window = NULL;

// Key mapping structure - maps virtual key codes to FMC button names
static std::map<int, const char*> g_key_mappings;

// Function prototypes
static int KeyCallback(char inChar, XPLMKeyFlags inFlags, char inVirtualKey, void* inRefcon);
static void DrawStatusWindow(XPLMWindowID inWindowID, void* inRefcon);
static int CaptainCommandHandler(XPLMCommandRef inCommand, XPLMCommandPhase inPhase, void* inRefcon);
static int FOCommandHandler(XPLMCommandRef inCommand, XPLMCommandPhase inPhase, void* inRefcon);
static void ToggleKeyboardInput(int side);
static bool IsZIBO737();
static void InitializeKeyMappings();
static void LogMessage(const char* message);
static void CreateStatusWindow();
static void UpdateStatusWindow();

// Initialize key mappings
static void InitializeKeyMappings()
{
    // Numbers (0-9)
    g_key_mappings[XPLM_VK_0] = "0";
    g_key_mappings[XPLM_VK_1] = "1";
    g_key_mappings[XPLM_VK_2] = "2";
    g_key_mappings[XPLM_VK_3] = "3";
    g_key_mappings[XPLM_VK_4] = "4";
    g_key_mappings[XPLM_VK_5] = "5";
    g_key_mappings[XPLM_VK_6] = "6";
    g_key_mappings[XPLM_VK_7] = "7";
    g_key_mappings[XPLM_VK_8] = "8";
    g_key_mappings[XPLM_VK_9] = "9";
    
    // Letters (A-Z)
    g_key_mappings[XPLM_VK_A] = "A";
    g_key_mappings[XPLM_VK_B] = "B";
    g_key_mappings[XPLM_VK_C] = "C";
    g_key_mappings[XPLM_VK_D] = "D";
    g_key_mappings[XPLM_VK_E] = "E";
    g_key_mappings[XPLM_VK_F] = "F";
    g_key_mappings[XPLM_VK_G] = "G";
    g_key_mappings[XPLM_VK_H] = "H";
    g_key_mappings[XPLM_VK_I] = "I";
    g_key_mappings[XPLM_VK_J] = "J";
    g_key_mappings[XPLM_VK_K] = "K";
    g_key_mappings[XPLM_VK_L] = "L";
    g_key_mappings[XPLM_VK_M] = "M";
    g_key_mappings[XPLM_VK_N] = "N";
    g_key_mappings[XPLM_VK_O] = "O";
    g_key_mappings[XPLM_VK_P] = "P";
    g_key_mappings[XPLM_VK_Q] = "Q";
    g_key_mappings[XPLM_VK_R] = "R";
    g_key_mappings[XPLM_VK_S] = "S";
    g_key_mappings[XPLM_VK_T] = "T";
    g_key_mappings[XPLM_VK_U] = "U";
    g_key_mappings[XPLM_VK_V] = "V";
    g_key_mappings[XPLM_VK_W] = "W";
    g_key_mappings[XPLM_VK_X] = "X";
    g_key_mappings[XPLM_VK_Y] = "Y";
    g_key_mappings[XPLM_VK_Z] = "Z";
    
    // Special function keys
    g_key_mappings[XPLM_VK_BACK] = "clr";          // Backspace -> Clear
    g_key_mappings[XPLM_VK_SPACE] = "SP";          // Space -> SP
    g_key_mappings[XPLM_VK_DELETE] = "del";        // Delete -> Delete
    g_key_mappings[XPLM_VK_SLASH] = "slash";       // Forward slash - main keyboard (0xB8)
    g_key_mappings[XPLM_VK_DIVIDE] = "slash";      // Forward slash - numpad (0x6F)
    g_key_mappings[XPLM_VK_PERIOD] = "period";     // Period/decimal point (0xB9)
    g_key_mappings[XPLM_VK_MINUS] = "minus";       // Minus sign - main keyboard (0xB1)
    g_key_mappings[XPLM_VK_SUBTRACT] = "minus";    // Minus sign - numpad (0x6D)
    g_key_mappings[XPLM_VK_ADD] = "plus_special";  // Plus sign - numpad (0x6B) - special handling
    // Note: XPLM_VK_EQUAL (0xB0) is handled specially in KeyCallback for Shift+Equal = Plus
    
    // Debug: Log the critical key mappings
    LogMessage("Key mappings initialized:");
    char mapping_info[256];
    snprintf(mapping_info, sizeof(mapping_info), "  MINUS(0xB1)→%s, SUBTRACT(0x6D)→%s, ADD(0x6B)→%s, EQUAL(0xB0)→double_minus", 
             g_key_mappings[XPLM_VK_MINUS], g_key_mappings[XPLM_VK_SUBTRACT], g_key_mappings[XPLM_VK_ADD]);
    LogMessage(mapping_info);
}

// Check if current aircraft is ZIBO 737
static bool IsZIBO737()
{
    if (g_icao_dataref == NULL) return false;
    
    char icao[40];
    XPLMGetDatab(g_icao_dataref, icao, 0, sizeof(icao) - 1);
    icao[sizeof(icao) - 1] = '\0';
    
    return (strcmp(icao, "B738") == 0);
}

// Log message to X-Plane log
static void LogMessage(const char* message)
{
    char full_message[512];
    snprintf(full_message, sizeof(full_message), "ZIBO Keyboard Input: %s\n", message);
    XPLMDebugString(full_message);
}

// Toggle keyboard input for specified side
static void ToggleKeyboardInput(int side)
{
    // Validate input parameters
    if (side != 1 && side != 2) {
        LogMessage("Invalid side parameter. Use 1 for Captain or 2 for FO");
        return;
    }
    
    // Only work with ZIBO 737
    if (!IsZIBO737()) {
        LogMessage("Not compatible with current aircraft");
        return;
    }
    
    if (g_toggled == 0) {
        g_toggled = 1;
        g_fmc_side = side;
        const char* side_name = (side == 1) ? "Captain" : "First Officer";
        char message[256];
        snprintf(message, sizeof(message), "ZIBO %s FMC Keyboard Input Enabled", side_name);
        LogMessage(message);
    } else {
        g_toggled = 0;
        const char* side_name = (g_fmc_side == 1) ? "Captain" : "First Officer";
        char message[256];
        snprintf(message, sizeof(message), "ZIBO %s FMC Keyboard Input Disabled", side_name);
        LogMessage(message);
    }
    
    // Update status window visibility
    UpdateStatusWindow();
}

// Key event callback
static int KeyCallback(char /*inChar*/, XPLMKeyFlags inFlags, char inVirtualKey, void* /*inRefcon*/)
{
    // Only process key presses when enabled, on ZIBO 737, and for valid key mappings
    if (!(inFlags & xplm_DownFlag) || g_toggled == 0 || !IsZIBO737()) {
        return 1; // Let other handlers process the key
    }
    
    // Fix for signed char issue - convert to unsigned char for proper key lookup first
    unsigned char virtualKey = (unsigned char)inVirtualKey;
    
    // Check if any modifier keys are pressed - but allow Shift+Equal for plus sign
    // This fixes the issue where combo keys (like CTRL+SHIFT+I) still input letters to FMC
    // Use unsigned comparison to fix the signed char issue
    bool hasShiftEqual = (inFlags & xplm_ShiftFlag) && (virtualKey == XPLM_VK_EQUAL);
    if ((inFlags & (xplm_ShiftFlag | xplm_OptionAltFlag | xplm_ControlFlag)) && !hasShiftEqual) {
        return 1; // Let other handlers (like key commands) process modifier key combinations
    }
    
    // Special handling for Shift+Equal = Plus
    // Since ZIBO FMC doesn't have a direct "plus" command, we use the minus key twice
    // because ZIBO has internal logic: first minus = "-", second minus = "+"
    const char* button_name = nullptr;
    bool needsDoubleMinus = false;
    
    if (hasShiftEqual) {
        button_name = "minus";  // Use minus key for plus (will be pressed twice)
        needsDoubleMinus = true;  // Flag to press minus twice for plus
    } else {
        auto it = g_key_mappings.find(virtualKey);  // Use unsigned virtual key
        if (it != g_key_mappings.end()) {
            if (strcmp(it->second, "plus_special") == 0) {
                // Numpad plus key also uses double minus technique
                button_name = "minus";
                needsDoubleMinus = true;
            } else {
                button_name = it->second;
            }
        }
    }
    
    if (button_name != nullptr) {
        // Build command string
        char command_string[256];
        snprintf(command_string, sizeof(command_string), "laminar/B738/button/fmc%d_%s", g_fmc_side, button_name);
        
        // Debug logging with more detail
        char debug_msg[512];
        if (needsDoubleMinus) {
            const char* plus_source = hasShiftEqual ? "Shift+Equal" : "Numpad Plus";
            snprintf(debug_msg, sizeof(debug_msg), "Attempting FMC command: %s x2 (VK: 0x%02X, Flags: 0x%02X) [%s -> Double Minus for Plus]", 
                    command_string, virtualKey, inFlags, plus_source);
        } else {
            snprintf(debug_msg, sizeof(debug_msg), "Attempting FMC command: %s (VK: 0x%02X, Flags: 0x%02X) [Normal key: %s]", 
                    command_string, virtualKey, inFlags, button_name);
        }
        LogMessage(debug_msg);
        
        // Execute the command
        XPLMCommandRef command = XPLMFindCommand(command_string);
        if (command != NULL) {
            XPLMCommandOnce(command);
            
            // For plus sign (Shift+Equal), execute minus command twice
            // because ZIBO FMC toggles between minus and plus on repeated presses
            if (needsDoubleMinus) {
                // Small delay between commands to ensure proper processing
                // Note: This is a simple approach, could be improved with a timer
                XPLMCommandOnce(command);  // Execute minus a second time to get plus
                LogMessage("Double minus command executed for plus sign");
            } else {
                LogMessage("Command executed successfully");
            }
            return 0; // Consume the key event
        } else {
            char error_msg[256];
            snprintf(error_msg, sizeof(error_msg), "Command not found: %s", command_string);
            LogMessage(error_msg);
        }
    } else {
        char debug_msg[256];
        snprintf(debug_msg, sizeof(debug_msg), "No mapping found for VK: 0x%02X (signed: %d)", virtualKey, inVirtualKey);
        LogMessage(debug_msg);
    }
    
    return 1; // Let other handlers process the key
}

// Create status window using modern X-Plane window system
static void CreateStatusWindow()
{
    if (g_status_window != NULL) {
        return; // Window already exists
    }
    
    // Get screen dimensions for positioning
    int screenWidth, screenHeight;
    XPLMGetScreenSize(&screenWidth, &screenHeight);
    
    // Position window in bottom-right corner
    int window_left = screenWidth - 100;
    int window_top = 80;
    int window_right = screenWidth - 10;
    int window_bottom = 40;
    
    XPLMCreateWindow_t window_params;
    memset(&window_params, 0, sizeof(window_params));
    window_params.structSize = sizeof(window_params);
    window_params.left = window_left;
    window_params.top = window_top;
    window_params.right = window_right;
    window_params.bottom = window_bottom;
    window_params.visible = 0;  // Initially hidden
    window_params.drawWindowFunc = DrawStatusWindow;
    window_params.handleMouseClickFunc = NULL;
    window_params.handleKeyFunc = NULL;
    window_params.handleCursorFunc = NULL;
    window_params.handleMouseWheelFunc = NULL;
    window_params.refcon = NULL;
    window_params.layer = xplm_WindowLayerFloatingWindows;  // Top layer
    window_params.decorateAsFloatingWindow = xplm_WindowDecorationNone;  // No decoration
    window_params.handleRightClickFunc = NULL;  // No right click handling
    
    g_status_window = XPLMCreateWindowEx(&window_params);
    
    if (g_status_window != NULL) {
        LogMessage("Status window created successfully");
    } else {
        LogMessage("Failed to create status window");
    }
}

// Update status window visibility based on current state
static void UpdateStatusWindow()
{
    if (g_status_window == NULL) {
        return;
    }
    
    // Show window only when keyboard input is enabled and on ZIBO 737
    int should_show = (g_toggled == 1 && IsZIBO737()) ? 1 : 0;
    XPLMSetWindowIsVisible(g_status_window, should_show);
    
    char debug_msg[256];
    snprintf(debug_msg, sizeof(debug_msg), "Status window visibility updated: %s (toggled=%d, ZIBO=%d)", 
             should_show ? "VISIBLE" : "HIDDEN", g_toggled, IsZIBO737() ? 1 : 0);
    LogMessage(debug_msg);
}

// Draw status window content
static void DrawStatusWindow(XPLMWindowID inWindowID, void* /*inRefcon*/)
{
    // Get window geometry
    int left, top, right, bottom;
    XPLMGetWindowGeometry(inWindowID, &left, &top, &right, &bottom);
    
    // Set up OpenGL state for 2D drawing
    XPLMSetGraphicsState(0, 0, 0, 0, 1, 0, 0);  // Enable alpha blending
    
    // Draw semi-transparent background
    XPLMDrawTranslucentDarkBox(left, top, right, bottom);
    
    // Prepare status text
    const char* side_text = (g_fmc_side == 1) ? "CAP" : "FO";
    char status_text[16];
    snprintf(status_text, sizeof(status_text), "KB:%s", side_text);
    
    // Draw bright green status text
    float green_color[3] = {0.0f, 1.0f, 0.0f};
    XPLMDrawString(green_color, left + 5, top - 15, status_text, NULL, xplmFont_Basic);
    
    // Debug logging (occasional)
    static int draw_count = 0;
    draw_count++;
    if (draw_count % 180 == 1) {  // Log every 3 seconds
        LogMessage("Status window drawing successfully");
    }
}

// Captain command handler
static int CaptainCommandHandler(XPLMCommandRef /*inCommand*/, XPLMCommandPhase inPhase, void* /*inRefcon*/)
{
    if (inPhase == xplm_CommandBegin) {
        ToggleKeyboardInput(1);
    }
    return 0;
}

// First Officer command handler
static int FOCommandHandler(XPLMCommandRef /*inCommand*/, XPLMCommandPhase inPhase, void* /*inRefcon*/)
{
    if (inPhase == xplm_CommandBegin) {
        ToggleKeyboardInput(2);
    }
    return 0;
}

// Plugin entry point
PLUGIN_API int XPluginStart(char* outName, char* outSig, char* outDesc)
{
    strcpy(outName, PLUGIN_NAME);
    strcpy(outSig, PLUGIN_SIG);
    strcpy(outDesc, PLUGIN_DESC);
    
    // Initialize key mappings
    InitializeKeyMappings();
    
    // Find aircraft ICAO dataref
    g_icao_dataref = XPLMFindDataRef("sim/aircraft/view/acf_ICAO");
    
    // Create custom commands
    g_captain_command = XPLMCreateCommand("Zibo/ZIBO_Keyboard/Toggle_Keyboard_Input_Captain", 
                                        "Toggle Keyboard Input (Captain)");
    g_fo_command = XPLMCreateCommand("Zibo/ZIBO_Keyboard/Toggle_Keyboard_Input_FO", 
                                   "Toggle Keyboard Input (FO)");
    
    // Register command handlers
    XPLMRegisterCommandHandler(g_captain_command, CaptainCommandHandler, 1, NULL);
    XPLMRegisterCommandHandler(g_fo_command, FOCommandHandler, 1, NULL);
    
    // Register key callback
    XPLMRegisterKeySniffer(KeyCallback, 1, NULL);
    
    // Create status window using modern X-Plane window system
    CreateStatusWindow();
    UpdateStatusWindow();  // Set initial visibility
    
    LogMessage("Plugin initialized successfully");
    
    return 1;
}

// Plugin shutdown
PLUGIN_API void XPluginStop(void)
{
    // Destroy status window
    if (g_status_window != NULL) {
        XPLMDestroyWindow(g_status_window);
        g_status_window = NULL;
        LogMessage("Status window destroyed");
    }
    
    // Unregister callbacks
    XPLMUnregisterKeySniffer(KeyCallback, 1, NULL);
    
    // Unregister command handlers
    if (g_captain_command) {
        XPLMUnregisterCommandHandler(g_captain_command, CaptainCommandHandler, 1, NULL);
    }
    if (g_fo_command) {
        XPLMUnregisterCommandHandler(g_fo_command, FOCommandHandler, 1, NULL);
    }
    
    LogMessage("Plugin stopped");
}

PLUGIN_API void XPluginDisable(void)
{
    // Disable keyboard input when plugin is disabled
    g_toggled = 0;
    UpdateStatusWindow();  // Hide status window when disabled
}

PLUGIN_API int XPluginEnable(void)
{
    return 1;
}

PLUGIN_API void XPluginReceiveMessage(XPLMPluginID /*inFrom*/, int /*inMsg*/, void* /*inParam*/)
{
    // Handle plugin messages if needed
}
