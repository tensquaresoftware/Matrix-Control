# Matrix-Control / Synth Parameters & GUI Logic

---

- **Author:** Guillaume DUPONT
- **Organization:** Ten Square Software
- **Revision date:** 2025-12-30

---

## Introduction

This document centralizes:

- The list of Oberheim Matrix-1000 parameters
- Their mapping to the Matrix-Control plugin GUI controls
- A list of additional GUI controls (Patch Manager, Utilities, etc.)
- The GUI logic

## Synth parameters and mapped GUI controls

The synth parameters are divided into 2 modes :

- PATCH
- MASTER

Each mode contains sections :

- PATCH mode : PATCH EDIT, MATRIX MODULATION, PATCH MANAGER (this last one is a specific extension for the Matrix-Control plugin)

- MASTER mode : MASTER EDIT

### PATCH EDIT

The PATCH EDIT section is divided into 10 parameter blocks :

- DCO 1
- DCO 2
- VCF/VCA
- FM/TRACK
- RAMP/PORTAMENTO
- ENV 1
- ENV 2
- ENV 3
- LFO 1
- LFO 2

#### DCO 1

| Parameter               | Type | Values                                                         | SysEx Offset | SysEx ID  | GUI Component                                    | Displayed Name      |
| ----------------------- | ---- | -------------------------------------------------------------- | ------------ | --------- | ------------------------------------------------ | ------------------- |
| dco1Frequency           | int  | Min = 0<br>Max = 63<br>Default = 0                             | 9            | 0<br>0x00 | Slider                                           | FREQUENCY           |
| dco1FrequencyModByLfo1  | int  | Min = -63<br>Max = 63<br>Default = 0                           | 86           | 1<br>0x01 | Slider                                           | FREQ MOD BY LFO 1   |
| dco1Sync                | enum | Items:<br>Off, (Default)<br>Soft,<br>Medium,<br>Hard           | 25           | 2<br>0x02 | ComboBox:<br>OFF,<br>SOFT,<br>MEDIUM,<br>HARD    | SYNC                |
| dco1PulseWidth          | int  | Min = 0<br>Max = 63<br>Default = 31                            | 11           | 3<br>0x03 | Slider                                           | PULSE WIDTH         |
| dco1PulseWidthModByLfo2 | int  | Min = -63<br>Max = 63<br>Default = 0                           | 87           | 4<br>0x04 | Slider                                           | PW MOD BY LFO 2     |
| dco1WaveShape           | int  | Min = 0<br>Max = 63<br>Default = 31                            | 10           | 5<br>0x05 | Slider                                           | WAVE SHAPE          |
| dco1WaveSelect          | enum | Items:<br>Off,<br>Pulse,<br>Wave, (Default)<br>Both            | 13           | 6<br>0x06 | ComboBox:<br>OFF,<br>PULSE,<br>WAVE,<br>BOTH     | WAVE SELECT         |
| dco1Levers              | enum | Items:<br>Off,<br>L1PitchBend,<br>L2Vibrato,<br>Both (Default) | 12           | 7<br>0x07 | ComboBox:<br>OFF,<br>L1/BEND,<br>L2/VIB,<br>BOTH | LEVERS              |
| dco1KeyboardPortamento  | enum | Items:<br>Keyboard, (Default)<br>Portamento                    | 21           | 8<br>0x08 | ComboBox:<br>KEYBD,<br>PORTA,                    | KEYBOARD/PORTAMENTO |
| dco1KeyClick            | enum | Items:<br>Off, (Default)<br>On                                 | 22           | 9<br>0x09 | ComboBox:<br>OFF,<br>ON                          | KEY CLICK           |

#### DCO 2

| Parameter               | Type | Values                                                         | SysEx Offset | SysEx ID   | GUI Component                                          | Displayed Name      |
| ----------------------- | ---- | -------------------------------------------------------------- | ------------ | ---------- | ------------------------------------------------------ | ------------------- |
| dco2Frequency           | int  | Min = 0<br>Max = 63<br>Default = 0                             | 14           | 10<br>0x0A | Slider                                                 | FREQUENCY           |
| dco2FrequencyModByLfo1  | int  | Min = -63<br>Max = 63<br>Default = 0                           | 88           | 11<br>0x0B | Slider                                                 | FREQ MOD BY LFO 1   |
| dco2Detune              | int  | Min = -31<br>Max = 31<br>Default = 2                           | 19           | 12<br>0x0C | Slider                                                 | DETUNE              |
| dco2PulseWidth          | int  | Min = 0<br>Max = 63<br>Default = 24                            | 16           | 13<br>0x0D | Slider                                                 | PULSE WIDTH         |
| dco2PulseWidthModByLfo2 | int  | Min = -63<br>Max = 63<br>Default = 0                           | 89           | 14<br>0x0E | Slider                                                 | PW MOD BY LFO 2     |
| dco2WaveShape           | int  | Min = 0<br>Max = 63<br>Default = 31                            | 15           | 15<br>0x0F | Slider                                                 | WAVE SHAPE          |
| dco2WaveSelect          | enum | Items:<br>Off,<br>Pulse, (Default)<br>Wave,<br>Both,<br>Noise  | 18           | 16<br>0x10 | ComboBox:<br>OFF,<br>PULSE,<br>WAVE,<br>BOTH,<br>NOISE | WAVE SELECT         |
| dco2Levers              | enum | Items:<br>Off,<br>L1PitchBend,<br>L2Vibrato,<br>Both (Default) | 17           | 17<br>0x11 | ComboBox:<br>OFF,<br>L1/BEND,<br>L2/VIB,<br>BOTH       | LEVERS              |
| dco2KeyboardPortamento  | enum | Items:<br>Off,<br>Portamento,<br>Keyboard (Default)            | 23           | 18<br>0x12 | ComboBox:<br>OFF,<br>PORTA,<br>KEYBD                   | KEYBOARD/PORTAMENTO |
| dco2KeyClick            | enum | Items:<br>Off, (Default)<br>On                                 | 24           | 19<br>0x13 | ComboBox:<br>OFF,<br>ON                                | KEY CLICK           |

