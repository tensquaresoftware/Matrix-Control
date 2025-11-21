# Matrix-Control

## Functional Specification (FS)

**Author:** Guillaume DUPONT  
**Organization:** Ten Square Software  
**Last modified:** 2025-11-18  
**Version:** 1.0.0

---

## 1. General information and context

### 1.1 Project objective

**Matrix-Control** is a MIDI editor for the **Oberheim Matrix-1000** synthesizer. It is available as a **VST/AU plugin** and **standalone application**, allowing users to graphically edit the synth's parameters in real-time via **MIDI / SysEx messages**.

**Main features:**

- Complete editing of PATCH mode parameters (PATCH EDIT + MATRIX MODULATION sections)
- Editing of MASTER mode parameters (MASTER EDIT section)
- Patch management: load/save (synth memory + local .syx files)
- Bidirectional synchronization plugin ↔ synth (Program Changes + MIDI / SysEx messages)
- Resizable graphical interface with zoom (50-400%), 2 themes provided (Black/Cream)

### 1.2 Target audience

- Musicians and composers using an Oberheim Matrix-1000 synthesizer
- Sound designers seeking a graphical alternative to hardware editing
- Users with a Matrix-1000 with Tauntek EPROM (recommended)
- Users of modern DAWs (Ableton Live, Steinberg Cubase, Apple Logic Pro, Reason Studios, etc.)

### 1.3 Technical context

The project is based on:

- **Framework:** JUCE 8
- **Language:** C++17
- **Platforms:** macOS Tahoe 26 + Xcode 26 / Windows 11 + Visual Studio 2026
- **Plugin formats:** VST3 (macOS & Windows), AU2 (macOS)
- **Target synthesizer in v1.0:** Oberheim Matrix-1000 only
- **Future development:** Compatibility with Oberheim Matrix-6/6R synthesizers (planned for v2.0)

### 1.4 Complementary documents

**Technical Specification:**

- General architecture and patterns
- Business classes and responsibilities
- Threading and MIDI management
- Project structure, development phases
- Technical implementation details (MIDI protocol, algorithms, packing/unpacking)

**Use cases:**

- Critical workflows and interaction scenarios
- Detailed nominal and alternative scenarios
- Edge cases and error handling

**MIDI/SysEx reference:**

- Oberheim Matrix-1000 - MIDI & SysEx Implementation

---

## 2. Functional requirements

### 2.1 PATCH EDIT Section

Complete editing of a synth patch via 10 synthesis modules (96 parameters):

DCO 1, DCO 2, VCF/VCA, FM/TRACK, RAMP/PORTAMENTO, ENV 1, ENV 2, ENV 3, LFO 1, LFO 2.

#### Parameter initialization

The plugin provides 4 initialization actions per module:

| Action                    | Function                                                         |
| ------------------------- | ---------------------------------------------------------------- |
| Double-click on parameter | Resets the individual parameter (from INIT.syx or default value) |
| Button `I` (Init Module)  | Resets all parameters of the module                              |
| Button `C` (Copy Module)  | Copies the module to clipboard (type-aware, persistent)          |
| Button `P` (Paste Module) | Pastes compatible parameters from clipboard                      |

**Constraints:**

- INIT.syx file: `~/Music/Matrix-Control/INIT.syx` (configurable via Preferences)
- Fallback: Plugin default values if file missing/invalid
- Message if file invalid: `"No valid INIT.syx file found > Using plugin default values"`
- Delay between sequential SysEx messages: 10 ms minimum
- Clipboard: Persistent between patches, compatibility validation required

**INIT button behavior:**

- Loads patch data from INIT.syx file (or default values if unavailable)
- Immediately sends this patch to the synth
- Displays message: `"Patch initialized"`

**Use cases:**

- **UC-02:** Parameter initialization
- **UC-03:** Module copy-paste

#### Logical constraints

Special handling of the `Legato Portamento` parameter in the RAMP/PORTAMENTO module:

- **Activation condition:** `LEGATO PORTA` is usable only if `KEYBOARD MODE` = `UNISON`
- **Automatic deactivation:** `LEGATO PORTA` is grayed out if `KEYBOARD MODE` ≠ `UNISON`
- **Automatic correction:** If `KEYBOARD MODE` changes from `UNISON` to another value with `LEGATO PORTA` = `ON`, the plugin forces `LEGATO PORTA` = `OFF` and displays: `"LEGATO PORTA can only be activated if KEYBOARD MODE = UNISON"`

