# Interactions contre-intuitives (Matrix-1000 / 6 / 6R)

Catalogue pour la mise au point du Patch Mutator : paramètres qui **ne se comportent pas** comme une addition indépendante « à l’oreille ».

**Sources :**

- *Matrix-6 Owner’s Manual* (Oberheim) — section VCA / modulations inter-reliées (pages ~30–31 / Edit Pages)
- Opcode *Galaxy Plus Editors — Matrix-6/1000* (résumé des paramètres ; précise que le manuel Matrix-1000 **ne documente pas** l’édition de patch)
- Tests hardware Guillaume (août 2026) sur `VCA 1 VOLUME` / `VCA 1 < VELOCITY` et `ENV 2 · AMP < VELOCITY` (validés post-fix SysEx)
- Notes projet : cascade VCA, 18 routages hardwired

---

## 1. Oui : ton observation VCA 1 est exacte

### Ce que tu as mesuré

- `VCA 1 VOLUME` = **63** → **vélocité positive** (`0`…`+63`) : peu ou pas d’effet audible (plafond déjà atteint).
- `VCA 1 VOLUME` = **63** + `VCA 1 < VELOCITY` = **−63** → **baisse de volume audible** sur rampe Live 0→127 (confirmé août 2026, post-fix SysEx).
- Pour entendre un effet de `VCA 1 < VELOCITY` **positif** = +63, il faut baisser `VCA 1 VOLUME` (0 ou faible).

**Asymétrie importante Mutator :** au plafond Volume, la mod négative **reste active** (ducking vélocité) ; seule la mod **positive** est « gaspillée ». Ne pas classer `VCA 1 < VELOCITY` négatif comme paramètre inutile quand Volume = 63.

### Pourquoi ça fait sens

Modèle mental (plafond / headroom) :

```text
niveau_VCA1 ≈ clamp( VOLUME + (VELOCITY_mod × force_frappe) )
                 └──────── plafond matériel / plage 0…63 ────────┘
```

- Si **VOLUME est déjà au max**, une Velocity **positive** n’a plus de marge pour monter → effet inaudible.
- Si **VOLUME est bas**, Velocity positive devient le vrai « ouvreur » du VCA 1 → le toucher devient audible (et risqué pour le Mutator).

### Ce que dit le manuel Matrix-6 (presque mot pour mot)

Paramètres **27 VCA 1 VOLUME** et **28 VCA 1 MOD BY VELOCITY** travaillent **hand-in-hand** :

- Si VOLUME = **63**, Velocity positive aura **peu ou pas d’effet**, parce que VCA 1 est déjà aussi fort qu’il peut l’être — Velocity **ne peut pas le rendre plus fort**.
- Si VOLUME est **bas**, une Velocity positive forte exige de frapper très fort, sinon certaines notes **semblent « drop out »** (inaudibles).
- Extremes VOLUME=0 + Velocity=+63 + sensibilité Master max → plage dynamique **presque inutilisable**.

### Manuels : où chercher

| Document | Contenu utile |
|----------|----------------|
| **Matrix-6 Owner’s Manual** | Oui — explication explicite du couple 27/28 + mises en garde |
| **Matrix-6R Owner’s Manual** | Même architecture Patch Edit (à croiser ; PDF scanné sans OCR ici) |
| **Matrix-1000 Owner’s Manual** | **Non** pour la prog de patch — surtout MIDI/SysEx, Ext. Funct., etc. (Opcode le dit clairement) |
| Opcode Galaxy Matrix-6/1000 | Décrit Volume / Velocity / Env2, sans le détail « plafond à 63 » du manuel 6 |

---

## 2. Autres comportements contre-intuitifs utiles au Mutator

### 2.1 VCA 2 n’a pas de « Volume »

- `VCA 2 < ENV 2` : le niveau de base de VCA 2 est **pré-réglé à 0** ; seul ENV 2 (via ce montant) l’ouvre.
- Montant **0** → pas de son (manuel Matrix-6).
- Montant **négatif** → **inverse** l’effet d’ENV 2 (contour « à l’envers »), pas un simple « moins fort » linéaire.

**Mutator :** ne pas traiter `VCA 2 < ENV 2` comme un gain indépendant d’ENV 2 ; c’est un **dosage du pré-câblage** ENV2→VCA2.

### 2.2 ENV 2 · AMP \< VELOCITY — volume déguisé (confirmé)

Le manuel souligne que les modulations sont **inter-related** :

- Toucher `ENV 2 · AMP < VELOCITY` modifie la profondeur d’ENV 2,
- or ENV 2 est **pré-câblée** vers VCA 2,
- donc on croit éditer « l’enveloppe » alors qu’on change le **volume final**.

**Confirmé hardware (août 2026, post-fix SysEx) :** rampe vélocité 0→127 sur `ENV 2 · AMP < VELOCITY` produit un effet **audible sur le volume** — cohérent avec le routage ENV 2 → VCA 2. Ce paramètre contrôle la sensibilité vélocité de l’enveloppe d’**amplitude**, pas un autre chemin parallèle.

