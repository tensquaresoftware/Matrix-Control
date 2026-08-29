# Inventaire des paramètres — Patch Edit & Matrix Modulation

Document de référence pour la calibration « playable » du Patch Mutator (éviter les patches morts / silencieux).

**Sources Oberheim :** implémentation MIDI/SysEx Matrix-1000 (manuel officiel) et Matrix-6/6R (même format de patch single), notes personnelles VCA cascade.  
**Sources Matrix-Control :** libellés UI (`PluginDisplayNames.h`) et identifiants APVTS (`PluginIDs.h`).

**Légende colonne 4 (extinction du son) :**

| Valeur | Sens |
|--------|------|
| **Oui** | Peut à lui seul (ou en valeur extrême typique) rendre le patch inaudible |
| **Oui (conditionnel)** | Peut éteindre le son seulement si d’autres réglages le permettent (ex. filtre fermé + résonance basse ; un seul oscillateur actif) |
| **Non** | Influence le timbre, la hauteur, le mouvement ou le jeu, sans couper le niveau audible de façon typique |

**Architecture utile (Oberheim) :** VCA 1 et VCA 2 sont en cascade. VCA 1 = volume de base + vélocité ; VCA 2 = ouverture par ENV 2. Les deux doivent laisser passer le signal pour qu’on entende quelque chose.

**Convention colonne 2 :** libellé affiché dans Matrix-Control, précédé du module, puis l’identifiant APVTS entre backticks.

---

## 1. Patch Edit

### 1.1 DCO 1

| Nom Oberheim officiel | Nom Matrix-Control | Influence sur le son | Extinction possible |
|-----------------------|--------------------|----------------------|---------------------|
| DCO 1 Initial Frequency | DCO 1 · FREQUENCY (`dco1Frequency`) | Hauteur de base de l’oscillateur 1 (par demi-tons). | Non |
| DCO 1 Frequency by LFO 1 Amount | DCO 1 · FREQ \< LFO 1 (`dco1FrequencyModByLfo1`) | Profondeur du vibrato / balayage de hauteur via LFO 1. | Non |
| DCO 1 Initial Pulse Width | DCO 1 · PULSE WIDTH (`dco1PulseWidth`) | Largeur de l’impulsion (timbre plus creux ou plus rond sur l’onde pulse). | Oui (conditionnel) — aux extrêmes, l’impulsion peut disparaître si WAVE SELECT = PULSE (ou BOTH sans composante wave audible) |
| DCO 1 PW by LFO 2 Amount | DCO 1 · PW \< LFO 2 (`dco1PulseWidthModByLfo2`) | Modulation de largeur d’impulsion (PWM) par LFO 2. | Oui (conditionnel) — même logique que PULSE WIDTH si la PWM pousse l’onde hors zone audible |
| DCO 1 Initial Waveshape | DCO 1 · WAVE SHAPE (`dco1WaveShape`) | Morph entre dent de scie et triangle sur la composante « wave ». | Non |
| DCO Sync Mode | DCO 1 · SYNC (`dco1Sync`) | Synchronisation de DCO 1 sur DCO 2 (soft / medium / hard) : harmoniques métalliques, locked pitch. | Non |
| DCO 1 Waveform Enable | DCO 1 · WAVE SELECT (`dco1WaveSelect`) | Active pulse, wave, les deux, ou coupe DCO 1 (OFF). | Oui — OFF coupe DCO 1 ; OFF sur les deux DCO (ou mix vers un DCO déjà OFF) → silence |
| DCO 1 Fixed Modulations (Lever 1 / Vibrato) | DCO 1 · LEVERS (`dco1Levers`) | Autorise bend (lever 1) et/ou vibrato global sur la hauteur de DCO 1. | Non |
| DCO 1 Fixed Modulations (Portamento) | DCO 1 · KEYBD/PORTA (`dco1KeyboardPortamento`) | Suit le clavier ou le signal de portamento pour la hauteur. | Non |
| DCO 1 Click | DCO 1 · KEY CLICK (`dco1KeyClick`) | Ajoute un clic d’attaque « percussif » à la note. | Non |

### 1.2 DCO 2