Apart from the `LEGATO PORTA` parameter, all PATCH mode parameters remain active, without graying out. The user can edit them freely; the synth handles any constraints internally.

**Use cases:**

- **UC-09:** LEGATO PORTA / KEYBOARD MODE constraint

#### VCF Frequency parameter: special handling

The `VCF Frequency` parameter (parameter 121) has special handling during patch data serialization:

- **All other signed parameters:** Standard signed range with sign extension
- **VCF Frequency only:** Uses 0-127 range without sign extension

This distinction is necessary to ensure compatibility with the Matrix-1000 SysEx format.

**Reference:** Oberheim Matrix-1000 - MIDI & SysEx Implementation, section "Single Patch Data Format".

### 2.2 MATRIX MODULATION Section

The modulation matrix complements synthesis parameters. It provides access to 10 identical modulation buses, numbered 0 to 9, each with 3 parameters: `Source` / `Amount` / `Destination`.

| Parameter     | Type        | Range                         | Behavior     |
| ------------- | ----------- | ----------------------------- | ------------ |
| `Source`      | Enumeration | None, Env1-3, LFO1-2, ...     | Never grayed |
| `Amount`      | Signed      | -63 to +63                    | Never grayed |
| `Destination` | Enumeration | None, Dco1 Freq, Dco1 PW, ... | Never grayed |

**Behavior:**

- Any modification of a parameter immediately sends the corresponding SysEx message to the synth
- The SysEx message encapsulates only the 3 parameters of the modified bus

**SysEx message 0BH parameters:**

```
F0H 10H 06H 0BH <bus> <source> <amount> <destination> F7H
```

- `<bus>`: Bus number (0-9)
- `<source>`: 0 to disable bus, 1-20 to select a source
- `<amount>`: Modulation amount (signed value), range -63 to +63
- `<destination>`: 0 to disable bus, 1-32 to select a destination

**Disable behavior:**

If `Source` OR `Destination` = 0, the bus is considered empty/disabled.

**Reference:** Oberheim Matrix-1000 - MIDI & SysEx Implementation, section "0BH - Remote Parameter Edit (Matrix Modulation)".

### 2.3 PATCH MANAGER Section

This section does not exist as such in the synth; it extends the Matrix-1000's capabilities. It includes 4 modules: PATCH NAME, BANK UTILITY, INTERNAL PATCHES, and COMPUTER PATCHES.

#### PATCH NAME Module

- Displays the name of the loaded patch (8 characters / Oberheim simplified ASCII format)
- At plugin startup: Displays `--------` to indicate "No patch loaded"

#### BANK UTILITY Module

| Control                           | Function                                                                                               |
| --------------------------------- | ------------------------------------------------------------------------------------------------------ |
| SELECT BANK :<br/>Buttons `0`-`9` | Selects and automatically locks the current bank<br/>Message displayed: `"Bank X selected and locked"` |
| Button `UNLOCK`                   | Unlocks the current bank<br/>Message displayed: `"Current bank unlocked"`                              |

**Technical implementation:**

- SELECT BANK buttons `0`-`9` send a specific SysEx message to the synth to select and lock the bank
- The synth responds by changing banks AND automatically activating Bank Lock
- The `UNLOCK` button sends a specific SysEx message to unlock the bank

**Reference:** Oberheim Matrix-1000 - MIDI & SysEx Implementation, sections "0AH - Set Bank" and "0CH - Unlock Bank".

#### INTERNAL PATCHES Module

Navigation through synth banks and patch memories.

##### BROWSER Group

| Control                    | Description                                                                                                                                                            |
| -------------------------- | ---------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| Display<br/>`Bank Number`  | Integer (0–9) read-only<br/>Modifiable via BANK UTILITY module                                                                                                         |
| Display<br/>`Patch Number` | Integer (00–99) editable via double-click<br/>Sends Program Change if valid<br/>Error message if invalid: `"Patch number is invalid : it must be in the range 00–99."` |

**Use cases:**

- **UC-05:** Editing Patch Number via double-click

##### UTILITY Group

