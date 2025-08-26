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

#define XPLM200 1  // Enable X-Plane 10+ APIs (for commands)
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
#define PLUGIN_NAME "Universal FMC Keyboard Input"
#define PLUGIN_SIG "justin.universal.fmc.keyboard"
#define PLUGIN_DESC "Universal keyboard input handler for multiple aircraft FMC systems"

// Global state variables
static int g_toggled = 0;           // 0 = disabled, 1 = enabled
static int g_fmc_side = 1;          // 1 = Captain, 2 = First Officer
static XPLMDataRef g_icao_dataref = NULL;
static XPLMCommandRef g_captain_command = NULL;
static XPLMCommandRef g_fo_command = NULL;
static XPLMWindowID g_status_window = NULL;

// +/- button state tracking: 1 = showing +, -1 = showing -, 0 = unknown
static int g_captain_plusminus_state = 1;   // Captain FMC +/- button state  
static int g_fo_plusminus_state = 1;        // FO FMC +/- button state

// Aircraft configuration structure
enum AircraftType {
    AIRCRAFT_UNKNOWN = 0,
    AIRCRAFT_ZIBO_737,
    AIRCRAFT_DEFAULT_737,
    AIRCRAFT_DEFAULT_A330,
    AIRCRAFT_DEFAULT_SR22
};

struct AircraftConfig {
    AircraftType type;
    const char* name;
    const char* icao;
    const char* command_format;      // Command format with %s for key name (single FMC)
    const char* command_format_side; // For aircraft with side-specific commands (like ZIBO)
    const char* command_format_capt; // Captain FMC command format (for FMS/FMS2 style)
    const char* command_format_fo;   // First Officer FMC command format (for FMS/FMS2 style)
    const char* minus_command;       // Specific minus command for +/- toggle
    const char* minus_command_capt;  // Captain minus command (for FMS/FMS2 style)
    const char* minus_command_fo;    // First Officer minus command (for FMS/FMS2 style)
    bool has_side_specific_fmc;      // Whether aircraft has separate Capt/FO FMCs
};

// Supported aircraft configurations
static const AircraftConfig g_aircraft_configs[] = {
    {
        AIRCRAFT_ZIBO_737,
        "ZIBO 737",
        "B738",
        nullptr,                           // Uses side-specific format
        "laminar/B738/button/fmc%d_%s",   // Format with FMC side
        nullptr,                           // No separate capt format
        nullptr,                           // No separate fo format
        "laminar/B738/button/fmc%d_minus", // Minus command format
        nullptr,                           // No separate capt minus
        nullptr,                           // No separate fo minus
        true                               // Has side-specific FMCs
    },
    {
        AIRCRAFT_DEFAULT_737,
        "Default 737",
        "B738",
        nullptr,                           // Uses capt/fo specific formats
        nullptr,                           // No side-specific format
        "sim/FMS/key_%s",                  // Captain FMS commands
        "sim/FMS2/key_%s",                 // First Officer FMS commands
        nullptr,                           // No single minus command
        "sim/FMS/key_minus",              // Captain minus command
        "sim/FMS2/key_minus",             // First Officer minus command
        true                               // Has side-specific FMCs
    },
    {
        AIRCRAFT_DEFAULT_A330,
        "Default A330",
        "A330",
        nullptr,                           // Uses capt/fo specific formats
        nullptr,                           // No side-specific format
        "sim/FMS/key_%s",                  // Captain FMS commands
        "sim/FMS2/key_%s",                 // First Officer FMS commands
        nullptr,                           // No single minus command
        "sim/FMS/key_minus",              // Captain minus command
        "sim/FMS2/key_minus",             // First Officer minus command
        true                               // Has side-specific FMCs
    },
    {
        AIRCRAFT_DEFAULT_SR22,
        "Default SR22",
        "SR22",
        "sim/GPS/gcu478/%s",              // GPS GCU commands
        nullptr,                           // No side-specific format
        nullptr,                           // No capt format
        nullptr,                           // No fo format
        nullptr,                           // No plus/minus functionality
        nullptr,                           // No capt minus
        nullptr,                           // No fo minus
        false                             // Single GPS system
    }
};

// Current aircraft detection
static AircraftType g_current_aircraft = AIRCRAFT_UNKNOWN;
static const AircraftConfig* g_current_config = nullptr;

