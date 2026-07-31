# Passation — Patch Name display & bank/Mutator naming (Matrix-Control)

**Date:** 2026-07-31  
**Repo:** Matrix-Control (JUCE plugin)  
**Langue chat avec Guillaume :** français, clair, sans jargon inutile  
**Objectif de cette passation :** en contexte frais, produire un **rapport complet et lisible** sur la gestion actuelle des noms affichés dans **Patch Name**, puis aider Guillaume à décider s’il faut conserver, restreindre ou retirer l’astuce « overlay » pour la RAM Matrix-1000.

Ne modifie pas le code dans ce premier passage : **rapport + options + recommandation** d’abord.

---

## Mission pour l’IA (premier message)

1. Lire cette passation + les fichiers SSOT listés plus bas.  
2. Vérifier dans le code actuel que le résumé ci-dessous est encore vrai (corriger le rapport si le code a divergé).  
3. Remettre à Guillaume un rapport structuré (modèle en fin de document).  
4. Mettre en avant la décision ouverte sur l’overlay RAM M-1000 (fragilité si la RAM est modifiée hors plugin).  
5. Distinguer clairement **Matrix-1000** vs **Matrix-6/6R**, et **affichage Patch Name** vs **noms dans fichiers .syx / export**.

---

## Décision produit ouverte (prioritaire)

Guillaume n’est **pas encore certain** de vouloir garder l’astuce suivante :

> Sur Matrix-1000, le firmware **enregistre le son** via SysEx mais **réécrit les octets 0–7 du nom** en placeholders du type `BNKx: yy` (constaté sur opcode `01H` **et** sur `0x0D` + `0x0E`). Matrix-Control mémorise alors de son côté le « vrai » nom (import / STORE) dans `PatchNameOverlayStore` et le **réaffiche** dans Patch Name quand le dump revient en `BNK…`.

**Fragilité reconnue :** si la RAM est modifiée **hors** Matrix-Control (autre éditeur, panneau machine, autre logiciel), l’overlay peut afficher un **vieux nom musical** alors que le son a changé — le synthé continuant de renvoyer `BNK…`, le plugin n’a pas de signal de désync.

Le rapport doit présenter des options product-level (garder / documenter le risque / invalider l’overlay dans certains cas / n’afficher que `BNK…` ou `Bx-Pyy` en RAM, etc.) avec **Recommandation BMad** (option + pourquoi + confiance ferme/nuancée).

---

## Faits hardware (ne pas rediscuter sans nouvelle preuve)

Mesure MIDI (logs projet, import bank 2 → bank 0) :

- Envoi d’un patch avec nom musical (ex. `PAPANO 4`) + paramètres.  
- Relecture : **paramètres identiques**, **seuls les 8 octets de nom** deviennent `BNK0: 00` (etc.).  
- Memory Protect OFF : si Protect était ON, le dump serait **ignoré entièrement** (sons compris) — ce n’est pas le symptôme observé.  
- Chemin `0x0D` (edit buffer) + `0x0E` (store edit buffer) : **même réécriture BNK**.  
- Interprétation produit : le M-1000 a un affichage **3 chiffres** ; les octets nom existent surtout pour **compatibilité format Matrix-6/6R** ; en pratique le M-1000 traite souvent ces octets comme **étiquette de case**, pas comme nom musical durable via SysEx.

Le code d’écriture bank import est revenu au chemin rapide **`01H` (`sendPatch`)** ; l’overlay compense l’affichage.

---

## Comportement actuel attendu (à valider dans le code)

### A. Afficheur Patch Name — après dump device (`loadCurrentPatchFromDevice` → `applyResolvedPatchName`)

Fichier pivot : `Source/Core/Actions/PatchManagerActionHandler.cpp` (`applyResolvedPatchName`).

Ordre de résolution :

1. **Banque ROM Matrix-1000** (`limits.isRomBank(bank)`, typiquement banks 2–9)  
   - Si entrée dans `Matrix1000FactoryPatchNames::nameFor(bank, patch)` → **nom d’usine** (table documentaire), même si le dump hardware est `BNK…`.  
   - Sinon, suite des règles ci-dessous.