| Control             | Function                                                                                                                                      |
| ------------------- | --------------------------------------------------------------------------------------------------------------------------------------------- |
| Buttons<br/>`<` `>` | Circular navigation through patches<br/>• Bank locked: navigation within this bank only<br/>• Bank unlocked: global navigation (1000 patches) |
| Button<br/>`INIT`   | Loads patch from INIT.syx (or default values) and immediately sends it to the synth                                                           |
| Button<br/>`COPY`   | Copies current patch to clipboard                                                                                                             |
| Button<br/>`PASTE`  | Pastes patch from clipboard<br/>• Active if clipboard compatible AND bank 0-1 (RAM)<br/>• Grayed if bank 2–9 (ROM)                            |
| Button<br/>`STORE`  | Saves patch to synth memory<br/>• Active if bank 0–1 (RAM)<br/>• Grayed if bank 2–9 (ROM)                                                     |

**STORE button implementation:**

- Sends a specific SysEx message to the synth to save the edit buffer to memory
- Displays message: `"Patch stored to memory"`

**Reference:** Oberheim Matrix-1000 - MIDI & SysEx Implementation, section "0EH - Store Edit Buffer".

**Use cases:**

- **UC-01:** Load a patch from internal memory
- **UC-04:** Navigation in locked/unlocked banks

#### COMPUTER PATCHES Module

Navigation through a folder of computer patches, stored as local .syx files.

##### BROWSER Group

| Control                   | Description                                                                                                                           |
| ------------------------- | ------------------------------------------------------------------------------------------------------------------------------------- |
| Buttons<br/>`<` `>`       | Circular navigation through valid patches in folder<br/>• Active if at least 2 valid patches<br/>• Grayed otherwise                   |
| ComboBox<br/>`Patch File` | List of valid patches sorted alphabetically<br/>• Grayed if no folder open or no valid patches<br/>• Displays `"No Patch !"` if empty |

##### FOLDER & FILES Group

| Control              | Function                                                                                                                                                                                  |
| -------------------- | ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| Button<br/>`OPEN`    | Opens a folder via system dialog<br/>Scans and validates .syx files (structure + checksum)<br/>Loads first valid patch<br/>Message displayed: `"Found X .syx files (Y valid, Z invalid)"` |
| Button<br/>`SAVE AS` | Saves via system dialog<br/>Stores full file path in Preferences<br/>Activates `SAVE` button                                                                                              |
| Button<br/>`SAVE`    | Quick save (overwrites stored file without confirmation)<br/>Grayed by default at startup                                                                                                 |

**Constraints:**

- The path stored in plugin Preferences is the full path of the last file saved via `SAVE AS`
- Each new `SAVE AS` replaces the previous path
- The `SAVE` button always uses the last stored path

**Use cases:**

- **UC-08:** SAVE AS → SAVE workflow
- **UC-10:** .syx file validation

### 2.4 MASTER EDIT Section

Editing of global synth parameters, divided into 3 modules: MIDI, VIBRATO, and MISC.

#### MIDI Module

Parameters: Channel, MIDI Echo, Controllers, Patch Changes, Pedal 1 Select, Pedal 2 Select, Lever 2 Select, Lever 3 Select.

**Note:** The plugin will not support editing via Registered/Non-Registered Parameters controllers (RPN/NRPN). The following MIDI controllers are documented in the MIDI/SysEx reference but their implementation remains optional for this version: Increment (96), Decrement (97), Non-Reg Parm LSB (98), Non-Reg Parm MSB (99), Reg Parm LSB (100), Reg Parm MSB (101).

#### VIBRATO Module

Parameters: Speed, Waveform, Amplitude, Speed Mod Source, Speed Mod Amount, Amp Mod Source, Amp Mod Amount.

#### MISC Module

Parameters: Master Tune, Master Transpose, Bend Range (+/-), Unison, Volume Invert, Bank Lock, Memory Protect.

#### Edit cycle

MASTER EDIT section parameters are not individually addressable via specific SysEx messages. Any modification of a MASTER parameter triggers sending the complete MASTER SysEx message (30+ parameters).

**Behavior:**

- **Initial FETCH:** Complete MASTER retrieval performed once at plugin startup
- **EDIT:** User modifies a parameter
- **UPDATE:** Updates corresponding value in plugin's local MasterModel
- **SEND:** Sends complete MASTER to synth

All MASTER EDIT section parameters remain active; they are never grayed out.

**Use cases:**

- **UC-06:** Editing a MASTER parameter

### 2.5 Settings Menu

Persistence of plugin preferences in DAW session (via APVTS) or local preferences file (Example: `~/Music/Matrix-Control/Settings.json`).

