# Intention — Patch Mutator jouable

**Projet :** Matrix-Control  
**Sujet :** calibrer le Patch Mutator pour que MUTATE / RETRY restent audibles et intéressants  
**Session :** août 2026 (brainstorming)  
**Hors scope immédiat :** create-story / code (à faire ensuite, sur demande)

---

## Intention produit

Après une mutation, le musicien doit entendre un son **présent** et **intéressant** — pas un silence, pas un filtre complètement étouffé.

Quand le toggle `MM` (**Matrix Modulation**) est actif, le Patch Mutator doit surtout **révéler** la richesse de la matrice (enveloppes, LFO, Track Generator, rampes → timbre et mouvement), pas la rendre dangereuse ou inutile.

**Jouable ne veut pas dire timide.** On distingue deux choses :

1. **Distance artistique** — à quel point on s’éloigne du patch de départ.  
2. **Sécurité sonore** — on ne livre jamais un patch structurellement muet, quel que soit le mode.

---

## Ce que l’utilisateur voit

Dans le panneau Patch Mutator, remplacer les curseurs Amount et Random par deux listes :

### MODE (Kindred → Drift → Warp → Wild)

| Mode | Sens |
|------|------|
| **Kindred** | Cousin proche : on garde l’essence du patch |
| **Drift** | Ça dérive, encore reconnaissable |
| **Warp** | Déformation audacieuse |
| **Wild** | Tout est permis **sauf** le silence |

Chaque mode fixe en coulisse une profondeur de mutation et une dose d’aléatoire (constantes dans le code, à affiner aux tests). L’utilisateur ne programme plus Amount / Random à la main.

### PITCH (liste à deux niveaux, comme mutations / retries)

| Choix | Sens |
|-------|------|
| **Preserve** | La hauteur des deux oscillateurs reste intacte (Patch Edit **et** Matrix Modulation) |
| **Consonant** → **± 1 Oct** … **± 5 Oct** | Sauts vers des consonances (quinte, quarte, tierces, sixtes, octaves…), dans une plage de ± X octaves **autour de la hauteur actuelle** (bornée 0…63) |
| **Dissonant** → **± 1 Oct** … **± 5 Oct** | Même logique de plage, mais sauts vers des dissonances assumées (secondes, septièmes, triton) — pour des nappes / textures où la tension fait le caractère (réf. musicale : intro d’*Inner City Life*, Goldie) |
| **Free** | Hauteur libre (glissements selon l’agressivité du MODE), toujours bornée 0…63 |

**Plage ± X octaves :** on ne promet pas « X octaves vers le haut » de façon absolue. Pour chaque oscillateur, la fenêtre va d’environ X octaves en dessous à X octaves au-dessus de sa valeur actuelle, sans sortir de 0…63. Près du plancher, surtout de la place vers le haut ; près du plafond, surtout vers le bas.

**Deux oscillateurs mutables :**
- **Kindred / Drift** — on **préserve l’écart** entre DCO 1 et DCO 2 (ex. doublage à l’octave).
- **Warp / Wild** — chaque DCO peut sauter **indépendamment**.

Les toggles de modules (D1…L2 + MM) restent la façon d’ouvrir ou fermer des familles de paramètres. **On ne modifie jamais un module dont le toggle est éteint** (pas de triche). On peut en revanche **lire** tout le patch pour décider intelligemment (ex. : un bus Matrix Modulation ouvre-t-il déjà le VCA 2 ?).

---

## Règles sonores essentielles

**Toujours actifs** (tous les modes) :

- Ne pas laisser la chaîne de volume se fermer (VCA 1, ouverture VCA 2 par ENV 2, amplitude d’ENV 2).  
- Ne pas laisser filtre très fermé **et** résonance très faible en même temps.  
- Ne jamais éteindre les deux oscillateurs.  
- Si le mix pointe vers un oscillateur éteint → corriger.  
- Sur Matrix Modulation : freiner les montants qui ferment volume / filtre / mix dangereux.

**Nuances importantes :**

