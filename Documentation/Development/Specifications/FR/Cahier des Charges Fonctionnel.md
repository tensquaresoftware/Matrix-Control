# Matrix-Control

## Cahier des Charges Fonctionnel (CdCT)

**Auteur :** Guillaume DUPONT  
**Organisation :** Ten Square Software  
**Date de modification :** 2025-11-18  
**Version :** 1.0.0

---

## 1. Généralités et contexte

### 1.1 Objectif du projet

**Matrix-Control** est un éditeur MIDI pour le synthétiseur **Oberheim Matrix-1000**. Il se présente sous la forme d'un **plugin VST/AU** et **application standalone**, permettant aux utilisateurs d'éditer graphiquement les paramètres du synthé en temps réel via des **messages MIDI / SysEx**.

**Fonctionnalités principales :**

- Édition complète des paramètres du mode PATCH (Sections PATCH EDIT + MATRIX MODULATION)
- Édition des paramètres du mode MASTER (Section MASTER EDIT)
- Gestion des patches : chargement/sauvegarde (mémoire du synthé + fichiers .syx locaux)
- Synchronisation bidirectionnelle plugin ↔ synthé (Program Changes + messages MIDI / SysEx)
- Interface graphique redimensionnable avec zoom (50-400%), 2 thèmes fournis (Black/Cream)

### 1.2 Public ciblé

- Musiciens et compositeurs utilisant un synthétiseur Oberheim Matrix-1000
- Sound designers cherchant une alternative graphique à l'édition hardware
- Utilisateurs possédant un Matrix-1000 avec EPROM Tauntek (recommandé)
- Utilisateurs de DAW modernes (Ableton Live, Steinberg Cubase, Apple Logic Pro, Reason Studios, etc.)

### 1.3 Contexte technique

Le projet repose sur :

- **Framework :** JUCE 8
- **Langage :** C++17
- **Plateformes :** macOS Tahoe 26 + Xcode 26 / Windows 11 + Visual Studio 2026
- **Formats du plugin :** VST3 (macOS & Windows), AU2 (macOS)
- **Synthétiseur ciblé en v1.0 :** Oberheim Matrix-1000 uniquement
- **Développement futur :** Compatibilité avec les synthés Oberheim Matrix-6/6R (envisagée en v2.0)

### 1.4 Documents complémentaires

**Cahier des Charges Technique :**

- Architecture générale et patterns
- Classes métier et responsabilités
- Threading et gestion MIDI
- Structure du projet, phases de développement
- Détails techniques d'implémentation (protocole MIDI, algorithmes, packing/unpacking)

**Cas d'utilisation :**

- Workflows critiques et scénarios d'interaction
- Scénarios nominaux et alternatifs détaillés
- Gestion des cas limites et erreurs

**Référence MIDI/SysEx :**

- Oberheim Matrix-1000 - MIDI & SysEx Implementation

---

## 2. Exigences fonctionnelles

### 2.1 Section PATCH EDIT

Édition complète d'un patch du synthé via 10 modules de synthèse (96 paramètres) :

DCO 1, DCO 2, VCF/VCA, FM/TRACK, RAMP/PORTAMENTO, ENV 1, ENV 2, ENV 3, LFO 1, LFO 2.

#### Initialisation des paramètres

Le plugin propose 4 actions d'initialisation par module :

| Action                       | Fonction                                                                    |
| ---------------------------- | --------------------------------------------------------------------------- |
| Double-clic sur un paramètre | Réinitialise le paramètre individuel (depuis INIT.syx ou valeur par défaut) |
| Bouton `I` (Init Module)     | Réinitialise tous les paramètres du module                                  |
| Bouton `C` (Copy Module)     | Copie le module dans le presse-papiers (type-aware, persistant)             |
| Bouton `P` (Paste Module)    | Colle les paramètres compatibles depuis le presse-papiers                   |

**Contraintes :**