| Parameter             | Type                                               | Default                         |
| --------------------- | -------------------------------------------------- | ------------------------------- |
| Graphical theme       | Black / Cream                                      | Black                           |
| Zoom factor           | 50%, 75%, 100%, 110%, 125%, 150%, 200%, 300%, 400% | 100%                            |
| Manual resizing       | Zoom factor calculated by plugin in %              | -                               |
| MIDI input port       | String                                             | Auto-detection                  |
| MIDI output port      | String                                             | Auto-detection                  |
| INIT.syx file path    | File path                                          | ~/Music/Matrix-Control/INIT.syx |
| Last saved patch file | File path                                          | Empty                           |

### 2.6 Message bar

Displays real-time user messages at the bottom of the plugin window. The last message remains displayed for 3 seconds then fades out.

Message types and color codes: Info (gray), Success (green), Warning (orange), Error (red).

**Message examples:**

```
Info    : "Patch loaded: WarmPad"
Success : "Patch saved to disk: ~/Music/MyPatches/MyBrass.syx"
Warning : "No valid INIT.syx file found > Using plugin default values"
Error   : "MIDI device disconnected"
```

---

## 3. Technical requirements

### 3.1 Platforms and versions

| Aspect             | Specifications                                                                              |
| ------------------ | ------------------------------------------------------------------------------------------- |
| **macOS**          | Tahoe 26 / Xcode 26                                                                         |
| **Windows**        | Windows 11 / Visual Studio 2026                                                             |
| **Plugin formats** | VST3 (macOS & Windows), AU2 (macOS)                                                         |
| **Framework**      | JUCE 8.0.10 (latest stable version)                                                         |
| **Language**       | C++17                                                                                       |
| **Synth**          | • Matrix-1000 only (v1.0)<br/>• Secondary compatibility with Matrix-6/6R, without guarantee |

### 3.2 Architectural constraints

#### Threading

| Thread               | Responsibilities                      | Constraints                                                              |
| -------------------- | ------------------------------------- | ------------------------------------------------------------------------ |
| **Audio**            | `processBlock()` called by DAW        | • NON-blocking!<br/>• NO MIDI I/O!<br/>• Updates only via APVTS          |
| **GUI**              | User events, APVTS listeners          | • Component updates<br/>• MIDI command dispatch                          |
| **MIDI (dedicated)** | Send/receive SysEx, timeouts, parsing | • Blocking OK<br/>• MIDI I/O allowed<br/>• Thread-safe updates via APVTS |

#### Inter-thread communication

Done via an APVTS object (thread-safe via listeners) + lock-free FIFO for MIDI thread messages.

#### Performance

- **SysEx latency:** < 100 ms between user action and message sent to synth
- **Inter-message SysEx delay:** 10 ms minimum between two sequential SysEx message sends (or between patches)
- **MIDI timeout:** 2 seconds (error message displayed if synth does not respond)
- **On MIDI error:** No automatic retry; user must manually re-edit the parameter

#### Robustness

- **No UI blocking** during MIDI disconnection or synth timeout (local editing remains possible)
- **SysEx message validation:** Structure (F0...F7), length consistent with message type, valid checksum
- **Graceful error handling:** Business exceptions + propagation to GUI via APVTS

### 3.3 MIDI and SysEx communication

#### Sending SysEx messages

Each parameter modification immediately sends the corresponding SysEx message to the synth, via the plugin's MIDI output port.

#### Receiving SysEx messages

The plugin continuously listens to the plugin's MIDI input port to capture synth responses.

#### SysEx message validation

All SysEx messages received from the synth must be validated according to the following criteria:

- **Structure:** Message starts with F0H and ends with F7H
- **Checksum:** Oberheim checksum validation
- **Length:** Consistent with message type (PATCH ≠ MASTER)

If a SysEx message does not meet these criteria, it is rejected and an error message is displayed. No invalid message modifies the plugin state.

#### SysEx data format

All data transmitted in SysEx messages is encoded in "unpacked" format (2 nibbles per byte: low nibble first, then high nibble).

**Transmission statistics:**

- **Patches** (type 01H): 134 packed bytes → 268 nibbles + header (5 bytes) + checksum (1 byte) + EOX (1 byte) = **275 total bytes transmitted**
- **Master parameters** (type 03H): 172 packed bytes → 344 nibbles + header (5 bytes) + checksum (1 byte) + EOX (1 byte) = **351 total bytes transmitted**

**Reference:** Oberheim Matrix-1000 - MIDI & SysEx Implementation, section "SysEx Data Format".

#### Data requests (SysEx 04H)

The plugin does not implement the "Request All Bank" function in this version (v1.0). Supported data requests are:

- **Type 3:** Request master parameters only
- **Type 4:** Request edit buffer (current patch)