// Key mapping structure - maps virtual key codes to FMC button names
static std::map<int, const char*> g_key_mappings;

// Function prototypes
static int KeyCallback(char inChar, XPLMKeyFlags inFlags, char inVirtualKey, void* inRefcon);
static void DrawStatusWindow(XPLMWindowID inWindowID, void* inRefcon);
static int CaptainCommandHandler(XPLMCommandRef inCommand, XPLMCommandPhase inPhase, void* inRefcon);
static int FOCommandHandler(XPLMCommandRef inCommand, XPLMCommandPhase inPhase, void* inRefcon);
static void ToggleKeyboardInput(int side);
static AircraftType DetectAircraft();
static const AircraftConfig* GetAircraftConfig(AircraftType type);
static bool IsSupportedAircraft();
static void InitializeKeyMappings();
static const char* ConvertKeyName(const char* zibo_key_name);
static void LogMessage(const char* message);
static void CreateStatusWindow();
static void UpdateStatusWindow();
static int* GetPlusMinusStatePtr(int side);
static void HandlePlusMinusKey(int desired_state);

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
    g_key_mappings[XPLM_VK_RETURN] = "ent";        // Enter/Return -> Enter
    g_key_mappings[XPLM_VK_ENTER] = "ent";         // Numpad Enter -> Enter
    g_key_mappings[XPLM_VK_SLASH] = "slash";       // Forward slash - main keyboard (0xB8)
    g_key_mappings[XPLM_VK_DIVIDE] = "slash";      // Forward slash - numpad (0x6F)
    g_key_mappings[XPLM_VK_PERIOD] = "period";     // Period/decimal point (0xB9)
    g_key_mappings[XPLM_VK_MINUS] = "minus";       // Minus sign - main keyboard (0xB1) -> minus button
    g_key_mappings[XPLM_VK_SUBTRACT] = "minus";    // Minus sign - numpad (0x6D) -> minus button  
    g_key_mappings[XPLM_VK_ADD] = "plus_key";      // Plus sign - numpad (0x6B) -> smart plus handling
    // Note: XPLM_VK_EQUAL (0xB0) with Shift is handled specially in KeyCallback for Plus -> smart plus handling
    

}

// Convert key name based on aircraft type (ZIBO format -> aircraft-specific format)
static const char* ConvertKeyName(const char* zibo_key_name)
{
    if (!g_current_config) return zibo_key_name;
    
    // ZIBO aircraft uses original key names
    if (g_current_aircraft == AIRCRAFT_ZIBO_737) {
        return zibo_key_name;
    }
    
    // Default aircraft (737/A330) use different key names
    if (g_current_aircraft == AIRCRAFT_DEFAULT_737 || g_current_aircraft == AIRCRAFT_DEFAULT_A330) {
        // Convert special keys
        if (strcmp(zibo_key_name, "clr") == 0) return "clear";
        if (strcmp(zibo_key_name, "SP") == 0) return "space";
        if (strcmp(zibo_key_name, "del") == 0) return "delete";
        if (strcmp(zibo_key_name, "ent") == 0) return "enter";
        if (strcmp(zibo_key_name, "slash") == 0) return "slash";
        if (strcmp(zibo_key_name, "period") == 0) return "period";
        if (strcmp(zibo_key_name, "minus") == 0) return "minus";
        
        // Convert letters to lowercase (default aircraft uses lowercase)
        static char lowercase_buffer[8];
        if (strlen(zibo_key_name) == 1 && zibo_key_name[0] >= 'A' && zibo_key_name[0] <= 'Z') {
            lowercase_buffer[0] = zibo_key_name[0] - 'A' + 'a';
            lowercase_buffer[1] = '\0';
            return lowercase_buffer;
        }
        
        // Numbers and other keys remain the same
        return zibo_key_name;
    }
    
    // SR22 uses GPS GCU commands with specific naming
    if (g_current_aircraft == AIRCRAFT_DEFAULT_SR22) {
        // Convert special keys for SR22 GPS
        if (strcmp(zibo_key_name, "clr") == 0) return "clr";          // Clear
        if (strcmp(zibo_key_name, "SP") == 0) return "spc";           // Space  
        if (strcmp(zibo_key_name, "del") == 0) return "bksp";         // Backspace
        if (strcmp(zibo_key_name, "ent") == 0) return "ent";          // Enter
        if (strcmp(zibo_key_name, "period") == 0) return "dot";       // Period
        // Note: SR22 has no slash or minus functionality
        if (strcmp(zibo_key_name, "slash") == 0) return nullptr;      // No slash support
        if (strcmp(zibo_key_name, "minus") == 0) return nullptr;      // No minus support
        
        // Convert letters to uppercase (SR22 GPS uses uppercase)
        static char uppercase_buffer[8];
        if (strlen(zibo_key_name) == 1 && zibo_key_name[0] >= 'A' && zibo_key_name[0] <= 'Z') {
            uppercase_buffer[0] = zibo_key_name[0]; // Already uppercase
            uppercase_buffer[1] = '\0';
            return uppercase_buffer;
        }
        
        // Numbers remain the same
        return zibo_key_name;
    }
    
    return zibo_key_name; // Fallback
}