**Mutator :** même famille de risque que `ENV 2 · AMPLITUDE` / `VCA 2 < ENV 2`, pas un paramètre « décoratif ». Toute mutation négative forte peut **tuer** le volume sur notes douces (soft-kill vélocité).

### 2.3 Famille « Initial Amount + Mod Amount » (plafond / marge)

Même logique de **headroom** que VCA 1, à valider au cas par cas sur machine, mais attendue sur :

| Initial (base) | Mod (ajoute / retire) | Effet contre-intuitif typique |
|----------------|----------------------|-------------------------------|
| `VCA 1 VOLUME` | `VCA 1 < VELOCITY` | **Confirmé** — plafond à 63 |
| `ENV n · AMPLITUDE` | `ENV n · AMP < VELOCITY` | Si Amp déjà haut, Velocity positive peu audible ; Amp bas → Velocity devient critique (silence / drop out) |
| `VCF FM AMOUNT` | `FM < ENV 3` / `FM < PRESSURE` | FM nulle → les mods FM n’ajoutent rien d’audible |
| `LFO n · AMPLITUDE` | `AMP < RAMP n` | Ramp « fade in » du LFO peu audible si Amp LFO déjà max ; Ramp négative = fade out depuis le niveau programmé |
| `FREQUENCY` (VCF) | `FREQ < ENV 1` / `FREQ < PRESSURE` | Cutoff déjà très haut → ouverture ENV1 limitée ; cutoff bas + ENV1 négatif → étouffement |

**Règle Mutator :** pour qu’une mod « \< SOURCE » soit **audible**, la base ne doit pas saturer le plafond dans le même sens que la mod.

### 2.4 Vélocité Master (Matrix-6/6R) vs patch

- `M40 VELOCITY SCALE` / `M41 VELOCITY SENS` (Master) **calibrent** toute la réponse vélocité du clavier.
- Un patch « VOLUME=0 + VEL=+63 » peut être jouable avec une sens Master douce, et **mort** avec une sens max.

**Mutator :** les floors playable sur Velocity patch doivent rester **relativisés** (le Master n’est pas dans le patch `.syx` single de la même façon partout) — ne pas sur-optimiser Velocity sans contexte.

### 2.5 Polarités négatives ≠ « un peu moins »

Plusieurs montants signés **inversent** le modulateur :

- `VCA 2 < ENV 2` négatif → enveloppe de volume inversée
- `FREQ < ENV 1` négatif → l’enveloppe **ferme** le filtre
- Mods Velocity négatives → plus tu frappes fort, plus ça baisse

**Mutator :** un amount négatif fort sur un chemin d’amplitude/filtre n’est pas un « tweak soft » ; c’est souvent un **tueur** ou un patch « bizarre ».

### 2.6 LFO Amp \< Ramp (hardwired)

- Ramp 1 → LFO 1 Amp, Ramp 2 → LFO 2 Amp (pré-câblé).
- Usage classique : vibrato qui **arrive** progressivement (Ramp positive) ou qui **part** (Ramp négative).
- Si `LFO · AMPLITUDE` = 0 et Ramp positive : le LFO part de 0 et monte — OK.
- Si Amp LFO déjà haut et Ramp positive : peu de marge (même idée de plafond).

### 2.7 FM : trio DCO1 Freq × VCF Freq × VCF FM Amount

Le manuel insiste : le caractère FM dépend de **trois** réglages à la fois. Muter un seul peut rendre la FM inaudible ou agressive sans « tuer » le volume — échec **timbral**, pas silence.

### 2.8 Opcode / Galaxy — phrase générique

> *Often, the effect of one parameter depends on the setting of other parameters. If you alter a parameter and don’t hear any change… the edited parameter is ineffectual due to the setting of some other parameter.*

C’est exactement la classe « plafond / headroom / pré-câblage à 0 ».

---

## 3. Implications Patch Mutator (pratiques)

| Insight | Conséquence algo |
|---------|------------------|
| Velocity VCA1 **positive inefficace** si VOLUME=63 | Muter `VCA 1 < VELOCITY` vers le **positif** seul n’enrichit pas si Volume reste max — bruit de mutation possible |
| Velocity VCA1 **négative active** si VOLUME=63 | Ducking vélocité **audible** même au plafond — ne pas ignorer les valeurs négatives dans les garde-fous |
| Velocity VCA1 **dangereuse** si VOLUME bas | Floor / couple : ne pas laisser Volume→0 **et** Vel→0 ; si Volume bas, garder Vel positive utilisable **ou** remonter Volume |
| VCA2 base = 0 | `VCA 2 < ENV 2` ≈0 = silence (sauf MM→VCA2) — tueur dur |
| ENV2 Amp\<Vel = volume déguisé | Traiter comme risque amplitude, pas comme décor enveloppe |
| Mods positives au plafond | Peu d’effet perçu → pauvre « playable interest » |
| Mods négatives sur chemins critiques | Risque silence / inversion — borner ou post-check |

