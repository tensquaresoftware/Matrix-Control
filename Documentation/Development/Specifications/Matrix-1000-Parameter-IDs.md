# Matrix Control | Synth Parameter IDs

---

- **Author:** Guillaume DUPONT
- **Organization:** Ten Square Software
- **Revision date:** 2025-08-22

---

The parameter IDs below are valid for the PATCHES mode of the Oberheim Matrix-1000, Matrix-6 and Matrix-6R synths.

## PATCH EDIT Section

### DCO 1 Module

- dco1Frequency
- dco1FrequencyModByLfo1
- dco1Sync
- dco1PulseWidth
- dco1PulseWidthModByLfo2
- dco1WaveShape
- dco1WaveSelect
- dco1Levers
- dco1KeyboardPortamento
- dco1KeyClick

### DCO 2 Module

- dco2Frequency
- dco2FrequencyModByLfo1
- dco2Detune
- dco2PulseWidth
- dco2PulseWidthModByLfo2
- dco2WaveShape
- dco2WaveSelect
- dco2Levers
- dco2KeyboardPortamento
- dco2KeyClick

### VCF/VCA Module

- vcfBalance
- vcfFequency
- vcfFrequencyModByEnv1
- vcfFrequencyModByPressure
- vcfResonance
- vcfLevers
- vcfKeyboardPortamento
- vca1Volume
- vca1ModByVelocity
- vca2ModByEnv2

### FM/Track Module

- fmAmount
- fmModByEnv3
- fmModByPressure
- trackInput
- trackPoint1
- trackPoint2
- trackPoint3
- trackPoint4
- trackPoint5

### RAMP/PORTAMENTO Module

- ramp1Rate
- ramp1Trigger
- ramp2Rate
- ramp2Trigger
- portamentoRate
- portamentoModByVelocity
- portamentoMode
- portamentoLegato
- portamentoKeyboardMode 

### ENV 1 Module

- env1Delay
- env1Attack
- env1Decay
- env1Sustain
- env1Release
- env1Amplitude
- env1AmplitudeModByVelocity
- env1TriggerMode
- env1EnvelopeMode
- env1Lfo1Trigger

### ENV 2 Module

- env2Delay
- env2Attack
- env2Decay
- env2Sustain
- env2Release
- env2Amplitude
- env2AmplitudeModByVelocity
- env2TriggerMode
- env2EnvelopeMode
- env2Lfo2Trigger

### ENV 3 Module

- env3Delay
- env3Attack
- env3Decay
- env3Sustain
- env3Release
- env3Amplitude
- env3AmplitudeModByVelocity
- env3TriggerMode
- env3EnvelopeMode
- env3Lfo3Trigger

### LFO 1 Module

- lfo1Speed
- lfo1SpeedModByPressure
- lfo1Waveform
- lfo1RetriggerPoint
- lfo1Amplitude
- lfo1AmplitudeModByRamp1
- lfo1TriggerMode
- lfo1Lag
- lfo1SampleInput

### LFO 2 Module

- lfo2Speed
- lfo2SpeedModByKeyboard
- lfo2Waveform
- lfo2RetriggerPoint
- lfo2Amplitude
- lfo2AmplitudeModByRamp2
- lfo2TriggerMode
- lfo2Lag
- lfo2SampleInput

## MATRIX MODULATION Section

There are 10 buses in total in this section, they are numbered from 0 to 9. Simply replace the "X" below with this number to generate the IDs for the 30 parameters (3 x 10) of the MATRIX MODULATION section.

### Bus X

- matrixModBusXSource
- matrixModBusXAmount
- matrixModBusXDestination

---

Copyright © 2025 Ten Square Software. All rights reserved.