// Detect current aircraft type based on ICAO and specific characteristics
static AircraftType DetectAircraft()
{
    if (g_icao_dataref == NULL) return AIRCRAFT_UNKNOWN;
    
    char icao[40];
    XPLMGetDatab(g_icao_dataref, icao, 0, sizeof(icao) - 1);
    icao[sizeof(icao) - 1] = '\0';
    
    // Check for ZIBO 737 first (has specific ZIBO commands)
    if (strcmp(icao, "B738") == 0) {
        // Test if ZIBO-specific command exists to distinguish from default 737
        XPLMCommandRef zibo_test = XPLMFindCommand("laminar/B738/button/fmc1_0");
        if (zibo_test != NULL) {
            return AIRCRAFT_ZIBO_737;
        } else {
            return AIRCRAFT_DEFAULT_737;
        }
    }
    
    // Check for A330
    if (strcmp(icao, "A330") == 0) {
        return AIRCRAFT_DEFAULT_A330;
    }
    
    // Check for SR22
    if (strcmp(icao, "SR22") == 0) {
        return AIRCRAFT_DEFAULT_SR22;
    }
    
    return AIRCRAFT_UNKNOWN;
}

// Get aircraft configuration for given type
static const AircraftConfig* GetAircraftConfig(AircraftType type)
{
    for (size_t i = 0; i < sizeof(g_aircraft_configs) / sizeof(g_aircraft_configs[0]); i++) {
        if (g_aircraft_configs[i].type == type) {
            return &g_aircraft_configs[i];
        }
    }
    return nullptr;
}

// Check if current aircraft is supported
static bool IsSupportedAircraft()
{
    // Update aircraft detection
    g_current_aircraft = DetectAircraft();
    g_current_config = GetAircraftConfig(g_current_aircraft);
    
    return (g_current_config != nullptr);
}

// Log message to X-Plane log
static void LogMessage(const char* message)
{
    char full_message[512];
    snprintf(full_message, sizeof(full_message), "Universal FMC Keyboard: %s\n", message);
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
    
    // Only work with supported aircraft
    if (!IsSupportedAircraft()) {
        LogMessage("Current aircraft is not supported");
        return;
    }
    
    if (g_toggled == 0) {
        g_toggled = 1;
        
        // For aircraft without side-specific FMCs, always use side 1 (single FMC)
        if (!g_current_config->has_side_specific_fmc) {
            g_fmc_side = 1;
        } else {
            g_fmc_side = side;
        }
        
        // Reset +/- state to default (assume showing +) when enabling keyboard input
        // since we don't know the actual state of the FMC +/- button
        if (g_fmc_side == 1) {
            g_captain_plusminus_state = 1; // Assume showing +
        } else {
            g_fo_plusminus_state = 1; // Assume showing +
        }
        
        const char* side_name = (side == 1) ? "Captain" : "First Officer";
        char message[256];
        
        // Handle aircraft with or without side-specific FMCs
        if (g_current_config->has_side_specific_fmc) {
            snprintf(message, sizeof(message), "%s %s FMC Keyboard Input Enabled", 
                    g_current_config->name, side_name);
        } else {
            snprintf(message, sizeof(message), "%s FMC Keyboard Input Enabled", 
                    g_current_config->name);
        }
        LogMessage(message);
    } else {
        g_toggled = 0;
        char message[256];
        
        // Handle aircraft with or without side-specific FMCs
        if (g_current_config && g_current_config->has_side_specific_fmc) {
            const char* side_name = (g_fmc_side == 1) ? "Captain" : "First Officer";
            snprintf(message, sizeof(message), "%s %s FMC Keyboard Input Disabled", 
                    g_current_config->name, side_name);
        } else if (g_current_config) {
            snprintf(message, sizeof(message), "%s FMC Keyboard Input Disabled", 
                    g_current_config->name);
        } else {
            snprintf(message, sizeof(message), "FMC Keyboard Input Disabled");
        }
        LogMessage(message);
    }
    
    // Update status window visibility
    UpdateStatusWindow();
}

