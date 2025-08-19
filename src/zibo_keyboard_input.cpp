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

#if APL || LIN
#define GL_SILENCE_DEPRECATION
#include <OpenGL/gl.h>
#elif IBM
#include <GL/gl.h>
#endif

#if IBM
#include <windows.h>
#endif

// Plugin information
#define PLUGIN_NAME "ZIBO Keyboard Input"
#define PLUGIN_SIG "3370tech.zibo.keyboard"
#define PLUGIN_DESC "Keyboard input handler for ZIBO 737 FMC"

// Global state variables
static int g_toggled = 0;           // 0 = disabled, 1 = enabled
static int g_fmc_side = 1;          // 1 = Captain, 2 = First Officer
static XPLMDataRef g_icao_dataref = NULL;
static XPLMCommandRef g_captain_command = NULL;
static XPLMCommandRef g_fo_command = NULL;

// Key mapping structure - maps virtual key codes to FMC button names
static std::map<int, const char*> g_key_mappings;

// Function prototypes
static int KeyCallback(char inChar, XPLMKeyFlags inFlags, char inVirtualKey, void* inRefcon);
static int DrawCallback(XPLMDrawingPhase inPhase, int inIsBefore, void* inRefcon);
static int CaptainCommandHandler(XPLMCommandRef inCommand, XPLMCommandPhase inPhase, void* inRefcon);
static int FOCommandHandler(XPLMCommandRef inCommand, XPLMCommandPhase inPhase, void* inRefcon);
static void ToggleKeyboardInput(int side);
static bool IsZIBO737();
static void InitializeKeyMappings();
static void LogMessage(const char* message);

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
    g_key_mappings[XPLM_VK_SLASH] = "slash";       // Forward slash
    g_key_mappings[XPLM_VK_PERIOD] = "period";     // Period/decimal point
    g_key_mappings[XPLM_VK_MINUS] = "minus";       // Minus sign
    g_key_mappings[XPLM_VK_SUBTRACT] = "minus";    // Numpad minus
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
}

// Key event callback
static int KeyCallback(char /*inChar*/, XPLMKeyFlags inFlags, char inVirtualKey, void* /*inRefcon*/)
{
    // Only process key presses when enabled, on ZIBO 737, and for valid key mappings
    if (!(inFlags & xplm_DownFlag) || g_toggled == 0 || !IsZIBO737()) {
        return 1; // Let other handlers process the key
    }
    
    auto it = g_key_mappings.find(inVirtualKey);
    if (it != g_key_mappings.end()) {
        // Build command string
        char command_string[256];
        snprintf(command_string, sizeof(command_string), "laminar/B738/button/fmc%d_%s", g_fmc_side, it->second);
        
        // Execute the command
        XPLMCommandRef command = XPLMFindCommand(command_string);
        if (command != NULL) {
            XPLMCommandOnce(command);
            return 0; // Consume the key event
        }
    }
    
    return 1; // Let other handlers process the key
}

// Drawing callback for visual indicators
static int DrawCallback(XPLMDrawingPhase /*inPhase*/, int /*inIsBefore*/, void* /*inRefcon*/)
{
    // Only draw for ZIBO 737 when keyboard input is enabled
    if (!IsZIBO737() || g_toggled == 0) {
        return 1;
    }
    
    // Get screen dimensions
    int screenWidth, screenHeight;
    XPLMGetScreenSize(&screenWidth, &screenHeight);
    
    // Prepare text
    const char* side_text = (g_fmc_side == 1) ? "CAP" : "FO";
    char full_text[64];
    snprintf(full_text, sizeof(full_text), "%s Keyboard Input Enabled", side_text);
    
    // Set up OpenGL for 2D drawing
    XPLMSetGraphicsState(0, 0, 0, 0, 0, 0, 0);
    
    // Set color to red for text drawing
    float red_color[3] = {1.0f, 0.0f, 0.0f};
    
    // Draw status text in top-left corner
    XPLMDrawString(red_color, 50, screenHeight - 50, full_text, NULL, xplmFont_Proportional);
    
    // Get mouse position and draw indicator near cursor
    int mouseX, mouseY;
    XPLMGetMouseLocation(&mouseX, &mouseY);
    XPLMDrawString(red_color, mouseX + 15, mouseY + 5, side_text, NULL, xplmFont_Proportional);
    
    return 1;
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
    g_captain_command = XPLMCreateCommand("3370Tech/ZIBO_Keyboard/Toggle_Keyboard_Input_Captain", 
                                        "Toggle Keyboard Input (Captain)");
    g_fo_command = XPLMCreateCommand("3370Tech/ZIBO_Keyboard/Toggle_Keyboard_Input_FO", 
                                   "Toggle Keyboard Input (FO)");
    
    // Register command handlers
    XPLMRegisterCommandHandler(g_captain_command, CaptainCommandHandler, 1, NULL);
    XPLMRegisterCommandHandler(g_fo_command, FOCommandHandler, 1, NULL);
    
    // Register key callback
    XPLMRegisterKeySniffer(KeyCallback, 1, NULL);
    
    // Register draw callback
    XPLMRegisterDrawCallback(DrawCallback, xplm_Phase_Gauges, 1, NULL);
    
    LogMessage("Plugin initialized successfully");
    
    return 1;
}

// Plugin shutdown
PLUGIN_API void XPluginStop(void)
{
    // Unregister callbacks
    XPLMUnregisterKeySniffer(KeyCallback, 1, NULL);
    XPLMUnregisterDrawCallback(DrawCallback, xplm_Phase_Gauges, 1, NULL);
    
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
}

PLUGIN_API int XPluginEnable(void)
{
    return 1;
}

PLUGIN_API void XPluginReceiveMessage(XPLMPluginID /*inFrom*/, int /*inMsg*/, void* /*inParam*/)
{
    // Handle plugin messages if needed
}
