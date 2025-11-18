# Matrix-Control

## Use Cases

**Author:** Guillaume DUPONT  
**Organization:** Ten Square Software  
**Last modified:** 2025-11-18  
**Version:** 1.0.0

---

## UC-01: Load a patch from the synthesizer

**Actor:** User  
**Objective:** Load a patch stored in the Matrix-1000  
**Trigger:** Click on `<` or `>` button (navigation)

### Preconditions

- Plugin is connected to synth via MIDI
- At least one patch exists in synth memory

### Nominal scenario

1. User clicks on `>` button
2. Plugin calculates next patch number (circular navigation)
3. Plugin sends Program Change #NN to synth
4. Synth loads patch into memory
5. Synth returns complete patch via SysEx
6. Plugin validates SysEx (structure + checksum)
7. Plugin parses 126 patch parameters
8. Plugin updates GUI (96 PATCH params + 30 MATRIX MODULATION params)
9. Message bar displays: `"Patch loaded: [PATCH_NAME]"`

### Alternative scenarios

**2a. Bank locked (Bank Lock activated)**

- 2a.1. Circular navigation only within the 100 patches of current bank
- 2a.2. Reached patch 99, return to patch 00 of same bank

**2b. Bank unlocked**

- 2b.1. Circular navigation across 1000 patches (10 banks)
- 2b.2. Reached patch 9-99, return to patch 0-00

**6a. Invalid SysEx (incorrect checksum)**

- 6a.1. Plugin rejects message
- 6a.2. Message bar displays: `"Invalid patch received"`
- 6a.3. Previous patch remains displayed

**5a. MIDI timeout (2 seconds)**

- 5a.1. Synth does not respond
- 5a.2. Message bar displays: `"Matrix-1000 not responding. Check connection and try again."`
- 5a.3. Patch number is not modified

### Postconditions

- Patch NN is loaded in plugin
- GUI displays 126 parameters correctly
- Plugin and synth are synchronized

---

## UC-02: Initialize an individual parameter

**Actor:** User  
**Objective:** Reset a parameter to its INIT value  
**Trigger:** Double-click on a parameter control

### Preconditions

- Plugin is launched
- A patch is currently being edited

### Nominal scenario

1. User double-clicks on slider "DCO1 Freq"
2. Plugin checks for presence of `~/Music/Matrix-Control/INIT.syx` file
3. Plugin loads corresponding value from INIT.syx
4. Plugin updates local parameter display
5. Plugin sends corresponding SysEx message to synth
6. Synth applies new value
7. Message bar displays: `"Parameter initialized"`

### Alternative scenarios

**2a. INIT.syx file missing or invalid**

- 2a.1. Plugin uses internal default value
- 2a.2. Message bar displays: `"No valid INIT.syx file found > Using plugin default values"`
- Return to step 4

**6a. Synth does not respond (2s timeout)**

- 6a.1. Message bar displays: `"Matrix-1000 not responding. Check connection and try again."`
- 6a.2. Local value remains modified (no rollback)

### Postconditions

- Parameter is reset to its INIT value
- Plugin and synth are synchronized (or error message displayed)

---

## UC-03: Copy/paste a module

**Actor:** User  
**Objective:** Duplicate parameters from one module to another compatible module  
**Trigger:** Click on `C` (Copy) button then `P` (Paste) button

### Preconditions

- Plugin is launched
- A patch is currently being edited

### Nominal scenario

1. User clicks on `C` button of DCO1 module
2. Plugin copies 10 DCO1 module parameters to clipboard
3. Plugin remembers copied module type (`DCO`)
4. User clicks on `P` button of DCO2 module
5. Plugin checks compatibility (`DCO` ↔ `DCO` = OK)
6. Plugin pastes common parameters into DCO2
7. Plugin sends corresponding SysEx messages to synth (10 ms delay between each)
8. Synth applies new values
9. Message bar displays: `"Module pasted successfully"`

### Alternative scenarios

**5a. Incompatible types (ex: DCO → LFO)**

- 5a.1. `P` button remains grayed
- 5a.2. No action possible

**5b. Empty clipboard**

- 5b.1. `P` button remains grayed
- 5b.2. No action possible

**8a. Synth does not respond during sequential sending**

- 8a.1. Message bar displays: `"MIDI device not responding"`
- 8a.2. Local values remain modified

### Postconditions

- Target module contains source module parameters
- Clipboard retains its copy (persistent between patches)

---

## UC-04: Navigate through synthesizer patches