- Fichier INIT.syx : `~/Music/Matrix-Control/INIT.syx` (configurable via Préférences)
- Fallback : Valeurs par défaut du plugin si fichier absent/invalide
- Message si fichier invalide : `"No valid INIT.syx file found > Using plugin default values"`
- Délai inter-messages SysEx séquentiels : 10 ms minimum
- Presse-papiers : Persistant entre patches, validation de compatibilité requise

**Comportement du bouton INIT :**

- Charge les données de patch depuis le fichier INIT.syx (ou valeurs par défaut si indisponible)
- Envoie immédiatement ce patch au synthé
- Affiche le message : `"Patch initialized"`

**Cas d'utilisation :**

- **UC-02** : Initialisation d'un paramètre
- **UC-03** : Copier-coller de module

#### Contraintes logiques

Gestion particulière du paramètre `Legato Portamento` du module RAMP/PORTAMENTO :

- **Condition d'activation :** `LEGATO PORTA` est utilisable uniquement si `KEYBOARD MODE` = `UNISON`
- **Désactivation automatique :** `LEGATO PORTA` est grisé si `KEYBOARD MODE` ≠ `UNISON`
- **Correction automatique :** Si `KEYBOARD MODE` passe de `UNISON` à une autre valeur avec `LEGATO PORTA` = `ON`, le plugin force `LEGATO PORTA` = `OFF` et affiche : `"LEGATO PORTA can only be activated if KEYBOARD MODE = UNISON"`

En dehors du paramètre `LEGATO PORTA`, tous les paramètres du mode PATCH restent actifs, sans grisage. L'utilisateur peut les éditer librement, le synthé gère les éventuelles contraintes en interne.

**Cas d'utilisation :**

- **UC-09** : Contrainte LEGATO PORTA / KEYBOARD MODE

#### Paramètre VCF Frequency : traitement spécial

Le paramètre `VCF Frequency` (paramètre 121) dispose d'un traitement spécial lors de la sérialisation des données patch :

- **Tous autres paramètres signés** : Plage signée standard avec extension de signe
- **VCF Frequency uniquement** : Utilise la plage 0-127 sans extension de signe

Cette distinction est nécessaire pour garantir la compatibilité avec le format SysEx du Matrix-1000.

**Référence :** Oberheim Matrix-1000 - MIDI & SysEx Implementation, section "Single Patch Data Format".

### 2.2 Section MATRIX MODULATION

La matrice de modulation complète les paramètres de synthèse. Elle donne accès à 10 bus de modulation identiques, numérotés de 0 à 9, et comportant chacun 3 paramètres : `Source` / `Amount` / `Destination`.

| Paramètre     | Type        | Plage                         | Comportement |
| ------------- | ----------- | ----------------------------- | ------------ |
| `Source`      | Énumération | None, Env1-3, LFO1-2, ...     | Jamais grisé |
| `Amount`      | Signé       | -63 à +63                     | Jamais grisé |
| `Destination` | Énumération | None, Dco1 Freq, Dco1 PW, ... | Jamais grisé |

**Comportement :**

- Toute modification d'un paramètre envoie immédiatement le message SysEx correspondant au synthé
- Le message SysEx encapsule uniquement les 3 paramètres du bus modifié

**Paramètres du message SysEx 0BH :**

```
F0H 10H 06H 0BH <bus> <source> <amount> <destination> F7H
```

- `<bus>` : Numéro du bus (0-9)
- `<source>` : 0 pour supprimer le bus, 1-20 pour sélectionner une source
- `<amount>` : Modulation amount (valeur signée), plage -63 à +63
- `<destination>` : 0 pour supprimer le bus, 1-32 pour sélectionner une destination

**Comportement de suppression :**

Si `Source` OU `Destination` = 0, le bus est considéré comme vide/désactivé.

**Référence :** Oberheim Matrix-1000 - MIDI & SysEx Implementation, section "0BH - Remote Parameter Edit (Matrix Modulation)".

### 2.3 Section PATCH MANAGER