#### VCF/VCA

| Parameter                 | Type | Values                                                         | SysEx Offset | SysEx ID   | GUI Component                                    | Displayed Name        |
| ------------------------- | ---- | -------------------------------------------------------------- | ------------ | ---------- | ------------------------------------------------ | --------------------- |
| vcfBalance                | int  | Min = 0<br>Max = 63<br>Default = 31                            | 20           | 20<br>0x14 | Slider                                           | BALANCE               |
| vcfFequency               | int  | Min = 0<br>Max = 127<br>Default = 55                           | 26           | 21<br>0x15 | Slider                                           | FREQUENCY             |
| vcfFrequencyModByEnv1     | int  | Min = -63<br>Max = 63<br>Default = 42                          | 90           | 22<br>0x16 | Slider                                           | FREQ MOD BY ENV 1     |
| vcfFrequencyModByPressure | int  | Min = -63<br>Max = 63<br>Default = 0                           | 91           | 23<br>0x17 | Slider                                           | FREQ MOD BY PRESSURE  |
| vcfResonance              | int  | Min = 0<br>Max = 63<br>Default = 0                             | 27           | 24<br>0x18 | Slider                                           | RESONANCE             |
| vcfLevers                 | enum | Items:<br>Off (Default),<br>L1PitchBend,<br>L2Vibrato,<br>Both | 28           | 25<br>     | ComboBox:<br>OFF,<br>L1/BEND,<br>L2/VIB,<br>BOTH | LEVERS                |
| vcfKeyboardPortamento     | enum | Items:<br>Off,<br>Portamento,<br>Keyboard (Default)            | 29           | 26<br>0x1A | ComboBox:<br>OFF,<br>PORTA,<br>KEYBD             | KEYBOARD/PORTAMENTO   |
| vca1Volume                | int  | Min = 0<br>Max = 63<br>Default = 63                            | 31           | 27<br>0x1B | Slider                                           | VCA 1 VOLUME          |
| vca1ModByVelocity         | int  | Min = -63<br>Max = 63<br>Default = 0                           | 92           | 28<br>0x1C | Slider                                           | VCA 1 MOD BY VELOCITY |
| vca2ModByEnv2             | int  | Min = -63<br>Max = 63<br>Default = 63                          | 93           | 29<br>0x1D | Slider                                           | VCA 2 MOD BY ENV 2    |

#### FM/TRACK

| Parameter       | Type | Values                                                                                                                                                                                                                                                                                       | SysEx Offset | SysEx ID   | GUI Component                                                                                                                                                                                                                                     | Displayed Name     |
| --------------- | ---- | -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- | ------------ | ---------- | ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- | ------------------ |
| fmAmount        | int  | Min = 0<br>Max = 63<br>Default = 0                                                                                                                                                                                                                                                           | 30           | 30<br>0x1E | Slider                                                                                                                                                                                                                                            | VCF FM AMOUNT      |
| fmModByEnv3     | int  | Min = -63<br>Max = 63<br>Default = 0                                                                                                                                                                                                                                                         | 100          | 31<br>0x1F | Slider                                                                                                                                                                                                                                            | FM MOD BY ENV 3    |
| fmModByPressure | int  | Min = -63<br>Max = 63<br>Default = 0                                                                                                                                                                                                                                                         | 101          | 32<br>0x20 | Slider                                                                                                                                                                                                                                            | FM MOD BY PRESSURE |
| trackInput      | enum | Items:<br>None,<br>Envelope1,<br>Envelope2,<br>Envelope3,<br>Lfo1,<br>Lfo2,<br>Vibrato,<br>Ramp1,<br>Ramp2,<br>Keyboard, (Default)<br>Portamento,<br>TrackingGenerator,<br>KeyboardGate,<br>Velocity,<br>ReleaseVelocity,<br>Pressure,<br>Pedal1,<br>Pedal2,<br>Lever1,<br>Lever2,<br>Lever3 | 76           | 33<br>0x21 | ComboBox:<br>NONE,<br>ENV 1,<br>ENV 2,<br>ENV 3,<br>LFO 1,<br>LFO 2,<br>VIBRATO,<br>RAMP 1,<br>RAMP 2,<br>KEYBD,<br>PORTA,<br>TRACK,<br>KB GATE,<br>VELOCITY,<br>REL VEL,<br>PRESSURE,<br>PEDAL 1,<br>PEDAL 2,<br>LEVER 1,<br>LEVER 2,<br>LEVER 3 | TRACK INPUT        |
| trackPoint1     | int  | Min = 0<br>Max = 63<br>Default = 0                                                                                                                                                                                                                                                           | 77           | 34<br>0x22 | Slider                                                                                                                                                                                                                                            | TRACK POINT 1      |
| trackPoint2     | int  | Min = 0<br>Max = 63<br>Default = 15                                                                                                                                                                                                                                                          | 78           | 35<br>0x23 | Slider                                                                                                                                                                                                                                            | TRACK POINT 2      |
| trackPoint3     | int  | Min = 0<br>Max = 63<br>Default = 31                                                                                                                                                                                                                                                          | 79           | 36<br>0x24 | Slider                                                                                                                                                                                                                                            | TRACK POINT 3      |
| trackPoint4     | int  | Min = 0<br>Max = 63<br>Default = 47                                                                                                                                                                                                                                                          | 80           | 37<br>0x25 | Slider                                                                                                                                                                                                                                            | TRACK POINT 4      |
| trackPoint5     | int  | Min = 0<br>Max = 63<br>Default = 63                                                                                                                                                                                                                                                          | 81           | 38<br>0x26 | Slider                                                                                                                                                                                                                                            | TRACK POINT 5      |