| Nom Oberheim officiel | Nom Matrix-Control | Influence sur le son | Extinction possible |
|-----------------------|--------------------|----------------------|---------------------|
| DCO 2 Initial Frequency | DCO 2 · FREQUENCY (`dco2Frequency`) | Hauteur de base de l’oscillateur 2. | Non |
| DCO 2 Frequency by LFO 1 Amount | DCO 2 · FREQ \< LFO 1 (`dco2FrequencyModByLfo1`) | Modulation de hauteur de DCO 2 par LFO 1. | Non |
| DCO 2 Detune | DCO 2 · DETUNE (`dco2Detune`) | Décalage fin vs DCO 1 : battements, épaisseur, chorus natif. | Non |
| DCO 2 Initial Pulse Width | DCO 2 · PULSE WIDTH (`dco2PulseWidth`) | Largeur d’impulsion de DCO 2. | Oui (conditionnel) — comme DCO 1 si seule l’impulsion est active |
| DCO 2 PW by LFO 2 Amount | DCO 2 · PW \< LFO 2 (`dco2PulseWidthModByLfo2`) | PWM de DCO 2 via LFO 2. | Oui (conditionnel) — idem |
| DCO 2 Initial Waveshape | DCO 2 · WAVE SHAPE (`dco2WaveShape`) | Morph scie ↔ triangle de DCO 2. | Non |
| DCO 2 Waveform Enable | DCO 2 · WAVE SELECT (`dco2WaveSelect`) | Pulse / wave / both / noise / OFF pour DCO 2. | Oui — OFF coupe DCO 2 ; NOISE seul reste audible (souffle) ; OFF + DCO 1 OFF → silence |
| DCO 2 Fixed Modulations (Lever 1 / Vibrato) | DCO 2 · LEVERS (`dco2Levers`) | Bend / vibrato sur DCO 2. | Non |
| DCO 2 Fixed Modulations (Portamento / Keyboard) | DCO 2 · KEYBD/PORTA (`dco2KeyboardPortamento`) | Clavier, portamento, ou aucun tracking de hauteur. | Non |
| DCO 2 Click | DCO 2 · KEY CLICK (`dco2KeyClick`) | Clic d’attaque sur DCO 2. | Non |

### 1.3 VCF / VCA

| Nom Oberheim officiel | Nom Matrix-Control | Influence sur le son | Extinction possible |
|-----------------------|--------------------|----------------------|---------------------|
| Mix | VCF/VCA · DCO 2 \| DCO 1 MIX (`vcfVcaBalance`) | Balance entre DCO 2 (0) et DCO 1 (63). Sculpte la présence relative des deux sources. | Oui (conditionnel) — extrême vers un DCO dont WAVE SELECT = OFF → silence |
| VCF Initial Frequency | VCF/VCA · FREQUENCY (`vcfVcaFrequency`) | Fréquence de coupure du filtre passe-bas : sombre ↔ ouvert / brillant. | Oui (conditionnel) — proche de 0 avec résonance basse → son étouffé / inaudible |
| VCF Frequency by Env 1 Amount | VCF/VCA · FREQ \< ENV 1 (`vcfVcaFrequencyModByEnv1`) | Enveloppe de filtre (contour de brillance dans le temps). | Oui (conditionnel) — montant négatif fort + cutoff déjà bas peut fermer le filtre |
| VCF Frequency by Pressure Amount | VCF/VCA · FREQ \< PRESSURE (`vcfVcaFrequencyModByPressure`) | Aftertouch qui ouvre ou ferme le filtre. | Oui (conditionnel) — si pressure nulle et montant dominant un cutoff bas |
| VCF Initial Resonance | VCF/VCA · RESONANCE (`vcfVcaResonance`) | Pic à la coupure : nasillard, sifflant, auto-oscillation aux valeurs hautes. | Non — haute résonance peut même « sauver » un cutoff bas en sifflant ; basse résonance aggrave l’extinction avec cutoff bas |
| VCA 1 (exponential) Initial Amount | VCF/VCA · VCA 1 VOLUME (`vcfVcaVca1Volume`) | Niveau de base du premier amplificateur (étage de gain principal). | Oui — proche de 0 → silence (cascade VCA) |
| VCA 1 by Velocity Amount | VCF/VCA · VCA 1 \< VELOCITY (`vcfVcaVca1ModByVelocity`) | Sensibilité du volume à la force de frappe ; travaille en tandem avec VCA 1 VOLUME. | Oui — proche de 0 (ou combo volume+vélocité trop faible) → notes inaudibles |
| VCA 2 by Env 2 Amount | VCF/VCA · VCA 2 \< ENV 2 (`vcfVcaVca2ModByEnv2`) | Ouverture du second VCA par ENV 2 (contour d’amplitude principal du patch). | Oui — proche de 0 → silence (VCA 2 ne laisse plus passer) |
| VCF Fixed Modulations (Lever 1 / Vibrato) | VCF/VCA · LEVERS (`vcfVcaLevers`) | Bend / vibrato appliqués à la coupure du filtre. | Non |
| VCF Keyboard Modulation | VCF/VCA · KEYBD/PORTA (`vcfVcaKeyboardPortamento`) | Tracking clavier / portamento sur le filtre (brillance selon la note). | Non |