Cette section n'existe pas en tant que telle dans le synthé, elle augmente les possibilités d'utilisation du Matrix-1000. Elle comporte 4 modules : PATCH NAME, BANK UTILITY, INTERNAL PATCHES et COMPUTER PATCHES.

#### Module PATCH NAME

- Affiche le nom du patch chargé (8 caractères / format ASCII simplifié Oberheim)
- Au démarrage du plugin : Affiche `--------` pour signifier "Aucun patch chargé"

#### Module BANK UTILITY

| Contrôle                            | Fonction                                                                                                          |
| ----------------------------------- | ----------------------------------------------------------------------------------------------------------------- |
| SELECT BANK :<br/>Boutons `0` à `9` | Sélectionne et verrouille automatiquement la banque courante<br/>Message affiché : `"Bank X selected and locked"` |
| Bouton `UNLOCK`                     | Déverrouille la banque courante<br/>Message affiché : `"Current bank unlocked"`                                   |

**Implémentation technique :**

- Les boutons `0` à `9` de SELECT BANK envoient un message SysEx spécifique au synthé pour sélectionner et verrouiller la banque
- Le synthé répond en changeant de banque ET en activant automatiquement le Bank Lock
- Le bouton `UNLOCK` envoie un message SysEx spécifique pour déverrouiller la banque

**Référence :** Oberheim Matrix-1000 - MIDI & SysEx Implementation, sections "0AH - Set Bank" et "0CH - Unlock Bank".

#### Module INTERNAL PATCHES

Navigation dans les banques et les mémoires de patches du synthé.

##### Groupe BROWSER

| Contrôle                     | Description                                                                                                                                                                            |
| ---------------------------- | -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| Afficheur<br/>`Bank Number`  | Nombre entier (0–9) en lecture seule<br/>Modifiable via le module BANK UTILITY                                                                                                         |
| Afficheur<br/>`Patch Number` | Nombre entier (00–99) éditable via double-clic<br/>Envoi d'un Program Change si valide<br/>Message d'erreur si invalide : `"Patch number is invalid : it must be in the range 00–99."` |

**Cas d'utilisation :**

- **UC-05** : Édition du Patch Number via un double-clic

##### Groupe UTILITY

| Contrôle               | Fonction                                                                                                                                                                 |
| ---------------------- | ------------------------------------------------------------------------------------------------------------------------------------------------------------------------ |
| Boutons<br/>`<` et `>` | Navigation circulaire dans les patches<br/>• Banque verrouillée : navigation dans cette banque uniquement<br/>• Banque déverrouillée : navigation globale (1000 patches) |
| Bouton<br/>`INIT`      | Charge le patch depuis INIT.syx (ou valeurs par défaut) et l'envoie immédiatement au synthé                                                                              |
| Bouton<br/>`COPY`      | Copie le patch courant dans le presse-papiers                                                                                                                            |
| Bouton<br/>`PASTE`     | Colle le patch du presse-papiers<br/>• Actif si presse-papiers compatible ET banque 0-1 (RAM)<br/>• Grisé si banque 2–9 (ROM)                                            |
| Bouton<br/>`STORE`     | Sauvegarde le patch dans la mémoire du synthé<br/>• Actif si banque 0–1 (RAM)<br/>• Grisé si banque 2–9 (ROM)                                                            |

**Implémentation du bouton STORE :**

- Envoie un message SysEx spécifique au synthé pour sauvegarder l'edit buffer en mémoire
- Affiche le message : `"Patch stored to memory"`

**Référence :** Oberheim Matrix-1000 - MIDI & SysEx Implementation, section "0EH - Store Edit Buffer".

**Cas d'utilisation :**

- **UC-01** : Charger un patch depuis la mémoire interne
- **UC-04** : Navigation dans les banques verrouillées / déverrouillées

#### Module COMPUTER PATCHES

Navigation dans un dossier de patches de l'ordinateur, stockés sous forme de fichiers .syx locaux.

##### Groupe BROWSER

