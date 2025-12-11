cmake_minimum_required(VERSION 3.22)

project(Matrix-Control VERSION 0.0.0)

# Platform-specific configurations
if(APPLE)
    # Force arm64 for Apple Silicon
    set(CMAKE_OSX_ARCHITECTURES "arm64" CACHE STRING "Build architectures for macOS")
endif()

if(WIN32)
    # Windows-specific settings
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Reference JUCE from environment variable
if(DEFINED ENV{JUCE_DIR})
    add_subdirectory($ENV{JUCE_DIR} "${CMAKE_BINARY_DIR}/JUCE")
else()
    message(FATAL_ERROR "JUCE_DIR environment variable not set. Please set it to your JUCE installation path.")
endif()

# ============================================================================
# SOURCE FILES
# Add your new .cpp files here (keep them sorted alphabetically for clarity)
# ============================================================================

set(PLUGIN_SOURCES
    # Core
    Source/PluginProcessor.cpp
    Source/PluginEditor.cpp
    Source/PluginFactory.cpp
    
    # Business - MIDI Communication Layer
    Source/Business/MIDI/Ports/MidiInputPort.cpp
    Source/Business/MIDI/Ports/MidiOutputPort.cpp
    Source/Business/MIDI/Transport/MidiSender.cpp
    Source/Business/MIDI/Transport/MidiReceiver.cpp
    Source/Business/MIDI/MidiManager.cpp
    Source/Business/MIDI/SysEx/SysExParser.cpp
    Source/Business/MIDI/SysEx/SysExDecoder.cpp
    Source/Business/MIDI/SysEx/SysExEncoder.cpp
    
    # UI - LookAndFeel
    Source/UI/LookAndFeel/McLookAndFeel.cpp
    
    # UI - Widgets
    Source/UI/Widgets/McSlider.cpp
    
    # Utilities
    Source/Business/MIDI/Utilities/MidiLogger.cpp
)

# ============================================================================
# PLUGIN CONFIGURATION
# ============================================================================

juce_add_plugin(Matrix-Control
    COMPANY_NAME "Ten Square Software"
    BUNDLE_ID "com.TenSquareSoftware.Matrix-Control"
    PLUGIN_MANUFACTURER_CODE Tssf
    PLUGIN_CODE Mcpi
    FORMATS AU VST3 Standalone
    PRODUCT_NAME "Matrix-Control"
	PLUGIN_NAME "Matrix-Control"
    IS_SYNTH FALSE
    NEEDS_MIDI_INPUT TRUE
    NEEDS_MIDI_OUTPUT TRUE
    IS_MIDI_EFFECT FALSE
    EDITOR_WANTS_KEYBOARD_FOCUS FALSE
    COPY_PLUGIN_AFTER_BUILD TRUE
    VST3_CATEGORIES Fx
    AU_MAIN_TYPE kAudioUnitType_Effect
)

target_sources(Matrix-Control PRIVATE ${PLUGIN_SOURCES})

target_link_libraries(Matrix-Control
    PRIVATE
        juce::juce_audio_utils
        juce::juce_dsp
    PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags
)

target_compile_definitions(Matrix-Control
    PUBLIC
        JUCE_WEB_BROWSER=0
        JUCE_USE_CURL=0
        JUCE_VST3_CAN_REPLACE_VST2=0
        JUCE_DISPLAY_SPLASH_SCREEN=0
        JUCE_REPORT_APP_USAGE=0
)