#### RAMP/PORTAMENTO

| Parameter               | Type | Values                                                                                                         | SysEx Offset | SysEx ID   | GUI Component                                        | Displayed Name       |
| ----------------------- | ---- | -------------------------------------------------------------------------------------------------------------- | ------------ | ---------- | ---------------------------------------------------- | -------------------- |
| ramp1Rate               | int  | Min = 0<br>Max = 63<br>Default = 0                                                                             | 82           | 40<br>0x28 | Slider                                               | RAMP 1 RATE          |
| ramp1Trigger            | enum | Items:<br>SingleTrigger (Default)<br>MultipleTrigger,<br>ExternalSingleTrigger,<br>GatedExternalSingleTrigger  | 83           | 41<br>0x29 | ComboBox:<br>STRIG,<br>MTRIG,<br>EXTRIG,<br>GATEDX   | RAMP 1 TRIGGER       |
| ramp2Rate               | int  | Min = 0<br>Max = 63<br>Default = 0                                                                             | 84           | 42<br>0x2A | Slider                                               | RAMP 2 RATE          |
| ramp2Trigger            | enum | Items:<br>SingleTrigger, (Default)<br>MultipleTrigger,<br>ExternalSingleTrigger,<br>GatedExternalSingleTrigger | 85           | 43<br>0x2B | ComboBox:<br>STRIG,<br>MTRIG,<br>EXTRIG,<br>GATEDX   | RAMP 2 TRIGGER       |
| portamentoRate          | int  | Min = 0<br>Max = 63<br>Default = 0                                                                             | 32           | 44<br>0X2C | Slider                                               | PORTAMENTO RATE      |
| portamentoModByVelocity | int  | Min = -63<br>Max = 63<br>Default = 0                                                                           | 99           | 45<br>0x2D | Slider                                               | PORT MOD BY VELOCITY |
| portamentoMode          | enum | Items:<br>Linear, (Default)<br>ConstantTime,<br>Exponential                                                    | 33           | 46<br>0x2E | ComboBox:<br>LINEAR,<br>CONST,<br>EXPO               | PORTAMENTO MODE      |
| portamentoLegato        | enum | Items:<br>Off, (Default)<br>On                                                                                 | 34           | 47<br>0x2F | ComboBox:<br>OFF,<br>ON                              | LEGATO PORTAMENTO    |
| portamentoKeyboardMode  | enum | Items:<br>Rotate,<br>Reassign, (Default)<br>Unison,<br>ReassignRob                                             | 8            | 48<br>0x30 | ComboBox:<br>ROTATE,<br>REASGN,<br>UNISON,<br>REAROB | KEYBOARD MODE        |

#### ENV 1

| Parameter                  | Type | Values                                                                                                                                                                                                                | SysEx Offset | SysEx ID   | GUI Component                                                                                | Displayed Name      |
| -------------------------- | ---- | --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- | ------------ | ---------- | -------------------------------------------------------------------------------------------- | ------------------- |
| env1Delay                  | int  | Min = 0<br>Max = 63<br>Default = 0                                                                                                                                                                                    | 50           | 50<br>0x32 | Slider                                                                                       | DELAY               |
| env1Attack                 | int  | Min = 0<br>Max = 63<br>Default = 0                                                                                                                                                                                    | 51           | 51<br>0x33 | Slider                                                                                       | ATTACK              |
| env1Decay                  | int  | Min = 0<br>Max = 63<br>Default = 10                                                                                                                                                                                   | 52           | 52<br>0x34 | Slider                                                                                       | DECAY               |
| env1Sustain                | int  | Min = 0<br>Max = 63<br>Default = 50                                                                                                                                                                                   | 53           | 53<br>0x35 | Slider                                                                                       | SUSTAIN             |
| env1Release                | int  | Min = 0<br>Max = 63<br>Default = 10                                                                                                                                                                                   | 54           | 54<br>0x36 | Slider                                                                                       | RELEASE             |
| env1Amplitude              | int  | Min = 0<br>Max = 63<br>Default = 40                                                                                                                                                                                   | 55           | 55<br>0x37 | Slider                                                                                       | AMPLITUDE           |
| env1AmplitudeModByVelocity | int  | Min = -63<br>Max = 63<br>Default = 63                                                                                                                                                                                 | 94           | 56<br>0x38 | Slider                                                                                       | AMP MOD BY VELOCITY |
| env1TriggerMode            | enum | Items:<br>SingleTrigger, (Default)<br>MultipleTrigger,<br>SingleTriggerReset,<br>MultipleTriggerReset,<br>ExternalSingleTrigger,<br>ExternalMultipleTrigger,<br>ExternalTriggerReset,<br>ExternalMultipleTriggerReset | 49           | 57<br>0x39 | ComboBox:<br>STRIG,<br>MTRIG,<br>SRESET,<br>MRESET,<br>XTRIG,<br>XMTRIG,<br>XRESET,<br>XMRST | TRIGGER MODE        |
| env1EnvelopeMode           | enum | Items:<br>Normal, (Default)<br>DelayAttackDecayRelease,<br>Freerun,<br>Both                                                                                                                                           | 57           | 58<br>0x3A | ComboBox:<br>NORMAL,<br>DADR,<br>FREE,<br>BOTH                                               | ENVELOPE MODE       |
| env1Lfo1Trigger            | enum | Items:<br>Normal, (Default)<br>Lfo1,<br>GatedLfo1Trigger                                                                                                                                                              | 56           | 59<br>0x3B | ComboBox:<br>NORMAL,<br>LFO 1,<br>G-LFO 1                                                    | LFO 1 TRIGGER       |