| Contrôle                  | Description                                                                                                                                     |
| ------------------------- | ----------------------------------------------------------------------------------------------------------------------------------------------- |
| Boutons<br/>`<` et `>`    | Navigation circulaire dans les patches valides du dossier<br/>• Actifs si au moins 2 patches valides<br/>• Grisés sinon                         |
| ComboBox<br/>`Patch File` | Liste des patches valides triés alphabétiquement<br/>• Grisé si aucun dossier ouvert ou aucun patch valide<br/>• Affiche `"No Patch !"` si vide |

##### Groupe FOLDER & FILES

| Contrôle             | Fonction                                                                                                                                                                                                 |
| -------------------- | -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| Bouton<br/>`OPEN`    | Ouvre un dossier via dialogue système<br/>Scanne et valide les fichiers .syx (structure + checksum)<br/>Charge le premier patch valide<br/>Message affiché : `"Found X .syx files (Y valid, Z invalid)"` |
| Bouton<br/>`SAVE AS` | Sauvegarde via dialogue système<br/>Mémorise le chemin complet du fichier dans les Préférences<br/>Active le bouton `SAVE`                                                                               |
| Bouton<br/>`SAVE`    | Sauvegarde rapide (écrase le fichier mémorisé sans confirmation)<br/>Grisé par défaut au démarrage                                                                                                       |

**Contraintes :**

- Le chemin mémorisé dans les Préférences du plugin est le chemin complet du dernier fichier sauvegardé via `SAVE AS`
- Chaque nouveau `SAVE AS` remplace le chemin précédent
- Le bouton `SAVE` utilise toujours le dernier chemin mémorisé

**Cas d'utilisation :**

- **UC-08** : Workflow SAVE AS → SAVE
- **UC-10** : Validation fichier .syx

### 2.4 Section MASTER EDIT

Édition des paramètres globaux du synthé, répartis en 3 modules : MIDI, VIBRATO et MISC.

#### Module MIDI

Paramètres : Channel, MIDI Echo, Controllers, Patch Changes, Pedal 1 Select, Pedal 2 Select, Lever 2 Select, Lever 3 Select.

**Note :** Le plugin ne supportera pas l'édition via les contrôleurs Registered/Non-Registered Parameters (RPN/NRPN). Les contrôleurs MIDI suivants sont documentés dans la référence MIDI/SysEx mais leur implémentation reste optionnelle pour cette version : Increment (96), Decrement (97), Non-Reg Parm LSB (98), Non-Reg Parm MSB (99), Reg Parm LSB (100), Reg Parm MSB (101).

#### Module VIBRATO

Paramètres : Speed, Waveform, Amplitude, Speed Mod Source, Speed Mod Amount, Amp Mod Source, Amp Mod Amount.

#### Module MISC

Paramètres : Master Tune, Master Transpose, Bend Range (+/-), Unison, Volume Invert, Bank Lock, Memory Protect.

#### Cycle d'édition

Les paramètres de la section MASTER EDIT ne sont pas adressables individuellement via des messages SysEx spécifiques. Toute modification d'un paramètre MASTER entraîne l'envoi du message SysEx MASTER complet (30+ paramètres).

**Comportement :**

- **FETCH initial :** Récupération du MASTER complet effectuée une seule fois au démarrage du plugin
- **EDIT :** Modification d'un paramètre par l'utilisateur
- **UPDATE :** Mise à jour de la valeur correspondante dans le MasterModel local du plugin
- **SEND :** Envoi du MASTER complet au synthé

Tous les paramètres de la section MASTER EDIT restent actifs, ils ne sont jamais grisés.

**Cas d'utilisation :**

- **UC-06** : Édition d'un paramètre du MASTER

### 2.5 Menu Settings

Persistance des préférences du plugin dans la session du DAW (via un APVTS) ou un fichier local de préférences (Exemple : `~/Music/Matrix-Control/Settings.json`).

