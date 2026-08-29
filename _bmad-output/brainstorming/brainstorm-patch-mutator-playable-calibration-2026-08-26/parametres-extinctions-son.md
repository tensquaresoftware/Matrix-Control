# Paramètres et combinaisons qui éteignent le son

Rapport d’analyse pour la calibration « playable » du Patch Mutator sur Matrix-1000 / Matrix-6 / Matrix-6R.

**But :** lister ce qui peut rendre un patch inaudible (silence total ou étouffement pratiquement mort), en tenant compte des **interactions**, pas seulement des paramètres isolés.

**Sources :** architecture Oberheim (format patch SysEx, Table 2/3 Matrix Modulation), notes VCA cascade, patch de base Matrix-6, inventaire Patch Edit / Matrix Modulation, échanges de calibration playable.

---

## 1. Chemin du signal (ce qu’il faut garder ouvert)

Schéma simplifié :

```text
DCO 1 ─┐
       ├─► MIX ─► VCF (filtre) ─► VCA 1 ─► VCA 2 ─► sortie
DCO 2 ─┘              ▲              ▲         ▲
                      │              │         │
                 FREQ / RES     VOLUME +     ENV 2
                 (+ ENV 1)      VELOCITY    (pré-câblé)
```

**Règle d’or :** pour entendre une note, il faut **à la fois** :

1. Au moins une source audio vivante (DCO 1 et/ou DCO 2, éventuellement bruit sur DCO 2),
2. Un mix qui pointe vers une source vivante,
3. Un filtre qui ne soit pas complètement fermé (sauf sauvetage par résonance),
4. VCA 1 qui laisse passer,
5. VCA 2 qui laisse passer.

VCA 1 et VCA 2 sont **en cascade** : fermer l’un des deux suffit à tuer le son.

---

## 2. Classes d’extinction

| Classe | Effet à l’oreille | Exemple typique |
|--------|-------------------|-----------------|
| **A — Tueur dur (amplitude)** | Silence total dès la note | `VCA 1 VOLUME` → 0 |
| **B — Étouffement (filtre)** | Son très sombre / quasi mort | `FREQUENCY` filtre ≈ 0 + `RESONANCE` basse |
| **C — Source coupée** | Plus d’onde (ou mauvais mix) | Les deux `WAVE SELECT` = OFF |
| **D — Sensation de mort (jeu)** | Notes trop tardives / trop courtes | `ENV 2` delay/attack extrêmes *(secondaire)* |
| **E — Matrice** | Même effets A/B/C via un bus | Amount négatif vers `VCA 1 VOLUME` |

---

## 3. Tueurs durs — chemin d’amplitude (classe A)

Ce sont les cas les plus dangereux pour un mutateur : **indépendants du jeu** (sauf la vélocité, voir plus bas).

### 3.1 Paramètres isolés

| Paramètre Matrix-Control | Rôle | Condition d’extinction |
|--------------------------|------|------------------------|
| **VCA 1 VOLUME** | Gain de base du premier VCA | Valeur trop proche de **0** → silence. **Tueur dur** le plus trivial. |
| **VCA 2 \< ENV 2** | Ouverture pré-câblée de VCA 2 par ENV 2 | Baisser vers **0** éteint le son **si aucune autre source ne module VCA 2** (notamment via Matrix Modulation → `VCA 2 VOLUME`). Cas « moins trivial » mais fréquent. |
| **ENV 2 · AMPLITUDE** | Profondeur d’ENV 2 (qui nourrit VCA 2) | Trop proche de **0** → ENV 2 n’ouvre plus VCA 2 → silence. |

### 3.2 Combinaisons / tandems (à traiter comme des couples)

Oberheim : amplitude d’enveloppe = **Amplitude + Amp \< Velocity** ; VCA 1 = **Volume + VCA 1 \< Velocity**.

| Combinaison | Mécanisme | Extinction |
|-------------|-----------|------------|
| **VCA 1 VOLUME** + **VCA 1 \< VELOCITY** | Volume = niveau de base plafonné ; Velocity **ajoute** (ou retire) par-dessus, sans dépasser le plafond | **Confirmé manuel Matrix-6 + tests G. :** si VOLUME = **63**, Velocity (+/−/0) ne change presque rien (déjà au max). Pour entendre Velocity, il faut **baisser VOLUME** (souvent vers 0). VOLUME bas + Velocity positif fort → notes douces qui « drop out ». Voir `interactions-contre-intuitives.md`. |
| **ENV 2 · AMPLITUDE** + **ENV 2 · AMP \< VELOCITY** | Même famille « initial + mod » (manuel : inter-relié via VCA 2) | Amplitude basse + mod vélocité défavorable → VCA 2 s’ouvre mal ; Velocity sur ENV 2 agit **indirectement** sur le volume final |
| **VCA 2 \< ENV 2** + **ENV 2 · AMPLITUDE** (+ éventuellement **AMP \< VELOCITY**) | Chaîne série : montant VCA2←ENV2 **et** profondeur d’ENV 2 | Baisser l’un **ou** l’autre peut tuer ; baisser les deux rend la récupération très difficile |
| **VCA 2 \< ENV 2** bas **sans** bus Matrix vers `VCA 2 VOLUME` | Pas de « plan B » pour ouvrir VCA 2 | Extinction progressive puis totale en descendant `VCA 2 \< ENV 2` |
| **VCA 2 \< ENV 2** bas **avec** bus Matrix actif → `VCA 2 VOLUME` (source vivante + amount utile) | Une autre source ouvre VCA 2 | **Pas** d’extinction obligatoire — exception de design connue (ex. LFO 1 → VCA 2) |