#### ENV 2

| Parameter                  | Type | Values                                                                                                                                                                                                                | SysEx Offset | SysEx ID   | GUI Component                                                                                | Displayed Name      |
| -------------------------- | ---- | --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- | ------------ | ---------- | -------------------------------------------------------------------------------------------- | ------------------- |
| env2Delay                  | int  | Min = 0<br>Max = 63<br>Default = 0                                                                                                                                                                                    | 59           | 60<br>0x3C | Slider                                                                                       | DELAY               |
| env2Attack                 | int  | Min = 0<br>Max = 63<br>Default = 0                                                                                                                                                                                    | 60           | 61<br>0x3D | Slider                                                                                       | ATTACK              |
| env2Decay                  | int  | Min = 0<br>Max = 63<br>Default = 10                                                                                                                                                                                   | 61           | 62<br>0x3E | Slider                                                                                       | DECAY               |
| env2Sustain                | int  | Min = 0<br>Max = 63<br>Default = 50                                                                                                                                                                                   | 62           | 63<br>0x3F | Slider                                                                                       | SUSTAIN             |
| env2Release                | int  | Min = 0<br>Max = 63<br>Default = 10                                                                                                                                                                                   | 63           | 64<br>0x40 | Slider                                                                                       | RELEASE             |
| env2Amplitude              | int  | Min = 0<br>Max = 63<br>Default = 40                                                                                                                                                                                   | 64           | 65<br>0x41 | Slider                                                                                       | AMPLITUDE           |
| env2AmplitudeModByVelocity | int  | Min = -63<br>Max = 63<br>Default = 63                                                                                                                                                                                 | 95           | 66<br>0x42 | Slider                                                                                       | AMP MOD BY VELOCITY |
| env2TriggerMode            | enum | Items:<br>SingleTrigger, (Default)<br>MultipleTrigger,<br>SingleTriggerReset,<br>MultipleTriggerReset,<br>ExternalSingleTrigger,<br>ExternalMultipleTrigger,<br>ExternalTriggerReset,<br>ExternalMultipleTriggerReset | 58           | 67<br>0x43 | ComboBox:<br>STRIG,<br>MTRIG,<br>SRESET,<br>MRESET,<br>XTRIG,<br>XMTRIG,<br>XRESET,<br>XMRST | TRIGGER MODE        |
| env2EnvelopeMode           | enum | Items:<br>Normal, (Default)<br>DelayAttackDecayRelease,<br>Freerun,<br>Both                                                                                                                                           | 66           | 68<br>0x44 | ComboBox:<br>NORMAL,<br>DADR,<br>FREE,<br>BOTH                                               | ENVELOPE MODE       |
| env2Lfo1Trigger            | enum | Items:<br>Normal, (Default)<br>Lfo1,<br>GatedLfo1Trigger                                                                                                                                                              | 65           | 69<br>0x45 | ComboBox:<br>NORMAL,<br>LFO 1,<br>G-LFO 1                                                    | LFO 1 TRIGGER       |

#### ENV 3