| Paramètre                           | Type                                               | Défaut                          |
| ----------------------------------- | -------------------------------------------------- | ------------------------------- |
| Thème graphique                     | Black / Cream                                      | Black                           |
| Facteur de zoom                     | 50%, 75%, 100%, 110%, 125%, 150%, 200%, 300%, 400% | 100%                            |
| Redimensionnement manuel            | Facteur de zoom calculé par le plugin en %         | -                               |
| Port d'entrée MIDI                  | String                                             | Auto-détection                  |
| Port de sortie MIDI                 | String                                             | Auto-détection                  |
| Chemin du fichier INIT.syx          | File path                                          | ~/Music/Matrix-Control/INIT.syx |
| Dernier fichier de patch sauvegardé | File path                                          | Vide                            |

### 2.6 Barre de messages

Affiche en temps réel des messages utilisateur en bas de la fenêtre du plugin. Le dernier message reste affiché 3 secondes à l'écran puis disparaît en fondu.

Types de messages et codes couleur : Info (gris), Succès (vert), Avertissement (orange), Erreur (rouge).

**Exemples de messages :**

```
Info    : "Patch loaded: WarmPad"
Succès  : "Patch saved to disk: ~/Music/MyPatches/MyBrass.syx"
Avertis : "No valid INIT.syx file found > Using plugin default values"
Erreur  : "MIDI device disconnected"
```

---

## 3. Exigences techniques

### 3.1 Plateformes et versions

| Aspect             | Spécifications                                                                                     |
| ------------------ | -------------------------------------------------------------------------------------------------- |
| **macOS**          | Tahoe 26 / Xcode 26                                                                                |
| **Windows**        | Windows 11 / Visual Studio 2026                                                                    |
| **Plugin formats** | VST3 (macOS & Windows), AU2 (macOS)                                                                |
| **Framework**      | JUCE 8.0.10 (dernière version stable)                                                              |
| **Langage**        | C++17                                                                                              |
| **Synthé**         | • Matrix-1000 uniquement (v1.0)<br/>• Compatibilité secondaire avec les Matrix-6/6R, sans garantie |

### 3.2 Contraintes architecturales

#### Threading

| Thread           | Responsabilités                       | Contraintes                                                                                         |
| ---------------- | ------------------------------------- | --------------------------------------------------------------------------------------------------- |
| **Audio**        | `processBlock()` appelé par le DAW    | • NON bloquant !<br/>• PAS d'entrée/sortie MIDI !<br/>• Mises à jour uniquement via APVTS           |
| **GUI**          | User events, APVTS listeners          | • Mise à jour des composants<br/>• Envoi des commandes MIDI                                         |
| **MIDI (dédié)** | Send/receive SysEx, timeouts, parsing | • Aspects bloquants OK<br/>• Entrée/sortie MIDI autorisées<br/>• Mises à jour thread-safe via APVTS |

#### Communication inter-threads

Se fait via un objet APVTS (thread-safe via listeners) + lock-free FIFO pour les messages du thread MIDI.

#### Performance

- **Latence SysEx :** < 100 ms entre une action utilisateur et l'envoi du message au synthé
- **Délai inter-messages SysEx :** 10 ms minimum entre deux envois de messages SysEx séquentiels (ou entre patches)
- **Timeout MIDI :** 2 secondes (affichage d'un message d'erreur si le synthé ne répond pas)
- **En cas d'erreur MIDI :** Pas de nouvel essai automatique, l'utilisateur doit manuellement rééditer le paramètre

#### Robustesse