2. **Si le nom device est placeholder `BNK…` OU inutilisable (vide)**  
   - Chercher `PatchNameOverlayStore` pour `(bank, patch)`.  
   - Si trouvé → **afficher l’overlay** (noms « musicaux » mémorisés).

3. **Si toujours inutilisable**  
   - Fallback éditeur : `B{bank}-P{patch}` via `PatchFileNameSanitizer::formatBankPatchLabel`  
   - **Attention :** ce n’est **pas** le format hardware `BNKx: yy` ; c’est le label plugin `Bx-Pyy`.

4. **Sinon** (nom device utilisable et **pas** `BNK…`)  
   - **Garder le nom du dump** tel quel.

Conséquences :

| Contexte | Ce que Patch Name tend à montrer |
|----------|-----------------------------------|
| M-1000 ROM 2–9 | Noms d’usine (table), pas `BNK…` |
| M-1000 RAM 0–1 après IMPORT/STORE via le plugin | Nom overlay (ex. `PAPANO 4`) si mémorisé ; sinon `BNK…` du hardware ou `Bx-Pyy` si vide |
| M-1000 RAM jamais touchée par overlay | Souvent `BNKx: yy` renvoyé par le hardware |
| Matrix-6/6R (`!hasBankConcept()`) | Pas de table ROM M-1000 ; pas de banques 0–9. Dump → nom des octets 0–7 s’ils sont utilisables ; sinon overlay si une entrée existe ; sinon `B0-Pxx` (bank clampée). **Sur 6/6R les vrais noms dans les octets sont en principe le comportement normal** (afficheur / usage nom) — vérifier si l’overlay est jamais peuplé sur 6/6R. |

### B. Overlay — quand on mémorise / oublie

Classe : `Source/Core/Services/PatchNameOverlayStore.{h,cpp}`  
Persistance APVTS : propriété enfant `patchManagerPatchNameOverlay` (`PluginIDs` Bank Utility state).  
Rechargé après chargement de session (`reloadPatchNameOverlayFromApvts` via `resetComputerPatchesBrowserAfterSessionLoad`).

Peuplé notamment par :

- **Bank IMPORT** : à chaque slot écrit → `rememberOverlayName(bank, slot, nom du payload importé)`.  
- **STORE** interne : `rememberOverlayName(currentBank, currentPatch, patchModel_->getName())`.  
- **Cancel import + restore snapshot** : si le snapshot a un nom musical non-`BNK` → remember ; sinon → `forget` ce slot.

### C. Fichiers `.syx` (Computer Patches / load fichier)

- Chargement via `PatchFileService` + éventuelle réconciliation nom fichier vs buffer (`PatchFileNameReconciler`).  
- Le nom affiché vient du **modèle / APVTS** après load (octets du fichier, éventuellement réconciliés avec le stem).  
- **Pas** la table ROM, **pas** (en général) l’overlay device — sauf si plus tard un dump device réapplique `applyResolvedPatchName`.

Import bank (préparation buffer avant envoi) :

- Si payload vide ou `BNK…` → récupérer le nom depuis le stem fichier export bank (`Pxx - Name` via `nameFromBankExportStem`) et le **tamponner dans le packed** avant envoi + overlay.

### D. Bank Utility EXPORT (Matrix-1000)

- Dump live slots 0–99 de la banque sélectionnée.  
- Avant écriture fichier : `applyResolvedPatchName` sur le patch dumpé (donc ROM → table usine ; RAM → overlay si `BNK` ; etc.).  
- Stem fichier : `Pxx - {Name}.syx` (`bankExportFileStem`) ; le payload `.syx` est sauvegardé **avec le nom résolu** (stamp dans les octets), pas seulement le filename.  
- Dossier : `BANK N`.

### E. Bank Utility IMPORT (Matrix-1000, RAM 0–1 seulement)

- Snapshot device → écriture `sendPatch` (`01H`) avec délai inter-messages tenant compte du temps câble (~120 ms).  
- Noms musicaux : stamp éventuel depuis filename si payload `BNK`/vide ; **overlay** après chaque écriture (car le hardware renverra `BNK`).  
- Fin d’import : applique le slot courant dans l’éditeur puis dump différé ; l’affichage durable des noms repose sur l’overlay + `applyResolvedPatchName`.