| Parameter                  | Type | Values                                                                                                                                                                                                                | SysEx Offset | SysEx ID   | GUI Component                                                                                | Displayed Name      |
| -------------------------- | ---- | --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- | ------------ | ---------- | -------------------------------------------------------------------------------------------- | ------------------- |
| env3Delay                  | int  | Min = 0<br>Max = 63<br>Default = 0                                                                                                                                                                                    | 68           | 70<br>0x46 | Slider                                                                                       | DELAY               |
| env3Attack                 | int  | Min = 0<br>Max = 63<br>Default = 0                                                                                                                                                                                    | 69           | 71<br>0x47 | Slider                                                                                       | ATTACK              |
| env3Decay                  | int  | Min = 0<br>Max = 63<br>Default = 20                                                                                                                                                                                   | 70           | 72<br>0x48 | Slider                                                                                       | DECAY               |
| env3Sustain                | int  | Min = 0<br>Max = 63<br>Default = 0                                                                                                                                                                                    | 71           | 73<br>0x49 | Slider                                                                                       | SUSTAIN             |
| env3Release                | int  | Min = 0<br>Max = 63<br>Default = 20                                                                                                                                                                                   | 72           | 74<br>0x4A | Slider                                                                                       | RELEASE             |
| env3Amplitude              | int  | Min = 0<br>Max = 63<br>Default = 40                                                                                                                                                                                   | 73           | 75<br>0x4B | Slider                                                                                       | AMPLITUDE           |
| env3AmplitudeModByVelocity | int  | Min = -63<br>Max = 63<br>Default = 63                                                                                                                                                                                 | 96           | 76<br>0x4C | Slider                                                                                       | AMP MOD BY VELOCITY |
| env3TriggerMode            | enum | Items:<br>SingleTrigger, (Default)<br>MultipleTrigger,<br>SingleTriggerReset,<br>MultipleTriggerReset,<br>ExternalSingleTrigger,<br>ExternalMultipleTrigger,<br>ExternalTriggerReset,<br>ExternalMultipleTriggerReset | 67           | 77<br>0x4D | ComboBox:<br>STRIG,<br>MTRIG,<br>SRESET,<br>MRESET,<br>XTRIG,<br>XMTRIG,<br>XRESET,<br>XMRST | TRIGGER MODE        |
| env3EnvelopeMode           | enum | Items:<br>Normal, (Default)<br>DelayAttackDecayRelease,<br>Freerun,<br>Both                                                                                                                                           | 75           | 78<br>0x4E | ComboBox:<br>NORMAL,<br>DADR,<br>FREE,<br>BOTH                                               | ENVELOPE MODE       |
| env3Lfo1Trigger            | enum | Items:<br>Normal, (Default)<br>Lfo1,<br>GatedLfo1Trigger                                                                                                                                                              | 74           | 79<br>0x4F | ComboBox:<br>NORMAL,<br>LFO 1,<br>GATED LFO 1 TRIGGER                                        | LFO 1 TRIGGER       |

#### LFO 1

| Parameter               | Type | Values                                                                                                                                                                                                                                                                                       | SysEx Offset | SysEx ID   | GUI Component                                                                                                                                                                                                                                     | Displayed Name        |
| ----------------------- | ---- | -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- | ------------ | ---------- | ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- | --------------------- |
| lfo1Speed               | int  | Min = 0<br>Max = 63<br>Default = 40                                                                                                                                                                                                                                                          | 35           | 80<br>0x50 | Slider                                                                                                                                                                                                                                            | SPEED                 |
| lfo1SpeedModByPressure  | int  | Min = -63<br>Max = 63<br>Default = 0                                                                                                                                                                                                                                                         | 102          | 81<br>0x51 | Slider                                                                                                                                                                                                                                            | SPEED MOD BY PRESSURE |
| lfo1Waveform            | enum | Items:<br>Triangle, (Default)<br>UpSawtooth,<br>DownSawtooth,<br>Square,<br>Random,<br>Noise,<br>SampledModulation                                                                                                                                                                           | 38           | 82<br>0x52 | ComboBox:<br>TRIANGLE,<br>UPSAW,<br>DNSAW,<br>SQUARE,<br>RANDOM,<br>NOISE,<br>SAMPLED                                                                                                                                                             | WAVEFORM              |
| lfo1RetriggerPoint      | int  | Min = 0<br>Max = 63<br>Default = 0                                                                                                                                                                                                                                                           | 39           | 83<br>0x53 | Slider                                                                                                                                                                                                                                            | RETRIGGER POINT       |
| lfo1Amplitude           | int  | Min = 0<br>Max = 63<br>Default = 0                                                                                                                                                                                                                                                           | 41           | 84<br>0x54 | Slider                                                                                                                                                                                                                                            | AMPLITUDE             |
| lfo1AmplitudeModByRamp1 | int  | Min = -63<br>Max = 63<br>Default = 63                                                                                                                                                                                                                                                        | 97           | 85<br>0x55 | Slider                                                                                                                                                                                                                                            | AMP MOD BY RAMP 1     |
| lfo1TriggerMode         | enum | Items:<br>Off, (Default)<br>SingleTrigger,<br>MultipleTrigger,<br>ExternalSingleTrigger                                                                                                                                                                                                      | 36           | 86<br>0x56 | ComboBox:<br>OFF,<br>STRIG,<br>MTRIG,<br>XTRIG                                                                                                                                                                                                    | TRIGGER MODE          |
| lfo1Lag                 | enum | Items:<br>Off, (Default)<br>On                                                                                                                                                                                                                                                               | 37           | 87<br>0x57 | ComboBox:<br>OFF,<br>ON                                                                                                                                                                                                                           | LAG                   |
| lfo1SampleInput         | enum | Items:<br>None,<br>Envelope1,<br>Envelope2,<br>Envelope3,<br>Lfo1,<br>Lfo2,<br>Vibrato,<br>Ramp1,<br>Ramp2,<br>Keyboard, (Default)<br>Portamento,<br>TrackingGenerator,<br>KeyboardGate,<br>Velocity,<br>ReleaseVelocity,<br>Pressure,<br>Pedal1,<br>Pedal2,<br>Lever1,<br>Lever2,<br>Lever3 | 40           | 88<br>0x58 | ComboBox:<br>NONE,<br>ENV 1,<br>ENV 2,<br>ENV 3,<br>LFO 1,<br>LFO 2,<br>VIBRATO,<br>RAMP 1,<br>RAMP 2,<br>KEYBD,<br>PORTA,<br>TRACK,<br>KB GATE,<br>VELOCITY,<br>REL VEL,<br>PRESSURE,<br>PEDAL 1,<br>PEDAL 2,<br>LEVER 1,<br>LEVER 2,<br>LEVER 3 | SAMPLE INPUT          |

