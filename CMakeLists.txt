cmake_minimum_required(VERSION 3.22)

# Platform-specific configuration
if(APPLE)
    # Force arm64 for Apple Silicon (macOS only)
    set(CMAKE_OSX_ARCHITECTURES "arm64" CACHE STRING "Build architectures for macOS")
    # Use Ninja generator on macOS if not specified
    if(NOT CMAKE_GENERATOR)
        find_program(NINJA_EXE ninja)
        if(NINJA_EXE)
            set(CMAKE_GENERATOR "Ninja" CACHE INTERNAL "")
        endif()
    endif()
elseif(WIN32)
    # Use Visual Studio generator on Windows if not specified
    if(NOT CMAKE_GENERATOR)
        set(CMAKE_GENERATOR "Visual Studio 17 2022" CACHE INTERNAL "")
    endif()
endif()

project(Matrix-Control VERSION 0.0.18)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Generate compile_commands.json for IDE support (clangd, etc.)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

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
    Source/Core/PluginProcessor.cpp
    Source/Core/Factories/ApvtsFactory.cpp
    Source/Shared/SynthDescriptors.cpp
    Source/GUI/PluginEditor.cpp
    Source/GUI/Factories/WidgetFactory.cpp
    
    # Core - MIDI Communication Layer
    Source/Core/MIDI/Ports/MidiInputPort.cpp
    Source/Core/MIDI/Ports/MidiOutputPort.cpp
    Source/Core/MIDI/Transport/MidiSender.cpp
    Source/Core/MIDI/Transport/MidiReceiver.cpp
    Source/Core/MIDI/MidiManager.cpp
    Source/Core/MIDI/SysEx/SysExParser.cpp
    Source/Core/MIDI/SysEx/SysExDecoder.cpp
    Source/Core/MIDI/SysEx/SysExEncoder.cpp
    
    # GUI - Themes
    Source/GUI/Themes/Theme.cpp
    
    # GUI - Main Component
    Source/GUI/MainComponent.cpp
    
    # GUI - Panels
    Source/GUI/Panels/FooterPanel/FooterPanel.cpp
    Source/GUI/Panels/HeaderPanel/HeaderPanel.cpp
    Source/GUI/Panels/BodyPanel/BodyPanel.cpp
    Source/GUI/Panels/BodyPanel/MasterEditPanel/MasterEditPanel.cpp
    Source/GUI/Panels/BodyPanel/MatrixModulationPanel/MatrixModulationPanel.cpp
    Source/GUI/Panels/BodyPanel/PatchEditPanel/Modules/Dco1Panel.cpp
    Source/GUI/Panels/BodyPanel/PatchEditPanel/Modules/Dco2Panel.cpp
    Source/GUI/Panels/BodyPanel/PatchEditPanel/Modules/FmTrackPanel.cpp
    Source/GUI/Panels/BodyPanel/PatchEditPanel/PatchEditPanel.cpp
    Source/GUI/Panels/BodyPanel/PatchEditPanel/Modules/RampPortamentoPanel.cpp
    Source/GUI/Panels/BodyPanel/PatchEditPanel/Modules/VcfVcaPanel.cpp
    Source/GUI/Panels/BodyPanel/PatchManagerPanel/PatchManagerPanel.cpp
    
    # GUI - Widgets
    Source/GUI/Widgets/Button.cpp
    Source/GUI/Widgets/ComboBox.cpp
    Source/GUI/Widgets/FocusableWidget.cpp
    Source/GUI/Widgets/GroupLabel.cpp
    Source/GUI/Widgets/ModuleName.cpp
    Source/GUI/Widgets/PanelSeparator.cpp
    Source/GUI/Widgets/ParameterLabel.cpp
    Source/GUI/Widgets/ParameterSeparator.cpp
    Source/GUI/Widgets/PopupMenu.cpp
    Source/GUI/Widgets/SectionName.cpp
    Source/GUI/Widgets/Slider.cpp
    
    # Utilities
    Source/Core/MIDI/Utilities/MidiLogger.cpp
)

# ============================================================================
# PLUGIN CONFIGURATION
# ============================================================================