Matrix-6/6R : import/export sur la mémoire unique (dossier `PATCHES`) ; pas de concept banques 0–9 ROM/RAM M-1000.

### F. Patch Mutator — export dossiers

- Contexte : `PatchLoadContext` (`deviceMemory` vs `computerFile`).  
- Basename session export device : `{Name} @ Bx-Pyy` (`formatBankPatchLabel` + nom patch courant sanitizé).  
  - Le `{Name}` est le **nom actuel dans le patch model** (donc peut être overlay / factory / dump selon ce qui a été chargé avant).  
  - Le suffixe lieu est **`Bx-Pyy`**, pas `BNKx: yy`.  
- Origine fichier : `{Name} @ SyxFile` (ou stem fichier si nom vide).  
- Fichiers internes Mutator (`Initial.syx`, roots/retries) : naming `MutationNaming` ; stamp du **nom utilisateur courant** dans le buffer à l’export fichier (`PatchFileService::writeExportPatchFile`).

---

## Carte code (ordre de lecture suggéré)

1. `Source/Core/Actions/PatchManagerActionHandler.cpp` — `applyResolvedPatchName`, dump device, STORE, bank import/export  
2. `Source/Core/Services/PatchNameOverlayStore.{h,cpp}`  
3. `Source/Core/Services/PatchFileNameSanitizer.{h,cpp}` — `BNK`, `Bx-Pyy`, stems bank export  
4. `Source/Core/Services/Matrix1000FactoryPatchNames.{h,cpp}` — ROM 2–9  
5. `Source/Core/Services/DeviceMemoryLimits.*` — ROM/RAM, `hasBankConcept`  
6. `Source/Core/Services/PatchMutator/PatchLoadContext.*` — basenames Mutator  
7. `Source/Core/Services/PatchFileService.*` — `.syx` load/save, stamp nom  
8. `Source/Core/Models/PatchModel.*` — octets 0–7, encoding nom  
9. Specs / contexte :  
   - `_bmad-output/implementation-artifacts/spec-bank-utility-import-export.md`  
   - `_bmad-output/reference-docs/oberheim/oberheim-matrix-1000-midi-sysex-implementation.md` (bytes 0–7 = nom 6-bit)  
   - Logs MIDI historisant la preuve BNK : `Logs/MIDI/midi-log-2026-07-27-23-47-51.txt` (0x0D/0x0E puis dumps `BNK0:…`)

---

## Modèle de rapport demandé à Guillaume

Répondre en français, structure claire :

### 1. En bref  
Une phrase : comment Patch Name est alimenté aujourd’hui, et le nœud de décision overlay RAM.

### 2. Matrice d’affichage Patch Name  
Tableau ou sections : M-1000 RAM / M-1000 ROM / M-6/6R / fichier `.syx` — source du nom, fallback, cas limites.

### 3. Import / Export banques M-1000  
Ce qui part sur le wire, ce qui est stampé dans les `.syx`, ce que l’UI montre après coup.

### 4. Export Patch Mutator  
Règle de nommage dossiers / fichiers et lien avec le nom affiché.

### 5. Overlay RAM — robustesse  
- Ce qui est solide  
- Scénario de désync (modification hors plugin)  
- Ce que l’utilisateur voit alors (risque concret)

### 6. Options produit (si on touche à l’overlay)  
3–5 options en langage quotidien + pour chacune : effet utilisateur, effort, risque.  
**Recommandation BMad** obligatoire (option + pourquoi + confiance).

### 7. Écarts code vs cette passation  
Liste courte si le code a changé.

### 8. Prochaine étape proposée  
Sans implémenter tant que Guillaume n’a pas tranché.

---

## Contraintes de communication

- Français accessible ; noms produit en anglais inchangés (`Patch Name`, `Bank Utility`, `Patch Mutator`, etc.).  
- Chemins / symboles en fin de paragraphe ou ligne technique, pas dans la phrase porteuse de sens.  
- Ne pas présenter l’overlay comme « le firmware est corrigé » — c’est une **couche éditeur**.  
- Ne pas committer / ne pas coder avant validation explicite de Guillaume sur la direction produit.