#### LFO 2

| Parameter               | Type | Values                                                                                                                                                                                                                                                                                       | SysEx Offset | SysEx ID   | GUI Component                                                                                                                                                                                                                                     | Displayed Name     |
| ----------------------- | ---- | -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- | ------------ | ---------- | ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- | ------------------ |
| lfo2Speed               | int  | Min = 0<br>Max = 63<br>Default = 30                                                                                                                                                                                                                                                          | 42           | 90<br>0x5A | Slider                                                                                                                                                                                                                                            | SPEED              |
| lfo2SpeedModByKeyboard  | int  | Min = -63<br>Max = 63<br>Default = 0                                                                                                                                                                                                                                                         | 103          | 91<br>0x5B | Slider                                                                                                                                                                                                                                            | SPEED MOD BY KEYBD |
| lfo2Waveform            | enum | Items:<br>Triangle, (Default)<br>UpSawtooth,<br>DownSawtooth,<br>Square,<br>Random,<br>Noise,<br>SampledModulation                                                                                                                                                                           | 45           | 92<br>0x5C | ComboBox:<br>TRIANGLE,<br>UPSAW,<br>DNSAW,<br>SQUARE,<br>RANDOM,<br>NOISE,<br>SAMPLED                                                                                                                                                             | WAVEFORM           |
| lfo2RetriggerPoint      | int  | Min = 0<br>Max = 63<br>Default = 0                                                                                                                                                                                                                                                           | 46           | 93<br>0x5D | Slider                                                                                                                                                                                                                                            | RETRIGGER POINT    |
| lfo2Amplitude           | int  | Min = 0<br>Max = 63<br>Default = 0                                                                                                                                                                                                                                                           | 48           | 94<br>0x5E | Slider                                                                                                                                                                                                                                            | AMPLITUDE          |
| lfo2AmplitudeModByRamp2 | int  | Min = -63<br>Max = 63<br>Default = 63                                                                                                                                                                                                                                                        | 98           | 95<br>0x5F | Slider                                                                                                                                                                                                                                            | AMP MOD BY RAMP 2  |
| lfo2TriggerMode         | enum | Items:<br>Off, (Default)<br>SingleTrigger,<br>MultipleTrigger,<br>ExternalSingleTrigger                                                                                                                                                                                                      | 43           | 96<br>0x60 | ComboBox:<br>OFF,<br>STRIG,<br>MTRIG,<br>XTRIG                                                                                                                                                                                                    | TRIGGER MODE       |
| lfo2Lag                 | enum | Items:<br>Off, (Default)<br>On                                                                                                                                                                                                                                                               | 44           | 97<br>0x61 | ComboBox:<br>OFF,<br>ON                                                                                                                                                                                                                           | LAG                |
| lfo2SampleInput         | enum | Items:<br>None,<br>Envelope1,<br>Envelope2,<br>Envelope3,<br>Lfo1,<br>Lfo2,<br>Vibrato,<br>Ramp1,<br>Ramp2,<br>Keyboard, (Default)<br>Portamento,<br>TrackingGenerator,<br>KeyboardGate,<br>Velocity,<br>ReleaseVelocity,<br>Pressure,<br>Pedal1,<br>Pedal2,<br>Lever1,<br>Lever2,<br>Lever3 | 47           | 98<br>0x62 | ComboBox:<br>NONE,<br>ENV 1,<br>ENV 2,<br>ENV 3,<br>LFO 1,<br>LFO 2,<br>VIBRATO,<br>RAMP 1,<br>RAMP 2,<br>KEYBD,<br>PORTA,<br>TRACK,<br>KB GATE,<br>VELOCITY,<br>REL VEL,<br>PRESSURE,<br>PEDAL 1,<br>PEDAL 2,<br>LEVER 1,<br>LEVER 2,<br>LEVER 3 | SAMPLE INPUT       |

### MATRIX MODULATION

The MATRIX MODULATION section is composed of 10 modulation busses, numbered from 0 to 9. These bussess are identical in their structure and operation.

#### Bus 0