**Note:** Type 0 responses (Request All Bank) would include 50 Matrix-6 compatibility "split patches" that must be ignored.

**Reference:** Oberheim Matrix-1000 - MIDI & SysEx Implementation, section "04H - Request Data".

#### Synth auto-detection at startup

At plugin startup, or when selecting a MIDI port, the plugin automatically executes an auto-detection procedure:

1. Send identification request to connected MIDI device
2. Wait for device response (2 second timeout)
3. Verify that device is an Oberheim Matrix-1000
4. If validation OK:
   - Display: `"Matrix-1000 detected. Version: X.XX"`
   - Enable all synth controls
   - Proceed with initial FETCH of current patch
   - Proceed with initial FETCH of master parameters
5. If validation failed:
   - Display: `"Connected device is not a Matrix-1000. Please check your connection."`
   - Disable synth controls
   - No SysEx messages will be sent

**Use cases:**

- **UC-11:** Synth auto-detection at startup

#### Disconnection handling

| Case                              | Behavior                                                                                                                                      |
| --------------------------------- | --------------------------------------------------------------------------------------------------------------------------------------------- |
| MIDI input port unavailable       | Message displayed: `"MIDI device disconnected or not responding"`                                                                             |
| MIDI output port unavailable      | Message displayed: `"MIDI device disconnected"`                                                                                               |
| Synth not responding (2s timeout) | Message displayed: `"Matrix-1000 not responding. Check connection and try again."`<br/>Previous value restored (case of Patch Number editing) |
| Reconnection detected             | Message displayed: `"MIDI device / Matrix-1000 reconnected."`<br/>Automatic re-execution of initial synchronization                           |

**Constraints:**

- Local editing remains possible even during disconnection
- No automatic retry on failure
- No user interface blocking

**Use cases:**

- **UC-07:** MIDI disconnection handling

### 3.4 Validation and serialization

#### Patch .syx files

- **Structure:** F0 ... F7 (standard SysEx)
- **Checksum:** Validation via specific Oberheim algorithm
- **Validation:** Before display in COMPUTER PATCHES ComboBox, or file loading
- **Invalid files:** Ignored (not displayed in ComboBox)
- **Detailed message:** Example: `"Found 7 .syx files (5 valid, 2 invalid)"`

**Use cases:**

- **UC-10:** .syx file validation

#### Checksum validation

All SysEx messages (PATCH and MASTER) must be validated via Oberheim checksum. An invalid checksum results in message rejection with error message display: `"Invalid checksum. Patch not loaded."`

#### Variable-size field handling

Patch parameters use variable-size fields (1 to 7 bits) in a binary representation. During conversion between plugin internal representation and synth SysEx format, each parameter must be correctly serialized/deserialized.

**Important exception:** The VCF Frequency parameter (parameter 121) uses different handling than other signed parameters. This difference must be respected during serialization/deserialization.

#### Patch parameters

**Sending a SysEx message:**

- Min & max value verification before sending
- Invalid values: clamped or rejected

**Receiving a SysEx message:**

- Parameters validated after reception
- Invalid patch: error message `"Invalid patch : not loaded"`

### 3.5 UI and presentation

- **Fully vector interface:** no bitmap images
- **Resizable window:** preconfigured zoom (50–400%) + resizable bottom-right corner
- **2 themes provided:** Black & Cream (Black by default)
- **Responsive layout:** adapted to large screens and/or high resolution

---

## 4. Validation and integrity

### 4.1 Critical implementation points

#### Factory and descriptors (single source of truth)

Descriptors declare all synth parameters + UI widgets. Factory reads them to build APVTS + GUI. Consistency validation at startup detects duplicate parameter IDs and SysEx offsets → exception thrown if problem detected.

#### APVTS consistency

APVTS must contain **120+ patch parameters + 30+ master parameters + settings**. No parameter must be duplicated or forgotten (validated at startup by Factory).

#### Type-aware clipboard

Clipboard must remember copied module type + signature. Compatibility validation before pasting (DCO1 ↔ DCO2, ENV1/2/3 ↔ ENV, LFO1 ↔ LFO2). Persistent between patches.

**Use cases:**

- **UC-03:** Copy/paste a module

#### Patch Number double-click editing

When modifying Patch Number via double-click, plugin sends Program Change to synth. On timeout, error message displayed and previous patch number restored.

**Note:** No click-drag (avoids MIDI buffer saturation).

**Use cases:**

- **UC-05:** Patch Number editing

