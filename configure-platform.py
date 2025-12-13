#!/usr/bin/env python3
"""
Platform Configuration Script for JUCE Projects
Automatically configures .vscode/settings.json based on the current platform.
Run this script if you open the project on a different platform than where it was generated.
"""

import os
import sys
import json
import platform
from pathlib import Path

def getScriptDirectory():
    return Path(__file__).parent

def getSettingsFilePath(scriptDir):
    return scriptDir / ".vscode" / "settings.json"

def ensureSettingsFileExists(settingsFile):
    if not settingsFile.exists():
        print("❌ Error: .vscode/settings.json not found.")
        print("   Make sure you run this script from the project root directory.")
        sys.exit(1)

def detectCurrentPlatform():
    system = platform.system()
    if system == "Darwin":
        return "build-macos", "default-macos", "macOS"
    elif system == "Windows":
        return "build-windows", "default-windows", "Windows"
    return "build", "default", system

def loadSettingsFromFile(settingsFile):
    try:
        with open(settingsFile, 'r', encoding='utf-8') as f:
            return json.load(f)
    except json.JSONDecodeError as e:
        print(f"❌ Error: Invalid JSON in {settingsFile}: {e}")
        sys.exit(1)

def normalizeBuildDirectoryPath(buildDir):
    buildPath = Path(buildDir)
    return buildPath.as_posix()

def updateSettingsForPlatform(settings, buildDir, presetName):
    normalizedBuildDir = normalizeBuildDirectoryPath(buildDir)
    settings["cmake.buildDirectory"] = f"${{workspaceFolder}}/{normalizedBuildDir}"
    if "cmake.configurePreset" in settings:
        settings["cmake.configurePreset"] = presetName

def writeSettingsToFile(settingsFile, settings):
    try:
        with open(settingsFile, 'w', encoding='utf-8') as f:
            json.dump(settings, f, indent=4, ensure_ascii=False)
    except Exception as e:
        print(f"❌ Error writing settings file: {e}")
        sys.exit(1)

def displaySuccessMessage(platformName, buildDir, presetName):
    print(f"✅ Successfully configured for {platformName}")
    print(f"   Build directory: {buildDir}")
    print(f"   CMake preset: {presetName}")
    print(f"\n   You can now open the project in Cursor and build directly!")

def configurePlatform():
    scriptDir = getScriptDirectory()
    settingsFile = getSettingsFilePath(scriptDir)
    ensureSettingsFileExists(settingsFile)
    
    buildDir, presetName, platformName = detectCurrentPlatform()
    settings = loadSettingsFromFile(settingsFile)
    updateSettingsForPlatform(settings, buildDir, presetName)
    writeSettingsToFile(settingsFile, settings)
    displaySuccessMessage(platformName, buildDir, presetName)

if __name__ == "__main__":
    configurePlatform()