**Actor:** User  
**Objective:** Browse internal synth patches according to lock mode  
**Trigger:** Click on `<` or `>` button of INTERNAL PATCHES module

### Preconditions

- Plugin is connected to synth
- At least one bank contains patches

### Nominal scenario (bank unlocked)

1. User clicks on `>` button
2. Plugin detects that current bank is unlocked
3. Plugin calculates next patch in global circular navigation (0-00 → 9-99)
4. Plugin sends corresponding Program Change
5. Synth returns patch via SysEx
6. Plugin updates GUI
7. Message bar displays: `"Patch loaded: [NAME]"`

### Alternative scenarios

**2a. Bank locked (Bank Lock activated)**

- 2a.1. Circular navigation only within current bank (ex: 3-00 → 3-99)
- 2a.2. Reached last patch of bank, return to first patch of this bank

**2b. Transition from 9-99 to 0-00 (bank unlocked)**

- 2b.1. Global circular navigation continues
- 2b.2. No interruption

**5a. MIDI timeout**

- 5a.1. Message bar displays: `"Matrix-1000 not responding. Check connection and try again."`
- 5a.2. Displayed patch number does not change

### Postconditions

- Next/previous patch is loaded
- Navigation respects lock mode

---

## UC-05: Edit patch number by double-click

**Actor:** User  
**Objective:** Load a specific patch by entering its number directly  
**Trigger:** Double-click on `Patch Number` display

### Preconditions

- Plugin is connected to synth
- `Patch Number` display is visible

### Nominal scenario

1. User double-clicks on `Patch Number` display
2. Display becomes editable (input field)
3. User enters `42` then validates (Enter key)
4. Plugin verifies that 42 is in range [00-99]
5. Plugin sends Program Change #42 to synth
6. Synth loads patch #42 into memory
7. Synth returns complete patch via SysEx
8. Plugin validates and parses SysEx
9. Plugin updates GUI
10. Message bar displays: `"Patch loaded: [NAME]"`

### Alternative scenarios

**4a. Invalid number (ex: 105, -3, "abc")**

- 4a.1. Plugin rejects input
- 4a.2. Previous number is restored in display
- 4a.3. Message bar displays: `"Patch number is invalid : it must be in the range 00–99."`

**7a. MIDI timeout (synth does not respond)**

- 7a.1. 2 second timeout exceeded
- 7a.2. Previous number is restored in display
- 7a.3. Message bar displays: `"Matrix-1000 not responding. Check connection and try again."`

**8a. Invalid SysEx (incorrect checksum)**

- 8a.1. Plugin rejects message
- 8a.2. Previous number is restored
- 8a.3. Message bar displays: `"Invalid patch received"`

### Postconditions

- Patch NN is loaded and displayed (or error message)
- Display becomes non-editable again

---

## UC-06: Edit a MASTER parameter

**Actor:** User  
**Objective:** Modify a global synth parameter (ex: Master Tune)  
**Trigger:** Modification of a MASTER EDIT section control

### Preconditions

- Plugin performed MASTER FETCH at startup
- Local MasterModel contains 30+ up-to-date parameters

### Nominal scenario

1. User modifies slider "Master Tune" to +12
2. Plugin updates corresponding value in local MasterModel
3. Plugin rebuilds complete MASTER SysEx message (30+ parameters)
4. Plugin sends MASTER SysEx to synth
5. Synth applies new global parameters
6. Message bar displays: `"Master parameter updated"`

### Alternative scenarios

**4a. MIDI timeout (synth does not respond)**

- 4a.1. Synth does not respond within 2 seconds
- 4a.2. Message bar displays: `"Matrix-1000 not responding. Check connection and try again."`
- 4a.3. Local value remains modified (optimistic editing, no rollback)

**1a. User rapidly modifies several MASTER parameters**

- 1a.1. Each modification triggers local UPDATE + complete SEND
- 1a.2. 10 ms delay between each SysEx send (throttling if necessary)

### Postconditions

- Local MasterModel is up to date
- Synth received complete MASTER
- No new FETCH necessary (plugin is source of truth)

---

## UC-07: Handle MIDI disconnection

**Actor:** System  
**Objective:** Detect and handle MIDI device disconnection  
**Trigger:** Loss of connection with Matrix-1000

### Preconditions

- Plugin was connected to synth
- MIDI communication was active

### Nominal scenario (disconnection detected)