### 3.3 Lecture « designer »

- **VCA 1 VOLUME** : interrupteur de master gain du patch.
- **VCA 2 \< ENV 2** : en patch « standard », on le laisse **haut / max** ; on ne le descend que si on pilote explicitement VCA 2 ailleurs.
- **ENV 2** : c’est l’enveloppe de **volume** du Matrix. Toucher son amplitude (et sa sensibilité vélocité) touche directement l’audibilité.

---

## 4. Étouffement par le filtre (classe B)

Le filtre ne coupe pas le gain comme un VCA, mais une coupure trop basse avec peu de résonance donne un son **inaudible en pratique**.

| Paramètre / combinaison | Condition | Remarque |
|-------------------------|-----------|----------|
| **FREQUENCY** (VCF) seul | Trop proche de **0** | Peut suffire à étouffer, surtout sur notes basses |
| **FREQUENCY** + **RESONANCE** basse / nulle | Cutoff bas **et** résonance faible | Combo classique d’extinction « douce » |
| **FREQUENCY** bas + **RESONANCE** haute | Cutoff bas mais sifflement / auto-oscillation | La résonance peut **sauver** l’audibilité (pic audible) |
| **FREQ \< ENV 1** négatif fort + cutoff déjà bas | L’enveloppe referme encore le filtre | Extinction pendant la note ou dès l’attaque |
| **FREQ \< PRESSURE** (montant qui ferme) + pas d’aftertouch / pressure nulle | Le montant « attend » une pressure qui n’arrive pas, ou un offset effectif trop bas | Cas plus rare, mais possible selon patch |
| Bus Matrix → **VCF FREQUENCY** avec amount défavorable | Même effet que baisser FREQUENCY | Voir §6 |

**RESONANCE seule** n’est pas un tueur : elle aggrave ou atténue le risque selon qu’elle est basse ou haute.

---

## 5. Sources audio coupées (classe C)

| Paramètre / combinaison | Condition d’extinction |
|-------------------------|------------------------|
| **DCO 1 · WAVE SELECT** = OFF **et** **DCO 2 · WAVE SELECT** = OFF | Plus aucune source → silence total |
| **DCO 2 \| DCO 1 MIX** extrême vers DCO 1 **et** DCO 1 = OFF | Mix « entend » un oscillateur mort |
| **DCO 2 \| DCO 1 MIX** extrême vers DCO 2 **et** DCO 2 = OFF | Idem de l’autre côté |
| Mix médian + un seul DCO OFF | En général encore audible (l’autre DCO reste) |

**Note :** `WAVE SELECT` = PULSE / WAVE / BOTH / NOISE change le **timbre** sans tuer le son. Seul **OFF** (surtout en duo, ou couplé au mix) est critique.

### Pulse Width (priorité basse)

| Combinaison | Risque | Commentaire |
|-------------|--------|-------------|
| **PULSE WIDTH** (ou PWM) aux extrêmes + **WAVE SELECT** = PULSE seulement | Impulsion peut s’effacer | Observé comme peu dominant en mutation réelle → **suspect secondaire**, pas tueur dur de premier plan |

---

## 6. Matrix Modulation (classe E)

La matrice ne crée pas une nouvelle physique : elle **attaque les mêmes points critiques** (et peut en ouvrir de nouveaux).

### 6.1 Destinations dangereuses

| Destination Matrix-Control | Effet si Amount pousse vers le silence |
|----------------------------|----------------------------------------|
| **VCA 1 VOLUME** | Tueur dur (comme baisser VCA 1 Volume) |
| **VCA 2 VOLUME** | Ferme VCA 2 (équivalent / complément de `VCA 2 \< ENV 2`) |
| **ENV 2 AMPLITUDE** | Assèche ENV 2 → VCA 2 ne s’ouvre plus |
| **VCF FREQUENCY** | Étouffement filtre |
| **DCO 2 \| DCO 1 MIX** | Peut pointer vers un DCO OFF |
| **ENV 2 DELAY / ATTACK** (etc.) | Peut rendre le patch « mort » au jeu *(secondaire)* |

### 6.2 Scénarios de mutation qui tuent