### 1.4 FM / TRACK

| Nom Oberheim officiel | Nom Matrix-Control | Influence sur le son | Extinction possible |
|-----------------------|--------------------|----------------------|---------------------|
| VCF FM Initial Amount | FM/TRACK · VCF FM AMOUNT (`fmTrackFmAmount`) | Quantité de FM du filtre par DCO 1 : métal, cloche, distorsion harmonique du timbre filtré. | Non |
| VCF FM Amount by Env 3 Amount | FM/TRACK · FM \< ENV 3 (`fmTrackFmModByEnv3`) | Enveloppe de l’intensité FM (cascade avec VCF FM AMOUNT). | Non |
| VCF FM Amount by Pressure Amount | FM/TRACK · FM \< PRESSURE (`fmTrackFmModByPressure`) | Aftertouch sur la profondeur FM. | Non |
| Tracking Generator Input Source Code | FM/TRACK · TRACK INPUT (`fmTrackInput`) | Quelle source alimente le Tracking Generator (souvent le clavier). | Non |
| Tracking Point 1 | FM/TRACK · TRACK POINT 1 (`fmTrackPoint1`) | Premier point de la courbe de tracking (souvent notes graves). | Non |
| Tracking Point 2 | FM/TRACK · TRACK POINT 2 (`fmTrackPoint2`) | Deuxième point de la courbe. | Non |
| Tracking Point 3 | FM/TRACK · TRACK POINT 3 (`fmTrackPoint3`) | Point médian typique de la courbe. | Non |
| Tracking Point 4 | FM/TRACK · TRACK POINT 4 (`fmTrackPoint4`) | Quatrième point (aigus intermédiaires). | Non |
| Tracking Point 5 | FM/TRACK · TRACK POINT 5 (`fmTrackPoint5`) | Dernier point (aigus). | Non |

### 1.5 RAMP / PORTAMENTO

| Nom Oberheim officiel | Nom Matrix-Control | Influence sur le son | Extinction possible |
|-----------------------|--------------------|----------------------|---------------------|
| Ramp 1 Rate | RAMP/PORTAMENTO · RAMP 1 RATE (`rampPortamentoRamp1Rate`) | Vitesse de la rampe 1 (montée progressive souvent utilisée pour doser un LFO). | Non |
| Ramp 1 Mode | RAMP/PORTAMENTO · RAMP 1 TRIGGER (`rampPortamentoRamp1Trigger`) | Comment la rampe 1 se redéclenche (single / multi / externe / gated). | Non |
| Ramp 2 Rate | RAMP/PORTAMENTO · RAMP 2 RATE (`rampPortamentoRamp2Rate`) | Vitesse de la rampe 2. | Non |
| Ramp 2 Mode | RAMP/PORTAMENTO · RAMP 2 TRIGGER (`rampPortamentoRamp2Trigger`) | Mode de déclenchement de la rampe 2. | Non |
| Portamento Initial Rate | RAMP/PORTAMENTO · PORTAMENTO RATE (`rampPortamentoPortamentoRate`) | Vitesse du glissando entre notes. | Non |
| Portamento Rate by Velocity Amount | RAMP/PORTAMENTO · PORTA \< VELOCITY (`rampPortamentoPortamentoModByVelocity`) | La force de frappe accélère ou ralentit le portamento. | Non |
| Lag Mode | RAMP/PORTAMENTO · PORTA MODE (`rampPortamentoPortamentoMode`) | Courbe du glissando (linéaire, temps constant, exponentiel). | Non |
| Legato Portamento Enable | RAMP/PORTAMENTO · LEGATO PORTA (`rampPortamentoPortamentoLegato`) | Portamento seulement en legato (souvent lié à l’unisson). | Non |
| Keyboard Mode | RAMP/PORTAMENTO · KEYBOARD MODE (`rampPortamentoPortamentoKeyboardMode`) | Assignation des voix : rotate, reassign, unison, reassign-rob. | Non |