// Key event callback
static int KeyCallback(char /*inChar*/, XPLMKeyFlags inFlags, char inVirtualKey, void* /*inRefcon*/)
{
    // Only process key presses when enabled and on supported aircraft
    if (!(inFlags & xplm_DownFlag) || g_toggled == 0 || !IsSupportedAircraft()) {
        return 1; // Let other handlers process the key
    }
    
    // Convert to unsigned for proper key lookup
    unsigned char virtualKey = (unsigned char)inVirtualKey;
    
    // Check if any modifier keys are pressed - but allow Shift+Equal for plus sign
    // This fixes the issue where combo keys (like CTRL+SHIFT+I) still input letters to FMC
    bool hasShiftEqual = (inFlags & xplm_ShiftFlag) && (virtualKey == XPLM_VK_EQUAL);
    if ((inFlags & (xplm_ShiftFlag | xplm_OptionAltFlag | xplm_ControlFlag)) && !hasShiftEqual) {
        return 1; // Let other handlers (like key commands) process modifier key combinations
    }
    
    // Determine the desired state and button name
    const char* button_name = nullptr;
    int desired_plusminus_state = 0; // 1 = want +, -1 = want -, 0 = not a +/- key
    
    if (hasShiftEqual) {
        // Shift+Equal = Plus
        button_name = "plus_key";
        desired_plusminus_state = 1; // Want plus sign
    } else {
        auto it = g_key_mappings.find(virtualKey);
        if (it != g_key_mappings.end()) {
            button_name = it->second;
            if (strcmp(button_name, "minus") == 0) {
                // Minus key -> want minus sign
                desired_plusminus_state = -1;
            } else if (strcmp(button_name, "plus_key") == 0) {
                // Plus key -> want plus sign  
                desired_plusminus_state = 1;
            }
        }
    }
    
    if (button_name != nullptr) {
        // Handle +/- keys with intelligent state management
        if (desired_plusminus_state != 0) {
            HandlePlusMinusKey(desired_plusminus_state);
            return 0; // Consume the key event
        } else {
            // Handle normal keys (non +/- keys) 
            char command_string[256];
            
            // Convert key name to aircraft-specific format
            const char* converted_key_name = ConvertKeyName(button_name);
            
            // Check if the key is supported by current aircraft
            if (converted_key_name == nullptr) {
                // Key not supported by this aircraft (e.g., slash/minus on SR22)
                return 1; // Let other handlers process the key
            }
            
            // Generate command based on aircraft configuration
            if (g_current_config->command_format_capt && g_current_config->command_format_fo) {
                // Aircraft with separate Capt/FO formats (like default 737/A330)
                const char* format = (g_fmc_side == 1) ? g_current_config->command_format_capt : g_current_config->command_format_fo;
                snprintf(command_string, sizeof(command_string), format, converted_key_name);
            } else if (g_current_config->has_side_specific_fmc && g_current_config->command_format_side) {
                // Aircraft with side-specific FMCs (like ZIBO)
                snprintf(command_string, sizeof(command_string), g_current_config->command_format_side, g_fmc_side, converted_key_name);
            } else if (g_current_config->command_format) {
                // Aircraft with single FMC system (like SR22)
                snprintf(command_string, sizeof(command_string), g_current_config->command_format, converted_key_name);
            } else {
                return 1; // No valid command format
            }
            
            XPLMCommandRef command = XPLMFindCommand(command_string);
            if (command != NULL) {
                XPLMCommandOnce(command);
                return 0; // Consume the key event
            }
        }
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
    
    // Show window only when keyboard input is enabled and on supported aircraft
    int should_show = (g_toggled == 1 && IsSupportedAircraft()) ? 1 : 0;
    XPLMSetWindowIsVisible(g_status_window, should_show);
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
    char status_text[32];
    if (g_current_config && g_current_config->has_side_specific_fmc) {
        const char* side_text = (g_fmc_side == 1) ? "CAP" : "FO";
        snprintf(status_text, sizeof(status_text), "KB:%s", side_text);
    } else if (g_current_config) {
        // For aircraft without side-specific FMCs, show aircraft type
        const char* aircraft_short = "";
        if (g_current_aircraft == AIRCRAFT_DEFAULT_737) aircraft_short = "737";
        else if (g_current_aircraft == AIRCRAFT_DEFAULT_A330) aircraft_short = "330";
        else if (g_current_aircraft == AIRCRAFT_DEFAULT_SR22) aircraft_short = "SR22";
        else aircraft_short = "FMC";
        snprintf(status_text, sizeof(status_text), "KB:%s", aircraft_short);
    } else {
        snprintf(status_text, sizeof(status_text), "KB:ON");
    }
    
    // Draw bright green status text
    float green_color[3] = {0.0f, 1.0f, 0.0f};
    XPLMDrawString(green_color, left + 5, top - 15, status_text, NULL, xplmFont_Basic);
}

// Get pointer to the correct +/- state variable based on FMC side
static int* GetPlusMinusStatePtr(int side)
{
    return (side == 1) ? &g_captain_plusminus_state : &g_fo_plusminus_state;
}

// Handle +/- key press with intelligent state management using aircraft-specific minus command
static void HandlePlusMinusKey(int desired_state)
{
    if (!g_current_config) return;
    
    // Check if current aircraft supports +/- functionality
    if (!g_current_config->minus_command && !g_current_config->minus_command_capt && !g_current_config->minus_command_fo) {
        // Aircraft like SR22 don't have +/- functionality, ignore the key press
        return;
    }
    
    int* current_state = GetPlusMinusStatePtr(g_fmc_side);
    
    // If we already have the desired state, do nothing
    if (*current_state == desired_state) {
        return;
    }
    
    // Generate minus command based on aircraft configuration
    char command_string[256];
    if (g_current_config->minus_command_capt && g_current_config->minus_command_fo) {
        // Aircraft with separate Capt/FO minus commands (like default 737/A330)
        const char* minus_cmd = (g_fmc_side == 1) ? g_current_config->minus_command_capt : g_current_config->minus_command_fo;
        strcpy(command_string, minus_cmd);
    } else if (g_current_config->has_side_specific_fmc && g_current_config->minus_command && strstr(g_current_config->minus_command, "%d")) {
        // Aircraft with side-specific minus commands (like ZIBO)
        snprintf(command_string, sizeof(command_string), g_current_config->minus_command, g_fmc_side);
    } else if (g_current_config->minus_command) {
        // Aircraft with single minus command
        strcpy(command_string, g_current_config->minus_command);
    } else {
        return; // No minus command available
    }
    
    XPLMCommandRef command = XPLMFindCommand(command_string);
    if (command != NULL) {
        XPLMCommandOnce(command);
        *current_state = desired_state;  // Update our state tracking
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
        // Update current config first
        IsSupportedAircraft();
        
        if (g_current_config && !g_current_config->has_side_specific_fmc) {
            // For aircraft without side-specific FMCs like SR22, FO command acts same as Captain command
            ToggleKeyboardInput(1); // Use single FMC/GPS
        } else {
            // For aircraft with dual FMCs (ZIBO, default 737/A330), use FO side
            ToggleKeyboardInput(2); // Use FO FMC
        }
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
    g_captain_command = XPLMCreateCommand("Universal/FMC_Keyboard/Toggle_Keyboard_Input_Captain", 
                                        "Toggle FMC Keyboard Input (Captain)");
    g_fo_command = XPLMCreateCommand("Universal/FMC_Keyboard/Toggle_Keyboard_Input_FO", 
                                   "Toggle FMC Keyboard Input (FO)");
    
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