| Parameter                | Type | Values                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                          | SysEx Offset | GUI Component                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                    | Displayed Name |
| ------------------------ | ---- | --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- | ------------ | -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- | -------------- |
| matrixModBus0Source      | enum | Items:<br>None, (Default)<br>Envelope1,<br>Envelope2,<br>Envelope3,<br>Lfo1,<br>Lfo2,<br>Vibrato,<br>Ramp1,<br>Ramp2,<br>Keyboard,<br>Portamento,<br>TrackingGenerator,<br>KeyboardGate,<br>Velocity,<br>ReleaseVelocity,<br>Pressure,<br>Pedal1,<br>Pedal2,<br>Lever1,<br>Lever2,<br>Lever3                                                                                                                                                                                                                                                                    | 104          | ComboBox:<br>NONE,<br>ENV 1,<br>ENV 2,<br>ENV 3,<br>LFO 1,<br>LFO 2,<br>VIBRATO,<br>RAMP 1,<br>RAMP 2,<br>KEYBD,<br>PORTA,<br>TRACK,<br>KB GATE,<br>VELOCITY,<br>REL VEL,<br>PRESSURE,<br>PEDAL 1,<br>PEDAL 2,<br>LEVER 1,<br>LEVER 2,<br>LEVER 3                                                                                                                                                                                                                                                                                                                                                                                | SOURCE         |
| matrixModBus0Amount      | int  | Min = -63<br>Max = 63<br>Default = 0                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                            | 105          | Slider                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                           | AMOUNT         |
| matrixModBus0Destination | enum | Items:<br>None, (Default)<br>Dco1Frequency,<br>Dco1PulseWidth,<br>Dco1WaveShape,<br>Dco2Frequency,<br>Dco2PulseWidth,<br>Dco2WaveShape,<br>VcfBalance,<br>VcfFmAmount,<br>VcfFrequency,<br>VcfResonance,<br>Vca1Volume,<br>Vca2Volume,<br>Env1Delay,<br>Env1Attack,<br>Env1Decay,<br>Env1Release,<br>Env1Amplitude,<br>Env2Delay,<br>Env2Attack,<br>Env2Decay,<br>Env2Release,<br>Env2Amplitude,<br>Env3Delay,<br>Env3Attack,<br>Env3Decay,<br>Env3Release,<br>Env3Amplitude,<br>Lfo1Speed,<br>Lfo1Amplitude,<br>Lfo2Speed,<br>Lfo2Amplitude,<br>PortamentoRate | 106          | ComboBox:<br>NONE,<br>DCO 1 FREQUENCY,<br>DCO 1 PULSE WIDTH,<br>DCO 1 WAVE SHAPE,<br>DCO 2 FREQUENCY,<br>DCO 2 PULSE WIDTH,<br>DCO 2 WAVE SHAPE,<br>DCO 1 - DCO 2 MIX,<br>VCF FM BY DCO 1,<br>VCF FREQUENCY,<br>VCF RESONANCE,<br>VCA 1 VOLUME,<br>VCA 2 VOLUME,<br>ENV 1 DELAY,<br>ENV 1 ATTACK,<br>ENV 1 DECAY,<br>ENV 1 RELEASE,<br>ENV 1 AMPLITUDE,<br>ENV 2 DELAY,<br>ENV 2 ATTACK,<br>ENV 2 DECAY,<br>ENV 2 RELEASE,<br>ENV 2 AMPLITUDE,<br>ENV 3 DELAY,<br>ENV 3 ATTACK,<br>ENV 3 DECAY,<br>ENV 3 RELEASE,<br>ENV 3 AMPLITUDE,<br>LFO 1 SPEED,<br>LFO 1 AMPLITUDE,<br>LFO 2 SPEED,<br>LFO 2 AMPLITUDE,<br>PORTAMENTO RATE | DESTINATION    |

#### Bus 1 to 9

Same Type, Values, GUI Component and Display Name per parameter, only SysEx Offsets change.

| Parameter                | SysEx Offset |
| ------------------------ | ------------ |
| matrixModBus1Source      | 107          |
| matrixModBus1Amount      | 108          |
| matrixModBus1Destination | 109          |
| matrixModBus2Source      | 110          |
| matrixModBus2Amount      | 111          |
| matrixModBus2Destination | 112          |
| matrixModBus3Source      | 113          |
| matrixModBus3Amount      | 114          |
| matrixModBus3Destination | 115          |
| matrixModBus4Source      | 116          |
| matrixModBus4Amount      | 117          |
| matrixModBus4Destination | 118          |
| matrixModBus5Source      | 119          |
| matrixModBus5Amount      | 120          |
| matrixModBus5Destination | 121          |
| matrixModBus6Source      | 122          |
| matrixModBus6Amount      | 123          |
| matrixModBus6Destination | 124          |
| matrixModBus7Source      | 125          |
| matrixModBus7Amount      | 126          |
| matrixModBus7Destination | 127          |
| matrixModBus8Source      | 128          |
| matrixModBus8Amount      | 129          |
| matrixModBus8Destination | 130          |
| matrixModBus9Source      | 131          |
| matrixModBus9Amount      | 132          |
| matrixModBus9Destination | 133          |

## Additional GUI Controls

### PATCH EDIT

#### DCO 1, DCO 2

| Function  | GUI Component | Displayed Name |
| --------- | ------------- | -------------- |
| Init DCO  | Button        | I              |
| Copy DCO  | Button        | C              |
| Paste DCO | Button        | P              |

#### ENV 1, ENV 2, ENV 3

| Function       | GUI Component | Displayed Name |
| -------------- | ------------- | -------------- |
| Init Envelope  | Button        | I              |
| Copy Envelope  | Button        | C              |
| Paste Envelope | Button        | P              |

#### LFO 1, LFO 2

| Function  | GUI Component | Displayed Name |
| --------- | ------------- | -------------- |
| Init LFO  | Button        | I              |
| Copy LFO  | Button        | C              |
| Paste LFO | Button        | P              |

### PATCH MANAGER

#### BANK UTILITY

| Function      | GUI Component | Displayed Name |
| ------------- | ------------- | -------------- |
| Unlock Bank   | Button        | UNLOCK         |
| Select Bank 0 | Button        | 0              |
| Select Bank 1 | Button        | 1              |
| Select Bank 2 | Button        | 2              |
| Select Bank 3 | Button        | 3              |
| Select Bank 4 | Button        | 4              |
| Select Bank 5 | Button        | 5              |
| Select Bank 6 | Button        | 6              |
| Select Bank 7 | Button        | 7              |
| Select Bank 8 | Button        | 8              |
| Select Bank 9 | Button        | 9              |