### 1.6 ENV 1

*(Routage fixe typique : modulation du filtre via FREQ \< ENV 1.)*

| Nom Oberheim officiel | Nom Matrix-Control | Influence sur le son | Extinction possible |
|-----------------------|--------------------|----------------------|---------------------|
| Env 1 Initial Delay Time | ENV 1 · DELAY (`env1Delay`) | Attend avant de démarrer l’enveloppe de filtre. | Non |
| Env 1 Initial Attack Time | ENV 1 · ATTACK (`env1Attack`) | Temps de montée de la brillance / du contour filtre. | Non |
| Env 1 Initial Decay Time | ENV 1 · DECAY (`env1Decay`) | Descente vers le sustain sur le filtre. | Non |
| Env 1 Sustain Level | ENV 1 · SUSTAIN (`env1Sustain`) | Niveau tenu du contour filtre tant que la touche est enfoncée. | Non |
| Env 1 Initial Release Time | ENV 1 · RELEASE (`env1Release`) | Relâchement du contour filtre après Note Off. | Non |
| Env 1 Initial Amplitude | ENV 1 · AMPLITUDE (`env1Amplitude`) | Profondeur globale d’ENV 1 (combo avec AMP \< VELOCITY). | Oui (conditionnel) — si le patch compte sur ENV 1 pour ouvrir un filtre sinon fermé |
| Env 1 Amplitude by Velocity Amount | ENV 1 · AMP \< VELOCITY (`env1AmplitudeModByVelocity`) | Vélocité qui dose la profondeur d’ENV 1. | Oui (conditionnel) — idem si cutoff de base très bas |
| Env 1 Trigger Mode | ENV 1 · TRIGGER MODE (`env1TriggerMode`) | Single / multi / reset / externe, etc. | Non |
| Env 1 Mode | ENV 1 · ENVELOPE MODE (`env1EnvelopeMode`) | Normal, DADR, freerun, ou combinaison. | Non |
| Env 1 LFO Trigger Mode | ENV 1 · LFO 1 TRIGGER (`env1Lfo1Trigger`) | Déclenchement d’ENV 1 par LFO 1 / gated LFO. | Non |

### 1.7 ENV 2

*(Routage fixe typique : ouverture de VCA 2 — contour d’amplitude principal.)*

| Nom Oberheim officiel | Nom Matrix-Control | Influence sur le son | Extinction possible |
|-----------------------|--------------------|----------------------|---------------------|
| Env 2 Initial Delay Time | ENV 2 · DELAY (`env2Delay`) | Retarde l’attaque du volume. | Oui (conditionnel) — delay max = silence prolongé au début (patch « mort » à l’oreille courte) |
| Env 2 Initial Attack Time | ENV 2 · ATTACK (`env2Attack`) | Montée du volume. | Oui (conditionnel) — attack extrême = quasi-silence sur notes courtes |
| Env 2 Initial Decay Time | ENV 2 · DECAY (`env2Decay`) | Descente vers le sustain de volume. | Non |
| Env 2 Sustain Level | ENV 2 · SUSTAIN (`env2Sustain`) | Niveau tenu du volume. | Oui (conditionnel) — sustain 0 + release court → notes très brèves / sensation d’extinction |
| Env 2 Initial Release Time | ENV 2 · RELEASE (`env2Release`) | Queue de volume après Note Off. | Non |
| Env 2 Initial Amplitude | ENV 2 · AMPLITUDE (`env2Amplitude`) | Profondeur d’ENV 2 vers le VCA (avec AMP \< VELOCITY). | Oui — proche de 0 → VCA 2 ne s’ouvre pas → silence |
| Env 2 Amplitude by Velocity Amount | ENV 2 · AMP \< VELOCITY (`env2AmplitudeModByVelocity`) | Vélocité sur la profondeur d’ENV 2. | Oui — proche de 0 avec amplitude basse → silence |
| Env 2 Trigger Mode | ENV 2 · TRIGGER MODE (`env2TriggerMode`) | Modes de déclenchement d’ENV 2. | Non |
| Env 2 Mode | ENV 2 · ENVELOPE MODE (`env2EnvelopeMode`) | Normal / DADR / freerun / both. | Non |
| Env 2 LFO Trigger Mode | ENV 2 · LFO 1 TRIGGER (`env2Lfo1Trigger`) | Sync éventuelle sur LFO 1. | Non |