### Correctif vs ancienne formulation (rapport extinctions)

**À éviter :** « volume OK + vel=0 + jeu doux → silence » quand Volume=63 (faux : le son reste fort).

**À retenir :**

1. Volume haut (≈63) → Velocity VCA1 **positive** presque sans effet (pas un tueur par elle-même).
2. Volume haut (≈63) → Velocity VCA1 **négative** peut **abaisser** le niveau (ducking) — à traiter comme mod expressive, pas comme no-op.
3. Volume bas + Velocity basse/nulle → **silence / drop out**.
4. Volume bas + Velocity haute → dynamique large, notes douces fragiles.

---

## 4. Bug MIDI corrigé (août 2026) — valeurs signées Remote Edit

**Symptôme initial :** `VCA 1 < VELOCITY` négatif et `ENV 2 · AMP < VELOCITY` semblaient sans effet ; MIDI Monitor montrait `F0 10 06 06 42` puis `Invalid 00 F7`, Tune Request, Clock, Reset…

**Cause :** le modèle patch stocke les signed en **8 bits** (`−63` → `0xC1`, `−9` → `0xF7`). Remote Edit (`0x06`) et Matrix Mod (`0x0B`) envoyaient ce byte **brut**. Or MIDI interdit le bit 7 dans les data : `0xF7` = fin de SysEx, `0xFF` = Reset, etc.

**Correctif :** masquer `& 0x7F` à l’émission (`−63` → `0x41`). Le synthé **sign-étend le bit 6** vers le bit 7 (spec Oberheim).

**Code :** `SysExEncoder::encodeRemoteParameterEdit` + `encodeMatrixModBusEdit` ; tests unitaires associés (`SysExEncoderTests`, dispatchers Patch / Matrix Mod).

### Validation (août 2026)

| Scénario | Résultat |
|----------|----------|
| `VOLUME=63` + `VCA 1 < VELOCITY=−63`, rampe Live 0→127 | Volume baisse audible ✓ |
| `ENV 2 · AMP < VELOCITY`, balayage −63…+63 | Effet volume audible ✓ |
| Trace MIDI post-fix | `F0 10 06 06 <param> <vv> F7` avec `vv` ≤ `0x7F`, pas de messages système parasites ✓ |

**Leçon calibration :** avant cette correction, des paramètres signés négatifs pouvaient être classés à tort « sans effet » ou « contre-intuitifs » — toujours vérifier la trace MIDI si le hardware ne répond pas.

**Paramètres Patch Edit signés concernés par la même classe de bug** (tous `minValue < 0` dans les descripteurs) : `VCA 1 < VELOCITY`, `FREQ < ENV 1`, `FREQ < PRESSURE`, `ENV n · AMP < VELOCITY`, `FM < ENV 3`, `FM < PRESSURE`, `PORTA < VELOCITY`, `LFO n · AMP < RAMP n`, `LFO n · SPEED < …`, plus **AMOUNT** de chaque bus Matrix Modulation.

---

## 5. Tests automatisés SysEx (piste technique)

Pour éviter une régression silencieuse sur **tous** les contrôles GUI émettant du Remote Edit / Matrix Mod :

- Suite de tests **descriptor-driven** (parcours `ApvtsPatchMapper` / `ApvtsMasterMapper`, pas automation GUI visuelle).
- Invariant : aucun octet data SysEx avec bit 7 ; round-trip signed via fil pour chaque paramètre `minValue < 0`.
- Exécutable en CI sans synthé ni MIDI Monitor — voir prompt `/bmad-build` préparé pour implémentation.

---

## 6. Comportements à confirmer (hardware)

Protocole : base au max vs base à 0, comparer 0 / +63 / −63 sur rampe vélocité ou note fixe.

| # | Couple | Question | Statut |
|---|--------|----------|--------|
| 1 | `ENV 2 · AMPLITUDE` max vs `AMP < VELOCITY` | Plafond amplitude ENV2 vs vélocité positive | Partiel — AMP\<Vel confirmé audible ; plafond Amp à confirmer |
| 2 | `ENV 1 · AMPLITUDE` max vs `AMP < VELOCITY` | Brillance / cutoff fixe | À faire |
| 3 | `LFO 1 · AMPLITUDE` max vs `AMP < RAMP 1` | Fade-in LFO au plafond | À faire |
| 4 | `VCF FM AMOUNT` = 0 vs `FM < ENV 3` = +63 | FM inaudible attendue | À faire |
| 5 | `FREQUENCY` VCF = 127 vs `FREQ < ENV 1` = +63 | Marge d’ouverture filtre | À faire |

Quand un test confirme, mettre à jour cette table avec date + device + patch de base.