#### INTERNAL PATCHES

##### BROWSER

| Function              | GUI Component | Displayed Name         |
| --------------------- | ------------- | ---------------------- |
| Select Previous Patch | Button        | <                      |
| Select Next Patch     | Button        | >                      |
| Select Bank Number    | Number        | (Current bank number)  |
| Select Patch Number   | Number        | (Current patch number) |

##### UTILITY

| Function    | GUI Component | Displayed Name |
| ----------- | ------------- | -------------- |
| Init Patch  | Button        | INIT           |
| Copy Patch  | Button        | COPY           |
| Paste Patch | Button        | PASTE          |
| Store Patch | Button        | STORE          |

#### COMPUTER PATCHES

##### BROWSER

| Function              | GUI Component | Displayed Name / Displayed Text                    |
| --------------------- | ------------- | -------------------------------------------------- |
| Select Previous Patch | Button        | <                                                  |
| Select Next Patch     | Button        | >                                                  |
| Select Patch          | ComboBox      | (Name of patch selected in folder opened via OPEN) |

##### FOLDER & FILES

| Function      | GUI Component | Displayed Name |
| ------------- | ------------- | -------------- |
| Open Folder   | Button        | OPEN           |
| Save Patch As | Button        | SAVE AS        |
| Save Patch    | Button        | SAVE           |

## GUI Logic

### PATCH EDIT

#### DCO 1, DCO 2

**`I` Button (Init DCO)**

- Initializes all DCO parameters according to the "INIT" patch if present in the computer, or to the default values
- This button is always active

**`C` Button (Copy DCO)**

- Copies all parameters common to DCO 1 and DCO 2 to the internal clipboard
- This button is always active

**`P` Button (Paste DCO)**

- Pastes the clipboard settings into the DCO
- Only active if the clipboard contains valid DCO data
- No visual feedback for the user when the button is grayed out

#### ENV 1, ENV 2, ENV 3

**`I` Button (Init ENV)**

- Initializes all envelope parameters according to the "INIT" patch if present in the computer, or to the default values
- This button is always active

**`C` Button (Copy ENV)**

- Copies all envelope parameters to the internal clipboard
- This button is always active

**`P` Button (Paste ENV)**

- Pastes the clipboard settings into the envelope
- This button is only active if the clipboard contains valid envelope data
- No visual feedback for the user when the button is grayed out

#### LFO 1, LFO 2

**`I` Button (Init LFO)**

- Initializes all LFO parameters according to the "INIT" patch if present in the computer, or to the default values
- This button is always active

**`C` Button (Copy LFO)**

- Copies all parameters common to LFO 1 and LFO 2 to the internal clipboard
- This button is always active

**`P` Button (Paste LFO)**

- Pastes the clipboard settings into the LFO
- Only active if the clipboard contains valid LFO data
- No visual feedback for the user when the button is grayed out

#### RAMP/PORTAMENTO

- The `LEGATO PORTA` dropdown menu is clickable only if the `KEYBOARD MODE` menu = `UNISON`, otherwise it is grayed out
- No visual feedback for the user when the dropdown menu is grayed out

### MATRIX MODULATION

- The `DESTINATION` dropdown menu for busses 0 to 9 is clickable only if an item other than `NONE` has been previously selected in the corresponding `SOURCE` dropdown menu, otherwise it is grayed out.
- When a `DESTINATION` dropdown menu is grayed out, the previous selection in that menu is retained
- No visual feedback for the user when a `DESTINATION` dropdown menu is grayed out

### PATCH MANAGER

#### INTERNAL PATCHES

##### BROWSER

Banks are numbered from 0 to 9 and patches from 0 to 99. The `<` (Select Previous Patch) and `>` (Select Next Patch) navigation buttons implement wrap-around behavior :

- When the currently selected patch is the first in the list, pressing `<` will select the last patch
- Conversely, when the currently selected patch is the last in the list, pressing `>` will select the first patch
- This allows continuous, circular browsing through all available patches without interruption

#### COMPUTER PATCHES

##### BROWSER

This block allows the user to browse patches stored as files in SysEx format or internal plugin format (JSON/XML), in a folder on the computer. The `<` (Select Previous Patch) and `>` (Select Next Patch) navigation buttons implement wrap-around behavior :

- When the currently selected patch is the first in the list, pressing `<` will select the last patch

- Conversely, when the currently selected patch is the last in the list, pressing `>` will select the first patch

- This allows continuous, circular browsing through all available patches without interruption

- If the selected patch is the last in the list, clicking on `>` button selects the first patch

- If the selected patch is the first in the list, clicking on `<` button selects the last patch

- The `<` (Select Previous Patch) and `>` (Select Next Patch) buttons and the `SELECT PATCH` menu are grayed out by default

- They become clickable after a valid patch folder has been opened via the `OPEN` button (OPEN FOLDER)

##### FOLDER & FILES

This block allows selecting a computer folder containing patches and saving the current patch to a computer file.

- The `OPEN` button opens a standard operating system dialog box, allowing selection of a computer folder
- The `SAVE AS` button opens a standard operating system dialog box, allowing selection of a computer folder and entering a file name
- The `SAVE` button is grayed out by default. It becomes clickable after a patch has been saved on disk via the `SAVE AS` button, as a SysEx file in a computer folder

---

Copyright © 2025 Ten Square Software. All rights reserved.