### 1.8 ENV 3

*(Routage fixe typique : profondeur FM du filtre via FM \< ENV 3.)*

| Nom Oberheim officiel | Nom Matrix-Control | Influence sur le son | Extinction possible |
|-----------------------|--------------------|----------------------|---------------------|
| Env 3 Initial Delay Time | ENV 3 · DELAY (`env3Delay`) | Retarde le contour FM. | Non |
| Env 3 Initial Attack Time | ENV 3 · ATTACK (`env3Attack`) | Montée de la FM. | Non |
| Env 3 Initial Decay Time | ENV 3 · DECAY (`env3Decay`) | Descente FM. | Non |
| Env 3 Sustain Level | ENV 3 · SUSTAIN (`env3Sustain`) | Niveau tenu de la FM. | Non |
| Env 3 Initial Release Time | ENV 3 · RELEASE (`env3Release`) | Relâchement FM. | Non |
| Env 3 Initial Amplitude | ENV 3 · AMPLITUDE (`env3Amplitude`) | Profondeur globale d’ENV 3. | Non — (sauf routage matrice inhabituel vers un VCA) |
| Env 3 Amplitude by Velocity Amount | ENV 3 · AMP \< VELOCITY (`env3AmplitudeModByVelocity`) | Vélocité sur ENV 3. | Non |
| Env 3 Trigger Mode | ENV 3 · TRIGGER MODE (`env3TriggerMode`) | Modes de déclenchement. | Non |
| Env 3 Mode | ENV 3 · ENVELOPE MODE (`env3EnvelopeMode`) | Normal / DADR / freerun / both. | Non |
| Env 3 LFO Trigger Mode | ENV 3 · LFO 1 TRIGGER (`env3Lfo1Trigger`) | Déclenchement via LFO 1. | Non |

### 1.9 LFO 1

| Nom Oberheim officiel | Nom Matrix-Control | Influence sur le son | Extinction possible |
|-----------------------|--------------------|----------------------|---------------------|
| LFO 1 Initial Speed | LFO 1 · SPEED (`lfo1Speed`) | Vitesse du LFO 1 (vibrato, PWM, etc. selon routages). | Non |
| LFO 1 Speed by Pressure Amount | LFO 1 · SPEED \< PRESSURE (`lfo1SpeedModByPressure`) | Aftertouch qui accélère/ralentit LFO 1. | Non |
| LFO 1 Waveshape | LFO 1 · WAVEFORM (`lfo1Waveform`) | Forme : triangle, scies, carré, random, noise, sampled. | Non |
| LFO 1 Retrigger Point | LFO 1 · RETRIGGER POINT (`lfo1RetriggerPoint`) | Point de reprise de la forme d’onde au retrigger. | Non |
| LFO 1 Sampled Source Number | LFO 1 · SAMPLE INPUT (`lfo1SampleInput`) | Source échantillonnée si WAVEFORM = Sampled. | Non |
| LFO 1 Initial Amplitude | LFO 1 · AMPLITUDE (`lfo1Amplitude`) | Profondeur de base de LFO 1. | Non |
| LFO 1 Amplitude by Ramp 1 Amount | LFO 1 · AMP \< RAMP 1 (`lfo1AmplitudeModByRamp1`) | La rampe 1 dose progressivement la profondeur du LFO 1. | Non |
| LFO 1 Trigger | LFO 1 · TRIGGER MODE (`lfo1TriggerMode`) | Off / single / multi / externe. | Non |
| LFO 1 Lag Enable | LFO 1 · LAG (`lfo1Lag`) | Lissage du signal LFO 1. | Non |

### 1.10 LFO 2