1. Plugin attempts to send a SysEx message
2. MIDI output port does not respond (2 second timeout)
3. Plugin detects disconnection
4. Message bar displays: `"MIDI device disconnected"`
5. Plugin enters "local editing only" mode
6. User can continue editing parameters locally
7. No SysEx messages are sent until connection is restored

### Alternative scenarios

**1a. Disconnection during SysEx reception**

- 1a.1. MIDI input port no longer responds
- 1a.2. 2 second timeout
- 1a.3. Message bar displays: `"MIDI device disconnected or not responding"`
- Return to step 5

**8. Reconnection detected**

- 8.1. User reconnects Matrix-1000
- 8.2. Plugin detects reconnection (on next SysEx send)
- 8.3. Message bar displays: `"MIDI device / Matrix-1000 reconnected."`
- 8.4. MIDI communications resume normally

### Postconditions

- Plugin gracefully handles disconnection
- Interface remains usable in local mode
- No UI blocking

---

## UC-08: SAVE AS → SAVE workflow

**Actor:** User  
**Objective:** Quickly save a patch after a first SAVE AS  
**Trigger:** Click on `SAVE AS` button then `SAVE` button

### Preconditions

- Plugin is launched
- A patch is currently being edited

### Nominal scenario

1. User clicks on `SAVE AS` button
2. System dialog opens
3. User selects folder `/Music/MyPatches/`
4. User enters filename `MyBrass.syx`
5. Plugin saves current patch as .syx file
6. Plugin stores full path `/Music/MyPatches/MyBrass.syx` in Preferences
7. `SAVE` button becomes active
8. Message bar displays: `"Patch saved to disk: /Music/MyPatches/MyBrass.syx"`
9. User then modifies patch
10. User clicks on `SAVE` button
11. Plugin directly overwrites `/Music/MyPatches/MyBrass.syx` without confirmation request
12. Message bar displays: `"Patch saved to disk: /Music/MyPatches/MyBrass.syx"`

### Alternative scenarios

**4a. User cancels dialog**

- 4a.1. No file is saved
- 4a.2. `SAVE` button remains grayed
- 4a.3. No message displayed

**4b. File already exists**

- 4b.1. System dialog requests overwrite confirmation
- 4b.2. If user confirms, return to step 5
- 4b.3. If user cancels, return to step 4

**10a. User clicks on SAVE AS again**

- 10a.1. New dialog opens
- 10a.2. User selects `/Music/NewPatches/`
- 10a.3. Enters `BetterBrass.syx`
- 10a.4. New path `/Music/NewPatches/BetterBrass.syx` replaces previous one in Preferences
- 10a.5. Next `SAVE` will overwrite `/Music/NewPatches/BetterBrass.syx`

### Postconditions

- Patch is saved to disk
- Full path of last saved file is stored
- `SAVE` button allows quick subsequent save

---

## UC-09: LEGATO PORTA / KEYBOARD MODE constraint

**Actor:** User  
**Objective:** Edit LEGATO PORTA parameter respecting business constraints  
**Trigger:** Modification of KEYBOARD MODE or LEGATO PORTA

### Preconditions

- Plugin is launched
- A patch is being edited

### Nominal scenario

1. User selects `KEYBOARD MODE` = `UNISON`
2. Plugin activates `LEGATO PORTA` control (no longer grayed)
3. User activates `LEGATO PORTA` = `ON`
4. Plugin sends corresponding SysEx
5. Synth applies configuration

### Alternative scenarios

**1a. User selects KEYBOARD MODE ≠ UNISON**

- 1a.1. Plugin automatically grays `LEGATO PORTA` control
- 1a.2. `LEGATO PORTA` is not modifiable

**3a. LEGATO PORTA = ON and user changes KEYBOARD MODE from UNISON to another value**

- 3a.1. Plugin detects inconsistency
- 3a.2. Plugin forces `LEGATO PORTA` = `OFF`
- 3a.3. Plugin sends corresponding SysEx for LEGATO PORTA
- 3a.4. Message bar displays: `"LEGATO PORTA can only be activated if KEYBOARD MODE = UNISON"`
- 3a.5. `LEGATO PORTA` control becomes grayed

### Postconditions

- Business constraint is respected
- User is informed in case of automatic modification

---

## UC-10: Validate .syx file before loading

**Actor:** User  
**Objective:** Ensure a .syx file is valid before loading it  
**Trigger:** Click on `OPEN` button of COMPUTER PATCHES module

### Preconditions

- Plugin is launched
- User has .syx files on their computer

### Nominal scenario

