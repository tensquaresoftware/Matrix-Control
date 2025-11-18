# Matrix-Control / Specifications

---

- **Author:** Guillaume DUPONT
- **Organization:** Ten Square Software
- **Revision date:** 2025-08-08

---

## 1. Plugin Objective

MatrixControl is a VST/AU plugin (and standalone application) for complete remote control of Oberheim Matrix-1000, Matrix-6, and Matrix-6R synthesizers via MIDI SysEx messages. Its focus is on stable, professional, and scalable operation compatible with all major DAWs (Ableton Live, Logic, etc.), including workarounds for SysEx filtering.

## 2. Supported Synthesizers

- **Matrix-1000:** Full parameter support via SysEx.
- **Matrix-6/6R:** Support planned (Phase 2), with model auto-detection and mapping of parameter/SysEx differences.

## 3. Core Features

- **Patch editing:** Real-time graphical editing (vector GUI, sliders, envelopes, menus).
- **Patch management:** Advanced Patch Manager with tagging, rating, favorites, and A/B comparison.
- **SysEx communication:** Reliable bidirectional MIDI SysEx exchange, including loopback/virtual port support.
- **Session recall:** Automatic restoration of patch state with DAW projects.
- **Import/export:** Compatible SysEx and internal format for cross-platform use.

## 4. User Interface (GUI)

### 4.1 Principles

- **Vector-based, resizable, and zoomable:** GUI adapts to any window size and screen (Retina/4K+), with zoom presets.
- **Themes:** "Black" (dark) and "Cream" (light), switchable on-the-fly.
- **Modern look:** Components designed for clarity and professional aesthetics.

### 4.2 Architecture

- **Centralized LookAndFeel system:** All components use a shared base (MatrixLookAndFeelBase, Black/Cream variants).
- **ScalableComponentMixin:** Utility class for managing scaling of all dimensions, fonts, and thicknesses.
- **Dynamic theme switching:** Immediate theme changes, no restart needed.
- **Responsive layout:** All controls adapt to resizing and zoom factor.

### 4.3 GUI Widgets

- **Widgets:** MatrixLabel, MatrixButton, MatrixSlider, MatrixComboBox, MatrixNumber, MatrixTabBar, MatrixLed, MatrixSectionTitle, MatrixModuleTitle, MatrixSeparatorLine, MatrixEnvelopeDisplay, MatrixTrackingGeneratorDisplay, MatrixPatchNameDisplay.
- **Envelopes:** Editable with mouse (Delay, Attack, Decay, Sustain, Release).
- **Patch navigation:** Bank/patch selectors, browser for internal and computer patches.
- **LEDs:** Always-visible MIDI activity indicators.
- **Menus:** MIDI In/Out, Audio In (for alternative DAW routing), theme, zoom.

## 5. MIDI & DAW Integration

- **SysEx I/O:** MIDI In/Out selection, loopback/virtual port workaround for DAWs filtering SysEx.
- **DAW routing:** Supports both audio and MIDI track insertion, with appropriate input/output selection.
- **Performance:** Send rate limitation to avoid buffer saturation; visual feedback for MIDI traffic.

## 6. Patch Management

- **Patch folder selection:** Configurable in settings.
- **Internal patch structure:** Fully compatible with Matrix synths (.syx files) and JUCE serialization (JSON/XML).
- **INIT patch:** Auto-load at startup if present.
- **History & comparison:** A/B snapshot, undo/redo tracking.
- **Advanced browser:** Tagging, rating, drag-and-drop, search/filter (future phase).

## 7. Technical Foundations

- **JUCE 8.0.8** (refer to official documentation for all implementation details and APIs)
- **macOS Big Sur, Xcode 12.4, C++17+**
- **AudioProcessorValueTreeState** for GUI <-> parameter binding.
- **Modular code:** SysExEngine, PatchModel, MidiManager, GUI, ThemeManager.
- **Session recall:** Patch state restored with DAW projects and in standalone mode.

## 8. Planned Evolutions

- **Random patch generator:** Lockable groups (osc/VCF/etc.).
- **Sound mutation:** Progressive alteration, mutation controls, history.
- **Macros:** Multiple parameters controlled by one UI element.
- **Virtual audio engine:** Matrix-like synthesis emulation.

## 9. Coding & Quality Constraints

- **No audio generation:** Except for future emulation phase.
- **Cross-platform:** macOS and Windows.
- **Code standards:** English only, explicit naming, short/clear methods (5-15 lines), strict separation of concerns.
- **Maintainability:** Modular, testable, and extensible code.
- **Performance:** Responsive UI, low-latency MIDI handling, efficient drawing.

## 10. Suggestions for Annexes

- **Annex 1:** GUI Implementation Details (scaling math, font usage, component prototypes, code snippets)
- **Annex 2:** MIDI/SysEx Protocol Reference (specs, examples, compatibility notes)
- **Annex 3:** DAW Integration & Testing Plan
- **Annex 4:** Accessibility & Internationalization (if required)
- **Annex 5:** Plugin Validation & User Testing Methodology

---

Copyright © 2025 Ten Square Software. All rights reserved.