# Platform-specific plugin formats (filter out AU on Windows)
set(PLUGIN_FORMATS_LIST AU VST3 Standalone)
if(NOT APPLE)
    # Remove AU from formats on non-Apple platforms
    string(REPLACE "AU " "" PLUGIN_FORMATS_LIST "${PLUGIN_FORMATS_LIST}")
    string(REPLACE " AU" "" PLUGIN_FORMATS_LIST "${PLUGIN_FORMATS_LIST}")
    string(REPLACE "AU" "" PLUGIN_FORMATS_LIST "${PLUGIN_FORMATS_LIST}")
    string(STRIP "${PLUGIN_FORMATS_LIST}" PLUGIN_FORMATS_LIST)
endif()

# Platform-specific COPY_PLUGIN_AFTER_BUILD setting
if(APPLE)
    set(COPY_PLUGIN_AFTER_BUILD_VALUE TRUE)
else()
    set(COPY_PLUGIN_AFTER_BUILD_VALUE FALSE)
endif()

juce_add_plugin(Matrix-Control
    COMPANY_NAME "Ten Square Software"
    BUNDLE_ID "com.TenSquareSoftware.Matrix-Control"
    PLUGIN_MANUFACTURER_CODE Tssf
    PLUGIN_CODE Mcpi
    FORMATS ${PLUGIN_FORMATS_LIST}
    PRODUCT_NAME "Matrix-Control"
	PLUGIN_NAME "Matrix-Control"
    IS_SYNTH FALSE
    NEEDS_MIDI_INPUT TRUE
    NEEDS_MIDI_OUTPUT TRUE
    IS_MIDI_EFFECT FALSE
    EDITOR_WANTS_KEYBOARD_FOCUS FALSE
    COPY_PLUGIN_AFTER_BUILD ${COPY_PLUGIN_AFTER_BUILD_VALUE}
    VST3_CATEGORIES Fx
    AU_MAIN_TYPE kAudioUnitType_Effect
)

target_sources(Matrix-Control PRIVATE ${PLUGIN_SOURCES})

target_link_libraries(Matrix-Control
    PRIVATE
        juce::juce_audio_utils
        juce::juce_dsp
        juce::juce_gui_basics
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

# ============================================================================
# CUSTOM VST3 INSTALLATION (Windows)
# Copy VST3 plugin to custom folder after build for easy testing
# ============================================================================
#
# CONFIGURATION:
# ------------
# To customize the VST3 installation folder, modify the default value below
# or override it when configuring CMake:
#   cmake .. -DCUSTOM_VST3_FOLDER="your/custom/path"
#
# Examples:
#   - Personal folder: "C:/Users/YourName/VST3"
#   - Custom location: "D:/MyPlugins/VST3"
#   - Standard location (requires admin): "C:/Program Files/Common Files/VST3"
#
# Note: Using a personal folder avoids requiring administrator privileges.
#       Configure your DAW to scan this custom folder for VST3 plugins.
#
# ============================================================================

if(WIN32)
    # Define custom VST3 folder (can be overridden via CMake cache)
    if(NOT DEFINED CUSTOM_VST3_FOLDER)
        set(CUSTOM_VST3_FOLDER "C:/Users/YourName/VST3" CACHE PATH "Custom VST3 installation folder")
    endif()
    
    # VST3 target name (JUCE appends _VST3)
    set(VST3_TARGET_NAME "Matrix-Control_VST3")
    
    # Path where JUCE generates the VST3 bundle (using generator expression for CONFIG)
    set(VST3_SOURCE_DIR "${CMAKE_BINARY_DIR}/Matrix-Control_artefacts/$<CONFIG>/VST3/Matrix-Control.vst3")
    set(VST3_DEST_DIR "${CUSTOM_VST3_FOLDER}/Matrix-Control.vst3")
    
    # Add custom command to copy VST3 plugin after build
    add_custom_command(TARGET ${VST3_TARGET_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E echo "Copying VST3 plugin to custom folder: ${CUSTOM_VST3_FOLDER}"
        COMMAND ${CMAKE_COMMAND} -E make_directory "${CUSTOM_VST3_FOLDER}"
        COMMAND ${CMAKE_COMMAND} -E remove_directory "${VST3_DEST_DIR}"
        COMMAND ${CMAKE_COMMAND} -E copy_directory "${VST3_SOURCE_DIR}" "${VST3_DEST_DIR}"
        COMMENT "Installing VST3 plugin to custom folder"
        VERBATIM
    )
    
    message(STATUS "VST3 plugin will be copied to: ${CUSTOM_VST3_FOLDER}")
endif()