| Nom Oberheim officiel | Nom Matrix-Control | Influence sur le son | Extinction possible |
|-----------------------|--------------------|----------------------|---------------------|
| LFO 2 Initial Speed | LFO 2 · SPEED (`lfo2Speed`) | Vitesse du LFO 2 (souvent PWM). | Non |
| LFO 2 Speed by Keyboard Amount | LFO 2 · SPEED \< KEYBD (`lfo2SpeedModByKeyboard`) | La note jouée influence la vitesse de LFO 2. | Non |
| LFO 2 Waveshape | LFO 2 · WAVEFORM (`lfo2Waveform`) | Forme d’onde de LFO 2. | Non |
| LFO 2 Retrigger Point | LFO 2 · RETRIGGER POINT (`lfo2RetriggerPoint`) | Point de reprise au retrigger. | Non |
| LFO 2 Sampled Source Number | LFO 2 · SAMPLE INPUT (`lfo2SampleInput`) | Source pour le mode Sampled. | Non |
| LFO 2 Initial Amplitude | LFO 2 · AMPLITUDE (`lfo2Amplitude`) | Profondeur de base de LFO 2. | Non |
| LFO 2 Amplitude by Ramp 2 Amount | LFO 2 · AMP \< RAMP 2 (`lfo2AmplitudeModByRamp2`) | Rampe 2 sur la profondeur de LFO 2. | Non |
| LFO 2 Trigger | LFO 2 · TRIGGER MODE (`lfo2TriggerMode`) | Off / single / multi / externe. | Non |
| LFO 2 Lag Enable | LFO 2 · LAG (`lfo2Lag`) | Lissage de LFO 2. | Non |

---

## 2. Matrix Modulation

Dix bus identiques (`0`–`9`). Chaque bus a trois paramètres : **SOURCE**, **AMOUNT**, **DESTINATION**.  
Noms Oberheim (format patch) : *Matrix Modulation Bus N Source Code / Amount / Destination Code*.  
Libellés Matrix-Control : `SOURCE`, `AMOUNT`, `DESTINATION` (répétés sur chaque bus).  
Identifiants APVTS : `modulationBus{N}Source`, `modulationBus{N}Amount`, `modulationBus{N}Destination` (N = 0…9).

Les lignes ci-dessous décrivent le **rôle** de chaque champ (valable pour les 10 bus). La colonne extinction précise quand le bus peut tuer le son selon la **destination** choisie.

| Nom Oberheim officiel | Nom Matrix-Control | Influence sur le son | Extinction possible |
|-----------------------|--------------------|----------------------|---------------------|
| Matrix Modulation Bus N Source Code | MATRIX MODULATION · SOURCE (`modulationBus{N}Source`) | Choisit le modulateur (ENV, LFO, clavier, vélocité, pressure, pédales, leviers, rampes, tracking, etc., ou NONE). Sans source active, le bus ne fait rien. | Non en soi — NONE désactive le bus ; une source « morte » (ex. pressure jamais jouée) peut laisser une destination figée à une valeur silencieuse **si** un amount extrême a déjà été appliqué autrement |
| Matrix Modulation Bus N Amount | MATRIX MODULATION · AMOUNT (`modulationBus{N}Amount`) | Intensité et polarité de la modulation (−63…+63). C’est le « dosage » du geste expressif ou du mouvement. | Oui (conditionnel) — si DESTINATION est un niveau critique (VCA, mix, cutoff) et que le montant pousse la valeur effective vers le silence |
| Matrix Modulation Bus N Destination Code | MATRIX MODULATION · DESTINATION (`modulationBus{N}Destination`) | Paramètre modulé en temps réel (hauteur, PW, filtre, volumes, temps d’enveloppe, LFO, portamento, ou NONE). | Oui (conditionnel) — voir tableau des destinations à risque ci-dessous |

### Destinations à risque d’extinction (Table 3 Oberheim)

Quand un bus module l’une de ces destinations avec un **AMOUNT** défavorable (souvent négatif fort, ou positif qui annule un niveau déjà bas), le patch peut devenir inaudible.