1. **Naissance d’un bus létal** : `SOURCE` passe de NONE → source active, `DESTINATION` = VCA / ENV 2 amp / filtre / mix, `AMOUNT` défavorable (souvent négatif fort).
2. **Empoisonnement d’un bus existant** : destination basculée vers un point critique, ou amount poussé vers la fermeture.
3. **Polarité** : un amount **négatif** vers VCA 1 / VCA 2 / ENV 2 Amp peut annuler un niveau Patch Edit encore « correct ».

Plusieurs bus peuvent **s’additionner** vers la même destination (Oberheim autorise de répéter un même routage) → extinction plus brutale.

### 6.3 Ce qui est sûr côté matrice

- `SOURCE` = **NONE** ou `DESTINATION` = **NONE** → bus inerte.
- Destinations hauteur, LFO speed/amp, portamento, FM, résonance, ENV 1/3 (hors cas exotiques) → en général **pas** d’extinction.

---

## 7. Cas secondaires (classe D) — plutôt « patch mort à jouer » que silence structurel

Utile à connaître, **moins prioritaire** si l’objectif est d’éviter le silence total entendu au premier Note On.

| Paramètre / combo | Effet |
|-------------------|--------|
| **ENV 2 · DELAY** très élevé | Silence long avant l’attaque |
| **ENV 2 · ATTACK** très élevé | Notes courtes quasi inaudibles |
| **ENV 2 · SUSTAIN** = 0 + **RELEASE** court | Son « clip » / sensation d’extinction |
| **ENV 1 · AMPLITUDE** (et AMP \< VELOCITY) très bas | Seulement si le patch **dépend** d’ENV 1 pour ouvrir un filtre sinon fermé |

---

## 8. Ce qui n’éteint pas le son (rappel)

En routage standard, ces modules **ne ferment pas** le chemin audio :

- **FM / TRACK** (FM amount, points de tracking),
- **RAMP / PORTAMENTO** (rates, modes, keyboard mode),
- **LFO 1 / LFO 2** (sauf s’ils sont routés en Matrix vers un VCA / filtre / mix),
- **ENV 3** (pré-câblée vers la FM du filtre),
- **ENV 1** en tant que forme seule (sauf dépendance filtre, §7),
- **DETUNE**, **SYNC**, **WAVE SHAPE**, **LEVERS**, **KEY CLICK**, **KEYBD/PORTA**.

---

## 9. Liste consolidée — pour garde-fous mutateur

### Priorité 1 — tueurs durs (floors / ne pas laisser descendre librement)

1. `VCA 1 VOLUME`
2. `VCA 2 \< ENV 2` *(sauf si un bus Matrix module déjà `VCA 2 VOLUME` de façon utile)*
3. `ENV 2 · AMPLITUDE`
4. Couples effectifs :
   - `VCA 1 VOLUME` + `VCA 1 \< VELOCITY`
   - `ENV 2 · AMPLITUDE` + `ENV 2 · AMP \< VELOCITY`
5. Bus Matrix : Amount (surtout négatif) vers `VCA 1 VOLUME`, `VCA 2 VOLUME`, `ENV 2 AMPLITUDE`

### Priorité 2 — étouffement / sources

6. `FREQUENCY` (VCF) couplé à `RESONANCE`
7. `FREQ \< ENV 1` (si cutoff bas)
8. Interdiction des deux `WAVE SELECT` = OFF
9. `DCO 2 \| DCO 1 MIX` extrême + DCO cible OFF
10. Bus Matrix vers `VCF FREQUENCY` ou `DCO 2 \| DCO 1 MIX`

### Priorité 3 — secondaires

11. `ENV 2` delay / attack extrêmes  
12. `PULSE WIDTH` / PWM si Pulse-only  
13. `ENV 1` amplitude si le filtre en dépend  

---

## 10. Formules mentales (interactions)

```text
Audible ≈
    SourceVivante(DCO1|DCO2, Mix)
  ∧ FiltreAssezOuvert(Freq, Res, Env1, MM→VcfFreq)
  ∧ Vca1Effectif(Volume, VelMod, MM→Vca1)
  ∧ Vca2Effectif(Vca2←Env2, Env2Amp, Env2Vel, MM→Vca2)
```

- Si **un** facteur tombe à zéro effectif → silence.
- `VCA 2 \< ENV 2` ≈ « gain du pré-câblage ENV2→VCA2 » : le baisser est sûr **seulement** si `MM→VCA 2` prend le relais.
- Vélocité = tueur **doux** : dépend du jeu ; dangereux surtout quand le niveau de base est déjà bas.

---

## 11. Références

- `_local/References/Oberheim/oberheim-matrix-1000-user-manual-personal-notes.md` — cascade VCA 1 / VCA 2
- `_local/References/Oberheim/oberheim-matrix-6r-basic-patch.md` — valeurs saines de référence (VA1=63, VA/E2=+63, Env2 Amp=40, etc.)
- `_local/References/Oberheim/oberheim-matrix-1000-midi-sysex-implementation.md` — Table 2/3 Matrix Modulation
- `inventaire-parametres-patch-edit-matrix-modulation.md` — inventaire paramètre par paramètre
- `.memlog.md` (même dossier) — décisions de calibration playable
)
