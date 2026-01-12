# Matrix-Control | GUI Structure

---

- **Author:** Guillaume DUPONT
- **Organization:** Ten Square Software
- **Revision date:** 2026-01-09

---

GUI construction, according to the Figma model. 

# L1. MainComponent

Dimensions :

```
X: 0
Y: 0
W: 1335
H: 790
```

Contains :

```
HeaderPanel, BodyPanel, FooterPanel.
```

## L2. HeaderPanel

Dimensions :

```
X: 0
Y: 0
W: parent
H: 30
```

Contains :

```
TODO
```

## L2. BodyPanel

Dimensions :

```
Padding: 15
X: 0
Y: HeaderPanel.bottom
W: parent
H: 730
```

Contains :

```
PatchEditPanel, VerticalSeparator1, MatrixModulationPanel, PatchManagerPanel, VerticalSeparator2, MasterEditPanel.
```

### L3. PatchEditPanel

Dimensions :

```
X: BodyPanel.padding
Y: BodyPanel.padding
W: 810
H: 700
```

Contains :

```
SectionName, TopPanel, MiddlePanel, BottomPanel.
```

#### L4. TopPanel

Dimensions :

```
X: 0
Y: 0
W: parent
H: 280
```

Contains :

```
Dco1Panel, Dco2Panel, VcfVcaPanel, FmTrackPanel, RampPortamentoPanel.
```

#### L4. MiddlePanel

Dimensions :

```
X: 0
Y: TopPanel.bottom
W: parent
H: 110
```

Contains :

```
Env1DisplayPanel, Env2DisplayPanel, Env3DisplayPanel, TrackGeneratorDisplayPanel, PatchNameDisplayPanel.
```

#### L4. BottomPanel

Dimensions :

```
X: 0
Y: MiddlePanel.bottom
W: parent
H: 280
```

Contains :

```
Env1Panel, Env2Panel, Env3Panel, Lfo1Panel, Lfo2Panel.
```

### L3. VerticalSeparator1

Dimensions :

```
X: PatchEditPanel.right
Y: BodyPanel.padding
W: 30
H: 700
```

Contains :

```
No other panel.
```

### L3. MatrixModulationPanel

Dimensions :

```
X: VerticalSeparator1.right
Y: BodyPanel.padding
W: 275
H: 325
```

Contains :

```
SectionName, ModulationBusHeader, Modulation Busses 0 to 9.
```

### L3. PatchManagerPanel

Dimensions :

```
X: VerticalSeparator1.right
Y: MatrixModulationPanel.bottom
W: 275
H: 375
```

Contains :

```
SectionName, BankUtilityPanel, InternalPatchesPanel, ComputerPatchesPanel.
```

#### L4. BankUtilityPanel

Dimensions :

```
X: 0
Y: SectionName.bottom
W: 275
H: 115
```

Contains :

```
XXX
```

#### L4. InternalPatchesPanel

Dimensions :

```
X: 0
Y: BankUtilityPanel.bottom
W: 275
H: 115
```

Contains :

```
XXX
```

#### L4. ComputerPatchesPanel

Dimensions :

```
X: 0
Y: InternalPatchesPanel.bottom
W: 275
H: 115
```

Contains :

```
XXX
```

### L3. VerticalSeparator2

Dimensions :

```
X: MatrixModulationPanel.right
Y: BodyPanel.padding
W: 30
H: 700
```

Contains :

```
No other panel.
```

### L3. MasterEditPanel

Dimensions :

```
X: VerticalSeparator2.right
Y: BodyPanel.padding
W: 160
H: 700
```

Contains :

```
SectionName, MidiPanel, VibratoPanel, MiscPanel.
```

#### L4. MidiPanel

Dimensions :

```
X: 0
Y: Section.bottom
W: parent
H: 245
```

Contains :

```
XXX
```

#### L4. VibratoPanel

Dimensions :

```
X: 0
Y: MidiPanel.bottom
W: parent
H: 220
```

Contains :

```
XXX
```

#### L4. MiscPanel

Dimensions :

```
X: 0
Y: VibratoPanel.bottom
W: parent
H: 205
```

Contains :

```
XXX
```

## L2. FooterPanel

Dimensions :

```
X: 0
Y: BodyPanel.bottom
W: parent
H: 30
```

Contains :

```
TODO
```

---

Copyright © 2026 Ten Square Software. All rights reserved.