| Destination Oberheim (Table 3) | Libellé Matrix-Control | Risque d’extinction |
|--------------------------------|------------------------|---------------------|
| Mix Level | DCO 2 \| DCO 1 MIX | Oui (conditionnel) — bascule vers un oscillateur OFF |
| VCF Frequency | VCF FREQUENCY | Oui (conditionnel) — ferme le filtre (surtout si résonance basse) |
| VCA 1 Level | VCA 1 VOLUME | Oui — baisse le gain de base |
| VCA 2 Level | VCA 2 VOLUME | Oui — ferme l’étage d’amplitude piloté par ENV 2 |
| Envelope 2 Amplitude | ENV 2 AMPLITUDE | Oui — réduit l’ouverture de VCA 2 |
| DCO 1 Pulse Width / DCO 2 Pulse Width | DCO 1/2 PULSE WIDTH | Oui (conditionnel) — si seule l’impulsion est active |
| DCO 1 Waveshape / DCO 2 Waveshape | DCO 1/2 WAVE SHAPE | Non (timbre) |
| DCO 1/2 Frequency | DCO 1/2 FREQUENCY | Non (hauteur) |
| VCF FM Amount | VCF FM BY DCO 1 | Non |
| VCF Resonance | VCF RESONANCE | Non (peut même aider à entendre un cutoff bas) |
| Envelope 1/3 Amplitude, Delay, Attack, Decay, Release | ENV 1/3 … | Oui (conditionnel) pour ENV 1 Amplitude seulement si le filtre dépend d’ENV 1 ; sinon Non |
| Envelope 2 Delay / Attack / Decay / Release | ENV 2 DELAY/ATTACK/… | Oui (conditionnel) — attack/delay extrêmes via mod = notes inaudibles en jeu normal |
| LFO 1/2 Speed / Amplitude | LFO 1/2 SPEED/AMPLITUDE | Non |
| Portamento Time | PORTAMENTO RATE | Non |
| Unused Modulation (NONE) | NONE | Non |

### Sources (Table 2 Oberheim) — rappel des libellés Matrix-Control

| Oberheim | Matrix-Control |
|----------|----------------|
| Unused Modulation | NONE |
| Env 1 / Envelope 1 | ENV 1 |
| Env 2 / Envelope 2 | ENV 2 |
| Env 3 / Envelope 3 | ENV 3 |
| LFO 1 | LFO 1 |
| LFO 2 | LFO 2 |
| Vibrato | VIBRATO |
| Ramp 1 | RAMP 1 |
| Ramp 2 | RAMP 2 |
| Keyboard | KEYBD |
| Portamento | PORTA |
| Tracking Generator | TRACK |
| Keyboard Gate | KB GATE |
| Velocity | VELOCITY |
| Release Velocity | REL VEL |
| Pressure | PRESSURE |
| Pedal 1 / 2 | PEDAL 1 / PEDAL 2 |
| Lever 1 / 2 / 3 | LEVER 1 / LEVER 2 / LEVER 3 |

---

## 3. Suspects prioritaires pour une calibration « playable »

À traiter en premier dans les garde-fous de mutation (ordre suggéré) :

1. **VCA 1 VOLUME** et **VCA 1 \< VELOCITY**
2. **VCA 2 \< ENV 2**, **ENV 2 · AMPLITUDE**, **ENV 2 · AMP \< VELOCITY**
3. **VCF FREQUENCY** (+ interaction **RESONANCE**)
4. **DCO 2 \| DCO 1 MIX** + **WAVE SELECT** (DCO 1 / DCO 2)
5. **PULSE WIDTH** (si WAVE SELECT = PULSE)
6. Bus Matrix Modulation vers **VCA 1 VOLUME**, **VCA 2 VOLUME**, **VCF FREQUENCY**, **ENV 2 AMPLITUDE**, **DCO 2 \| DCO 1 MIX**

---

## 4. Références

- `_local/References/Oberheim/oberheim-matrix-1000-midi-sysex-implementation.md` — noms officiels du format patch / Table 2 & 3
- `_local/References/Oberheim/oberheim-matrix-6-6r-midi-sysex-implementation.md` — même cartographie paramètres + préfixe page Patch Edit
- `_local/References/Oberheim/oberheim-matrix-1000-user-manual-personal-notes.md` — cascade VCA 1 / VCA 2
- `Source/Shared/Definitions/PluginDisplayNames.h` — libellés UI
- `Source/Shared/Definitions/PluginIDs.h` — identifiants APVTS
)