1. User clicks on `OPEN` button
2. System dialog opens
3. User selects folder `/Music/SynthPatches/`
4. Plugin scans all .syx files in folder
5. Plugin validates each file (F0...F7 structure + Oberheim checksum)
6. Plugin finds 7 files: 5 valid, 2 invalid
7. Plugin sorts 5 valid files alphabetically
8. Plugin loads first valid patch from list
9. Plugin updates GUI with patch parameters
10. `Patch File` ComboBox displays list of 5 valid patches only
11. Message bar displays: `"Found 7 .syx files (5 valid, 2 invalid)"`

### Alternative scenarios

**6a. No valid files found**

- 6a.1. ComboBox remains grayed
- 6a.2. ComboBox displays `"No Patch !"`
- 6a.3. `<` and `>` buttons remain grayed
- 6a.4. Message bar displays: `"Found 3 .syx files (0 valid, 3 invalid)"`

**6b. All files are valid**

- 6b.1. All files are displayed in ComboBox
- 6b.2. Message bar displays: `"Found 10 .syx files (10 valid, 0 invalid)"`

**5a. Corrupted file detected**

- 5a.1. Invalid SysEx structure (not F0...F7)
- 5a.2. File is ignored (not displayed in ComboBox)
- Return to step 6

**5b. Incorrect checksum**

- 5b.1. Oberheim algorithm detects error
- 5b.2. File is ignored
- Return to step 6

### Postconditions

- Only valid .syx files are displayed and loaded
- User is informed of scan result

---

## UC-11: Synth auto-detection at startup

**Actor:** System (automatic)  
**Objective:** Verify that connected MIDI device is an Oberheim Matrix-1000  
**Trigger:** Plugin startup or change of selected MIDI port

### Preconditions

- Plugin is launched
- A MIDI output port is selected

### Nominal scenario

1. Plugin at startup executes synth auto-detection
2. Plugin sends Device Inquiry request to selected MIDI port
3. Plugin waits for synth Device ID response (2 second timeout)
4. Plugin analyzes received response and verifies identification criteria:
   - Manufacturer ID = 10H (Oberheim)
   - Family = 06H (Matrix-6/6R/1000)
   - Member = 02H (Matrix-1000)
5. All criteria validated ✓
6. Plugin displays: `"Matrix-1000 detected. Version: X.XX"`
7. Plugin enables all synth controls (sliders, buttons, etc.)
8. Plugin proceeds with initial FETCH of current patch (request type 4)
9. Plugin proceeds with initial FETCH of master parameters (request type 3)
10. Message bar displays: `"Synth connected and synchronized"`

### Alternative scenarios

**2a. No MIDI port selected**

- 2a.1. Plugin cannot execute auto-detection
- 2a.2. Plugin displays: `"No MIDI output port selected"`
- 2a.3. Synth controls remain grayed

**3a. MIDI timeout (synth does not respond within 2 seconds)**

- 3a.1. Device Inquiry request remains unanswered
- 3a.2. Plugin displays: `"Connected device not responding. Is it a Matrix-1000?"`
- 3a.3. Synth controls remain grayed
- 3a.4. User can retry by changing port

**4a. Incorrect Manufacturer ID**

- 4a.1. Example: device responded with Manufacturer ID = FFH
- 4a.2. Plugin displays: `"Connected device is not a Matrix-1000. Please check your connection."`
- 4a.3. Synth controls remain grayed

**4b. Incorrect Family or Member**

- 4b.1. Example: it's a Matrix-6 or Matrix-6R (not a Matrix-1000)
- 4b.2. Plugin displays: `"Connected device is not a Matrix-1000. Please check your connection."`
- 4b.3. Synth controls remain grayed

**8a. Timeout during FETCH of current patch**

- 8a.1. Synth does not respond to request type 4
- 8a.2. Plugin displays: `"Failed to fetch current patch from synth"`
- 8a.3. Controls remain grayed
- 8a.4. User can retry manually

**9a. Timeout during FETCH of master parameters**

- 9a.1. Synth does not respond to request type 3
- 9a.2. Plugin displays: `"Failed to fetch master parameters from synth"`
- 9a.3. Controls remain grayed
- 9a.4. User can retry manually

### Postconditions

- Connected synth is confirmed as Matrix-1000 (or error message)
- Synth controls are enabled/disabled accordingly
- Current patch and master parameters are synchronized (or not)
- Plugin is ready for editing (nominal case) or waits for user action (error case)

---

Copyright © 2025 Ten Square Software. All rights reserved.