- **Pas de blocage UI** lors d'une déconnexion MIDI ou d'un timeout synthé (l'édition locale reste possible)
- **Validation des messages SysEx :** Structure (F0...F7), longueur cohérente avec le type de message, checksum valide
- **Gestion gracieuse des erreurs :** Exceptions métier + propagation vers la GUI via l'APVTS

### 3.3 Communication MIDI et SysEx

#### Envoi de messages SysEx

Chaque modification d'un paramètre entraîne l'envoi immédiat du message SysEx correspondant au synthé, via le port de sortie MIDI du plugin.

#### Réception de messages SysEx

Le plugin écoute en permanence le port d'entrée MIDI du plugin, afin de capturer les réponses du synthé.

#### Validation des messages SysEx

Tous les messages SysEx reçus du synthé doivent être validés selon les critères suivants :

- **Structure :** Message commence par F0H et se termine par F7H
- **Checksum :** Validation du checksum Oberheim
- **Longueur :** Cohérente avec le type de message (PATCH ≠ MASTER)

Si un message SysEx ne satisfait pas ces critères, il est rejeté et un message d'erreur est affiché. Aucun message invalide ne modifie l'état du plugin.

#### Format des données SysEx

Toutes les données transmises dans les messages SysEx sont encodées en format "unpacked" (2 nibbles par byte : nibble bas d'abord, puis nibble haut).

**Statistiques de transmission :**

- **Patches** (type 01H) : 134 bytes packés → 268 nibbles + header (5 bytes) + checksum (1 byte) + EOX (1 byte) = **275 bytes totaux transmis**
- **Master parameters** (type 03H) : 172 bytes packés → 344 nibbles + header (5 bytes) + checksum (1 byte) + EOX (1 byte) = **351 bytes totaux transmis**

**Référence :** Oberheim Matrix-1000 - MIDI & SysEx Implementation, section "SysEx Data Format".

#### Requêtes de données (SysEx 04H)

Le plugin n'implémente pas la fonction "Request All Bank" dans cette version (v1.0). Les requêtes de données supportées sont :

- **Type 3** : Requête des master parameters uniquement
- **Type 4** : Requête de l'edit buffer (patch courant)

**Note :** Les réponses de type 0 (Request All Bank) incluraient 50 "split patches" de compatibilité Matrix-6 qui doivent être ignorés.

**Référence :** Oberheim Matrix-1000 - MIDI & SysEx Implementation, section "04H - Request Data".

#### Auto-détection du synthé au démarrage

Au démarrage du plugin, ou lors de la sélection d'un port MIDI, le plugin exécute automatiquement une procédure d'auto-détection :

1. Envoyer une requête d'identification au périphérique MIDI connecté
2. Attendre la réponse du périphérique (timeout 2 secondes)
3. Vérifier que le périphérique est un Oberheim Matrix-1000
4. Si validation OK :
   - Afficher : `"Matrix-1000 detected. Version: X.XX"`
   - Activer tous les contrôles de synthé
   - Procéder au FETCH initial du patch courant
   - Procéder au FETCH initial des master parameters
5. Si validation échouée :
   - Afficher : `"Connected device is not a Matrix-1000. Please check your connection."`
   - Désactiver les contrôles de synthé
   - Aucun message SysEx ne sera envoyé

**Cas d'utilisation :**

- **UC-11** : Auto-détection du synthé au démarrage

#### Gestion des déconnexions

| Cas                               | Comportement                                                                                                                                                  |
| --------------------------------- | ------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| Port d'entrée MIDI indisponible   | Message affiché : `"MIDI device disconnected or not responding"`                                                                                              |
| Port de sortie MIDI indisponible  | Message affiché : `"MIDI device disconnected"`                                                                                                                |
| Synthé ne répond pas (timeout 2s) | Message affiché : `"Matrix-1000 not responding. Check connection and try again."`<br/>Restauration de la valeur précédente (cas de l'édition du Patch Number) |
| Reconnexion détectée              | Message affiché : `"MIDI device / Matrix-1000 reconnected."`<br/>Réexécution automatique de la synchronisation initiale                                       |

**Contraintes :**

- L'édition locale reste possible même en cas de déconnexion
- Pas de nouvel essai automatique en cas d'échec
- Pas de blocage de l'interface utilisateur

**Cas d'utilisation :**

- **UC-07** : Gestion des déconnexions MIDI

### 3.4 Validation et sérialisation

#### Fichiers .syx de patches

- **Structure :** F0 ... F7 (SysEx standard)
- **Checksum :** Validation via algorithme Oberheim spécifique
- **Validation :** Avant affichage dans la ComboBox de COMPUTER PATCHES, ou chargement du fichier
- **Fichiers invalides :** Ignorés (non affichés dans la ComboBox)
- **Message détaillé :** Exemple : `"Found 7 .syx files (5 valid, 2 invalid)"`

**Cas d'utilisation :**

- **UC-10** : Validation fichier .syx

#### Validation du checksum

Tous les messages SysEx (PATCH et MASTER) doivent être validés via le checksum Oberheim. Un checksum invalide entraîne le rejet du message avec l'affichage du message d'erreur : `"Invalid checksum. Patch not loaded."`

#### Gestion des champs de taille variable

Les paramètres de patch utilisent des champs de tailles variables (1 à 7 bits) dans une représentation binaire. Lors de la conversion entre représentation interne du plugin et format SysEx du synthé, chaque paramètre doit être sérialisé/désérialisé correctement.

**Exception importante :** Le paramètre VCF Frequency (paramètre 121) utilise un traitement différent des autres paramètres signés. Cette différence doit être respectée lors de la sérialisation/désérialisation.

#### Paramètres de patch

**Envoi d'un message SysEx :**

- Vérification des valeurs min & max avant l'envoi
- Valeurs invalides : clampées ou rejetées

**Réception d'un message SysEx :**

- Paramètres validés après réception
- Patch invalide : message d'erreur `"Invalid patch : not loaded"`

### 3.5 UI et présentation

- **Interface entièrement vectorielle :** pas d'images bitmap
- **Fenêtre redimensionnable :** zoom préconfigurés (50–400%) + coin inférieur droit étirable
- **2 thèmes proposés :** Black & Cream (Black par défaut)
- **Layout responsive :** adapté aux écrans de grande taille et/ou haute résolution

---

## 4. Validation et intégrité

### 4.1 Points critiques d'implémentation

#### Factory et descriptors (source unique de vérité)

Les Descriptors déclarent tous les paramètres synthé + widgets UI. La Factory les lit pour construire APVTS + GUI. La validation de cohérence au startup détecte les doublons d'ID paramètres et les offsets SysEx → exception lancée si problème.

#### APVTS consistency

L'APVTS doit contenir **120+ paramètres patch + 30+ paramètres master + settings**. Aucun paramètre ne doit être dupliqué ou oublié (validé au startup par la Factory).

#### Clipboard type-aware

Le presse-papiers doit mémoriser le type de module copié + signature. Validation de compatibilité avant collage (DCO1 ↔ DCO2, ENV1/2/3 ↔ ENV, LFO1 ↔ LFO2). Persistant entre patches.

**Cas d'utilisation :**

- **UC-03** : Copier/coller un module

#### Édition Patch Number double-clic

Lors de la modification du Patch Number via double-clic, le plugin envoie un Program Change au synthé. En cas de timeout, un message d'erreur est affiché et le numéro de patch précédent est restauré.

**Note :** Pas de cliquer-glisser (évite saturation buffer MIDI).

**Cas d'utilisation :**

- **UC-05** : Édition du Patch Number

### 4.2 Tests unitaires

Classes critiques à tester (JUCE Framework) :

- `SysExParser` : validation structure, checksum, parsing
- `PatchModel` : cohérence paramètres
- `MidiManager` : timeouts, reconnexions
- `ClipboardManager` : compatibilité types
- `PluginParameterFactory` : validation cohérence 1:1

---

## 5. Référence des messages SysEx

Le tableau ci-dessous récapitule tous les messages SysEx utilisés par le plugin Matrix-Control :

| Opcode       | Nom                            | Direction       | Usage                                   | Format                                 |
| ------------ | ------------------------------ | --------------- | --------------------------------------- | -------------------------------------- |
| 01H          | Single Patch Data              | Plugin → Synthé | Charger/stocker un patch en mémoire     | F0H 10H 06H 01H NN DATA CS F7H         |
| 03H          | Master Parameter Data          | Plugin ↔ Synthé | Envoyer/recevoir les paramètres master  | F0H 10H 06H 03H VER DATA CS F7H        |
| 04H (type 3) | Request Data - Master          | Plugin → Synthé | Requête master parameters               | F0H 10H 06H 04H 03H 00H F7H            |
| 04H (type 4) | Request Data - Edit Buffer     | Plugin → Synthé | Requête du patch courant                | F0H 10H 06H 04H 04H 00H F7H            |
| 06H          | Remote Parameter Edit          | Plugin → Synthé | Édition d'un paramètre patch individuel | F0H 10H 06H 06H PP VV F7H              |
| 0AH          | Set Bank                       | Plugin → Synthé | Sélection et verrouillage de banque     | F0H 10H 06H 0AH BB F7H                 |
| 0BH          | Remote Parameter Edit (Matrix) | Plugin → Synthé | Édition d'un bus de matrice             | F0H 10H 06H 0BH BB SS AM DD F7H        |
| 0CH          | Unlock Bank                    | Plugin → Synthé | Déverrouillage de banque                | F0H 10H 06H 0CH F7H                    |
| 0DH          | Patch to Edit Buffer           | Plugin → Synthé | Charger un patch en buffer d'édition    | F0H 10H 06H 0DH DATA CS F7H            |
| 0EH          | Store Edit Buffer              | Plugin → Synthé | Sauvegarder edit buffer en mémoire      | F0H 10H 06H 0EH NN BB ID F7H           |
| -            | Device Inquiry                 | Plugin → Synthé | Auto-détection synthé                   | F0H 7EH CC 06H 01H F7H                 |
| -            | Device ID                      | Synthé → Plugin | Réponse identification                  | F0H 7EH CC 06H 02H MFG FAM MEM REV F7H |

**Légende :**

- NN = Numéro du patch (0-99)
- VER = Version (03H pour Matrix-1000)
- DATA = Données du patch/master
- CS = Checksum
- BB = Numéro de banque (0-9)
- PP = Numéro du paramètre
- VV = Valeur du paramètre
- SS = Source (matrice)
- AM = Amount (matrice)
- DD = Destination (matrice)
- CC = Channel MIDI
- MFG = Manufacturer ID
- REV = Révision firmware

**Référence :** Oberheim Matrix-1000 MIDI & SysEx Implementation

---

## 6. Scope v1.0

### 6.1 Must-have (obligatoire)

- ✅ Mode PATCH complet édition temps réel
- ✅ Mode MASTER simplifié (sans GROUPS, sans RPN/NRPN)
- ✅ Charger patch synthé (PC + SysEx)
- ✅ Éditer et envoyer SysEx au synthé
- ✅ Sauvegarder patches mémoire synthé + fichiers .syx
- ✅ Navigation circulaire (verrouillé/déverrouillé)
- ✅ Clipboard intelligent inter-modules
- ✅ Fetch-Update-Send MASTER
- ✅ Auto-détection Matrix-1000 au démarrage
- ✅ Synchronisation initiale (fetch patch + master)
- ✅ Settings persistants (thème, ports MIDI, INIT.syx path)
- ✅ Barre de messages (info/succès/avertissement/erreur)
- ✅ Communication MIDI bidirectionnelle
- ✅ Gestion déconnexions gracieuse + reconnexion automatique
- ✅ UI vectorielle, resizable (zoom), themable (Black/Cream)
- ✅ Cross-platform (macOS + Windows)

### 6.2 Nice-to-have (v1.1+)

- 💡 Heartbeat/ping synthé (détection proactive continu)
- 💡 Gestion GROUPS (Mode MASTER avancé)
- 💡 Support Matrix-6/6R
- 💡 Undo/redo patches
- 💡 Générateur aléatoire patches
- 💡 Macros (1 contrôle = N paramètres)
- 💡 Tags/ratings patches
- 💡 Émulation virtuelle Matrix-1000
- 💡 Internationalization (i18n)

---

Copyright © 2025 Ten Square Software. Tous droits réservés.