### 4.2 Unit tests

Critical classes to test (JUCE Framework):

- `SysExParser`: structure validation, checksum, parsing
- `PatchModel`: parameter consistency
- `MidiManager`: timeouts, reconnections
- `ClipboardManager`: type compatibility
- `PluginParameterFactory`: 1:1 consistency validation

---

## 5. SysEx message reference

The table below summarizes all SysEx messages used by the Matrix-Control plugin:

| Opcode       | Name                           | Direction      | Usage                           | Format                                 |
| ------------ | ------------------------------ | -------------- | ------------------------------- | -------------------------------------- |
| 01H          | Single Patch Data              | Plugin → Synth | Load/store patch in memory      | F0H 10H 06H 01H NN DATA CS F7H         |
| 03H          | Master Parameter Data          | Plugin ↔ Synth | Send/receive master parameters  | F0H 10H 06H 03H VER DATA CS F7H        |
| 04H (type 3) | Request Data - Master          | Plugin → Synth | Request master parameters       | F0H 10H 06H 04H 03H 00H F7H            |
| 04H (type 4) | Request Data - Edit Buffer     | Plugin → Synth | Request current patch           | F0H 10H 06H 04H 04H 00H F7H            |
| 06H          | Remote Parameter Edit          | Plugin → Synth | Edit individual patch parameter | F0H 10H 06H 06H PP VV F7H              |
| 0AH          | Set Bank                       | Plugin → Synth | Bank selection and locking      | F0H 10H 06H 0AH BB F7H                 |
| 0BH          | Remote Parameter Edit (Matrix) | Plugin → Synth | Edit matrix bus                 | F0H 10H 06H 0BH BB SS AM DD F7H        |
| 0CH          | Unlock Bank                    | Plugin → Synth | Bank unlocking                  | F0H 10H 06H 0CH F7H                    |
| 0DH          | Patch to Edit Buffer           | Plugin → Synth | Load patch to edit buffer       | F0H 10H 06H 0DH DATA CS F7H            |
| 0EH          | Store Edit Buffer              | Plugin → Synth | Save edit buffer to memory      | F0H 10H 06H 0EH NN BB ID F7H           |
| -            | Device Inquiry                 | Plugin → Synth | Synth auto-detection            | F0H 7EH CC 06H 01H F7H                 |
| -            | Device ID                      | Synth → Plugin | Identification response         | F0H 7EH CC 06H 02H MFG FAM MEM REV F7H |

**Legend:**

- NN = Patch number (0-99)
- VER = Version (03H for Matrix-1000)
- DATA = Patch/master data
- CS = Checksum
- BB = Bank number (0-9)
- PP = Parameter number
- VV = Parameter value
- SS = Source (matrix)
- AM = Amount (matrix)
- DD = Destination (matrix)
- CC = MIDI Channel
- MFG = Manufacturer ID
- REV = Firmware revision

**Reference:** Oberheim Matrix-1000 MIDI & SysEx Implementation

---

## 6. Scope v1.0

### 6.1 Must-have (mandatory)

- ✅ Complete PATCH mode real-time editing
- ✅ Simplified MASTER mode (without GROUPS, without RPN/NRPN)
- ✅ Load synth patch (PC + SysEx)
- ✅ Edit and send SysEx to synth
- ✅ Save patches synth memory + .syx files
- ✅ Circular navigation (locked/unlocked)
- ✅ Intelligent inter-module clipboard
- ✅ Fetch-Update-Send MASTER
- ✅ Matrix-1000 auto-detection at startup
- ✅ Initial synchronization (fetch patch + master)
- ✅ Persistent settings (theme, MIDI ports, INIT.syx path)
- ✅ Message bar (info/success/warning/error)
- ✅ Bidirectional MIDI communication
- ✅ Graceful disconnection handling + automatic reconnection
- ✅ Vector UI, resizable (zoom), themable (Black/Cream)
- ✅ Cross-platform (macOS + Windows)

### 6.2 Nice-to-have (v1.1+)

- 💡 Heartbeat/ping synth (continuous proactive detection)
- 💡 GROUPS management (Advanced MASTER mode)
- 💡 Matrix-6/6R support
- 💡 Undo/redo patches
- 💡 Random patch generator
- 💡 Macros (1 control = N parameters)
- 💡 Patch tags/ratings
- 💡 Virtual Matrix-1000 emulation
- 💡 Internationalization (i18n)

---

Copyright © 2025 Ten Square Software. All rights reserved.