- **VCA 1 VOLUME** : intouchable en Kindred / Drift / Warp ; en Wild seulement un tout petit mouvement + seuil de sécurité.  
- **VCA 2 ← ENV 2** : rester haut, sauf si Matrix Modulation pilote déjà le VCA 2 (cas de design volontaire).  
- **ENV 2** = enveloppe de **volume** (pré-câblée) → plus stricte qu’ENV 1 (filtre) ou ENV 3 (FM).  
- Si une enveloppe (ou un LFO) devient le **seul** robinet de volume via Matrix Modulation → appliquer les mêmes seuils durs.  
- Un LFO qui module le volume fait en général un **tremolo**, pas un silence permanent — sauf s’il est le seul à ouvrir le VCA.

**Fidélité du patch de départ :**

- Patch à **deux** oscillateurs actifs → on garde deux sources.  
- Patch à **un** seul → on garde un seul (on peut permuter lequel).  
- Bruit déjà présent → on le conserve ; sinon, on peut en ajouter un peu en modes explorateurs.

En **Kindred**, Matrix Modulation ne touche **presque que les montants** des bus déjà actifs — le nom du mode doit dire vrai.

---

## Socle technique (pour la suite)

Dès la première version d’implémentation :

- Une **table de politiques** par paramètre (libre / borné / verrouillé / choix restreint / correction après coup), extensible.  
- Un **passage après mutation** pour les règles qui regardent plusieurs paramètres ensemble.  
- Des **constantes** Amount / Random par mode, faciles à retoucher pendant les essais.

Les paramètres Amount / Random restent dans l’algorithme ; ils ne sont plus exposés en tant que curseurs dans l’interface.

### Pistes musicales algo (v1, sans élargir la GUI)

1. **Mutation plus clairsemée** en Kindred (moins de paramètres, gestes plus clairs).  
2. **Gestes de famille** (ex. contour d’enveloppe lié ; filtre + résonance ensemble).  
3. **Biais Matrix Modulation** vers des paires expressives (ENV / LFO / Track / Rampes → timbre / mouvement).  
4. **Vitesses de LFO** qui « respirent » (éviter extrêmes inutiles en Kindred).  
5. **RETRY** qui évite un résultat trop proche du précédent.  
6. Si **MM** est allumé : garantir au moins un **mouvement** audible après mutation.  
7. **Pitch** Consonant / Dissonant / écart DCO selon MODE (déjà ci-dessus).

Maquette Figma : MODE + PITCH en deux listes de 56 px — **validé**, ça rentre à la place de Amount / Random.

---

## Priorité de livraison (retenue en session)

**À faire dans la v1 (Must) :** couche de politiques + corrections après mutation ; garde-fous anti-silence ; interface MODE + PITCH ; constantes par mode ; respect des toggles ; Preserve global ; cardinalité des oscillateurs / Noise ; règles VCA 2 ← ENV 2 ; escalade selon le rôle réel dans Matrix Modulation ; coup de pouce FM en Warp / Wild ; Kindred Matrix Mod = montants seuls ; **les 7 pistes musicales algo** ci-dessus.

**Souhaitable ensuite (Should) :** petit indicateur si un garde-fou a corrigé le résultat ; jeux de tests « patch mort → patch jouable » ; Unison / Keyboard Mode plus protégés jusqu’à Warp ; ajout mesuré de Noise.

**Plus tard (hors calibration playable) :** aide contextuelle furtive sur le footer (survol + focus clavier), d’abord pour C / D / F / E du Patch Mutator — voir `parking-aide-contextuelle-footer.md` (bande des messages / bande système / bande appareil).

**Pas maintenant :** cadenas devant chaque contrôle Patch Edit ; changer la formule mathématique de base Amount × Random ; create-story dans le brainstorming ; info-bulles flottantes comme solution principale.

---

## Prochaine étape recommandée

1. Court **addendum** à la spec d’algorithme de mutation (table de politiques, MODE → constantes, sauts Pitch, échelle Matrix Modulation, pistes musicales).  
2. Puis **implémentation** quand tu le lanceras (`bmad-build` / story).  
3. Finaliser la maquette Figma MODE + PITCH (côté Guillaume).

**Documents liés :** dossier  
`_bmad-output/brainstorming/brainstorm-patch-mutator-playable-calibration-2026-08-26/`  
(synthèse EN, intention FR, inventaire, extinctions, interactions, parking aide footer, memlog).
