# Matrix-Control

## Cas d'utilisation

**Auteur :** Guillaume DUPONT  
**Organisation :** Ten Square Software  
**Date de modification :** 2025-11-18  
**Version :** 1.0.0

---

## UC-01 : Charger un patch du synthétiseur

**Acteur :** Utilisateur  
**Objectif :** Charger un patch stocké dans le Matrix-1000  
**Déclencheur :** Clic sur bouton `<` ou `>` (navigation)

### Préconditions

- Le plugin est connecté au synthé via MIDI
- Au moins un patch existe dans la mémoire du synthé

### Scénario nominal

1. L'utilisateur clique sur le bouton `>`
2. Le plugin calcule le numéro du patch suivant (navigation circulaire)
3. Le plugin envoie un Program Change #NN au synthé
4. Le synthé charge le patch en mémoire
5. Le synthé renvoie le patch complet via SysEx
6. Le plugin valide le SysEx (structure + checksum)
7. Le plugin parse les 126 paramètres du patch
8. Le plugin met à jour la GUI (96 params PATCH + 30 params MATRIX MODULATION)
9. La barre de message affiche : `"Patch loaded: [NOM_PATCH]"`

### Scénarios alternatifs

**2a. Banque verrouillée (Bank Lock activé)**

- 2a.1. Navigation circulaire uniquement dans les 100 patches de la banque courante
- 2a.2. Arrivé au patch 99, retour au patch 00 de la même banque

**2b. Banque déverrouillée**

- 2b.1. Navigation circulaire sur les 1000 patches (10 banques)
- 2b.2. Arrivé au patch 9-99, retour au patch 0-00

**6a. SysEx invalide (checksum incorrect)**

- 6a.1. Le plugin rejette le message
- 6a.2. La barre de message affiche : `"Invalid patch received"`
- 6a.3. Le patch précédent reste affiché

**5a. Timeout MIDI (2 secondes)**

- 5a.1. Le synthé ne répond pas
- 5a.2. La barre de message affiche : `"Matrix-1000 not responding. Check connection and try again."`
- 5a.3. Le numéro de patch n'est pas modifié

### Postconditions

- Le patch NN est chargé dans le plugin
- La GUI affiche les 126 paramètres correctement
- Le plugin et le synthé sont synchronisés

---

## UC-02 : Initialiser un paramètre individuel

**Acteur :** Utilisateur  
**Objectif :** Réinitialiser un paramètre à sa valeur INIT  
**Déclencheur :** Double-clic sur un contrôle de paramètre

### Préconditions

- Le plugin est lancé
- Un patch est actuellement édité

### Scénario nominal

1. L'utilisateur double-clique sur le slider "DCO1 Freq"
2. Le plugin vérifie la présence du fichier `~/Music/Matrix-Control/INIT.syx`
3. Le plugin charge la valeur correspondante depuis INIT.syx
4. Le plugin met à jour l'afficheur local du paramètre
5. Le plugin envoie le message SysEx correspondant au synthé
6. Le synthé applique la nouvelle valeur
7. La barre de message affiche : `"Parameter initialized"`

### Scénarios alternatifs

**2a. Fichier INIT.syx absent ou invalide**

- 2a.1. Le plugin utilise la valeur par défaut interne
- 2a.2. La barre de message affiche : `"No valid INIT.syx file found > Using plugin default values"`
- Retour à l'étape 4

**6a. Synthé ne répond pas (timeout 2s)**

- 6a.1. La barre de message affiche : `"Matrix-1000 not responding. Check connection and try again."`
- 6a.2. La valeur locale reste modifiée (pas de rollback)

### Postconditions

- Le paramètre est réinitialisé à sa valeur INIT
- Le plugin et le synthé sont synchronisés (ou message d'erreur affiché)

---

## UC-03 : Copier/coller un module

**Acteur :** Utilisateur  
**Objectif :** Dupliquer les paramètres d'un module vers un autre module compatible  
**Déclencheur :** Clic sur bouton `C` (Copy) puis `P` (Paste)

### Préconditions

- Le plugin est lancé
- Un patch est actuellement édité

### Scénario nominal

1. L'utilisateur clique sur le bouton `C` du module DCO1
2. Le plugin copie les 10 paramètres du module DCO1 dans le presse-papiers
3. Le plugin mémorise le type du module copié (`DCO`)
4. L'utilisateur clique sur le bouton `P` du module DCO2
5. Le plugin vérifie la compatibilité (`DCO` ↔ `DCO` = OK)
6. Le plugin colle les paramètres communs dans DCO2
7. Le plugin envoie les messages SysEx correspondants au synthé (délai de 10 ms entre chaque)
8. Le synthé applique les nouvelles valeurs
9. La barre de message affiche : `"Module pasted successfully"`

### Scénarios alternatifs

**5a. Types incompatibles (ex: DCO → LFO)**

- 5a.1. Le bouton `P` reste grisé
- 5a.2. Aucune action possible

**5b. Presse-papiers vide**

- 5b.1. Le bouton `P` reste grisé
- 5b.2. Aucune action possible

**8a. Synthé ne répond pas pendant l'envoi séquentiel**

- 8a.1. La barre de message affiche : `"MIDI device not responding"`
- 8a.2. Les valeurs locales restent modifiées

### Postconditions

- Le module cible contient les paramètres du module source
- Le presse-papiers conserve sa copie (persistant entre patches)

---

## UC-04 : Naviguer dans les patches du synthétiseur

**Acteur :** Utilisateur  
**Objectif :** Parcourir les patches internes du synthé selon le mode de verrouillage  
**Déclencheur :** Clic sur bouton `<` ou `>` du module INTERNAL PATCHES

### Préconditions

- Le plugin est connecté au synthé
- Au moins une banque contient des patches

### Scénario nominal (banque déverrouillée)

1. L'utilisateur clique sur le bouton `>`
2. Le plugin détecte que la banque courante est déverrouillée
3. Le plugin calcule le patch suivant en navigation circulaire globale (0-00 → 9-99)
4. Le plugin envoie le Program Change correspondant
5. Le synthé renvoie le patch via SysEx
6. Le plugin met à jour la GUI
7. La barre de message affiche : `"Patch loaded: [NOM]"`

### Scénarios alternatifs

**2a. Banque verrouillée (Bank Lock activé)**

- 2a.1. Navigation circulaire uniquement dans la banque courante (ex: 3-00 → 3-99)
- 2a.2. Arrivé au dernier patch de la banque, retour au premier patch de cette banque

**2b. Passage de 9-99 à 0-00 (banque déverrouillée)**

- 2b.1. Navigation circulaire globale continue
- 2b.2. Aucune interruption

**5a. Timeout MIDI**

- 5a.1. La barre de message affiche : `"Matrix-1000 not responding. Check connection and try again."`
- 5a.2. Le numéro de patch affiché ne change pas

### Postconditions

- Le patch suivant/précédent est chargé
- La navigation respecte le mode de verrouillage

---

## UC-05 : Éditer le numéro de patch par double-clic

**Acteur :** Utilisateur  
**Objectif :** Charger un patch spécifique en saisissant son numéro directement  
**Déclencheur :** Double-clic sur l'afficheur `Patch Number`

### Préconditions

- Le plugin est connecté au synthé
- L'afficheur `Patch Number` est visible

### Scénario nominal

1. L'utilisateur double-clique sur l'afficheur `Patch Number`
2. L'afficheur devient éditable (champ de saisie)
3. L'utilisateur saisit `42` puis valide (touche Entrée)
4. Le plugin vérifie que 42 est dans la plage [00-99]
5. Le plugin envoie le Program Change #42 au synthé
6. Le synthé charge le patch #42 en mémoire
7. Le synthé renvoie le patch complet via SysEx
8. Le plugin valide et parse le SysEx
9. Le plugin met à jour la GUI
10. La barre de message affiche : `"Patch loaded: [NOM]"`

### Scénarios alternatifs

**4a. Numéro invalide (ex: 105, -3, "abc")**

- 4a.1. Le plugin rejette la saisie
- 4a.2. Le numéro précédent est restauré dans l'afficheur
- 4a.3. La barre de message affiche : `"Patch number is invalid : it must be in the range 00–99."`

**7a. Timeout MIDI (synthé ne répond pas)**

- 7a.1. Timeout de 2 secondes dépassé
- 7a.2. Le numéro précédent est restauré dans l'afficheur
- 7a.3. La barre de message affiche : `"Matrix-1000 not responding. Check connection and try again."`

**8a. SysEx invalide (checksum incorrect)**

- 8a.1. Le plugin rejette le message
- 8a.2. Le numéro précédent est restauré
- 8a.3. La barre de message affiche : `"Invalid patch received"`

### Postconditions

- Le patch NN est chargé et affiché (ou message d'erreur)
- L'afficheur redevient non-éditable

---

## UC-06 : Éditer un paramètre MASTER

**Acteur :** Utilisateur  
**Objectif :** Modifier un paramètre global du synthé (ex: Master Tune)  
**Déclencheur :** Modification d'un contrôle de la section MASTER EDIT

### Préconditions

- Le plugin a effectué un FETCH du MASTER au démarrage
- Le MasterModel local contient les 30+ paramètres à jour

### Scénario nominal

1. L'utilisateur modifie le slider "Master Tune" à +12
2. Le plugin met à jour la valeur correspondante dans le MasterModel local
3. Le plugin reconstruit le message SysEx MASTER complet (30+ paramètres)
4. Le plugin envoie le SysEx MASTER au synthé
5. Le synthé applique les nouveaux paramètres globaux
6. La barre de message affiche : `"Master parameter updated"`

### Scénarios alternatifs

**4a. Timeout MIDI (synthé ne répond pas)**

- 4a.1. Le synthé ne répond pas dans les 2 secondes
- 4a.2. La barre de message affiche : `"Matrix-1000 not responding. Check connection and try again."`
- 4a.3. La valeur locale reste modifiée (édition optimiste, pas de rollback)

**1a. L'utilisateur modifie rapidement plusieurs paramètres MASTER**

- 1a.1. Chaque modification déclenche UPDATE local + SEND complet
- 1a.2. Délai de 10 ms entre chaque envoi SysEx (throttling si nécessaire)

### Postconditions

- Le MasterModel local est à jour
- Le synthé a reçu le MASTER complet
- Pas de nouveau FETCH nécessaire (le plugin est la source de vérité)

---

## UC-07 : Gérer une déconnexion MIDI

**Acteur :** Système  
**Objectif :** Détecter et gérer une déconnexion du périphérique MIDI  
**Déclencheur :** Perte de connexion avec le Matrix-1000

### Préconditions

- Le plugin était connecté au synthé
- Une communication MIDI était active

### Scénario nominal (déconnexion détectée)

1. Le plugin tente d'envoyer un message SysEx
2. Le port de sortie MIDI ne répond pas (timeout de 2 secondes)
3. Le plugin détecte la déconnexion
4. La barre de message affiche : `"MIDI device disconnected"`
5. Le plugin entre en mode "édition locale uniquement"
6. L'utilisateur peut continuer à éditer les paramètres localement
7. Aucun message SysEx n'est envoyé tant que la connexion n'est pas rétablie

### Scénarios alternatifs

**1a. Déconnexion pendant la réception d'un SysEx**

- 1a.1. Le port d'entrée MIDI ne répond plus
- 1a.2. Timeout de 2 secondes
- 1a.3. La barre de message affiche : `"MIDI device disconnected or not responding"`
- Retour à l'étape 5

**8. Reconnexion détectée**

- 8.1. L'utilisateur reconnecte le Matrix-1000
- 8.2. Le plugin détecte la reconnexion (lors du prochain envoi SysEx)
- 8.3. La barre de message affiche : `"MIDI device / Matrix-1000 reconnected."`
- 8.4. Les communications MIDI reprennent normalement

### Postconditions

- Le plugin gère gracieusement la déconnexion
- L'interface reste utilisable en mode local
- Aucun blocage de l'UI

---

## UC-08 : Workflow SAVE AS → SAVE

**Acteur :** Utilisateur  
**Objectif :** Sauvegarder rapidement un patch après un premier SAVE AS  
**Déclencheur :** Clic sur bouton `SAVE AS` puis `SAVE`

### Préconditions

- Le plugin est lancé
- Un patch est actuellement édité

### Scénario nominal

1. L'utilisateur clique sur le bouton `SAVE AS`
2. Une boîte de dialogue système s'ouvre
3. L'utilisateur sélectionne le dossier `/Music/MyPatches/`
4. L'utilisateur saisit le nom de fichier `MyBrass.syx`
5. Le plugin sauvegarde le patch courant en fichier .syx
6. Le plugin mémorise le chemin complet `/Music/MyPatches/MyBrass.syx` dans les Préférences
7. Le bouton `SAVE` devient actif
8. La barre de message affiche : `"Patch saved to disk: /Music/MyPatches/MyBrass.syx"`
9. L'utilisateur modifie ensuite le patch
10. L'utilisateur clique sur le bouton `SAVE`
11. Le plugin écrase directement `/Music/MyPatches/MyBrass.syx` sans demande de confirmation
12. La barre de message affiche : `"Patch saved to disk: /Music/MyPatches/MyBrass.syx"`

### Scénarios alternatifs

**4a. L'utilisateur annule la boîte de dialogue**

- 4a.1. Aucun fichier n'est sauvegardé
- 4a.2. Le bouton `SAVE` reste grisé
- 4a.3. Aucun message n'est affiché

**4b. Le fichier existe déjà**

- 4b.1. La boîte de dialogue système demande confirmation d'écrasement
- 4b.2. Si l'utilisateur confirme, retour à l'étape 5
- 4b.3. Si l'utilisateur annule, retour à l'étape 4

**10a. L'utilisateur clique sur SAVE AS à nouveau**

- 10a.1. Une nouvelle boîte de dialogue s'ouvre
- 10a.2. L'utilisateur sélectionne `/Music/NewPatches/`
- 10a.3. Saisie de `BetterBrass.syx`
- 10a.4. Le nouveau chemin `/Music/NewPatches/BetterBrass.syx` remplace l'ancien dans les Préférences
- 10a.5. Les prochains `SAVE` écraseront `/Music/NewPatches/BetterBrass.syx`

### Postconditions

- Le patch est sauvegardé sur disque
- Le chemin complet du dernier fichier sauvegardé est mémorisé
- Le bouton `SAVE` permet une sauvegarde rapide ultérieure

---

## UC-09 : Contrainte LEGATO PORTA / KEYBOARD MODE

**Acteur :** Utilisateur  
**Objectif :** Éditer le paramètre LEGATO PORTA en respectant les contraintes métier  
**Déclencheur :** Modification de KEYBOARD MODE ou LEGATO PORTA

### Préconditions

- Le plugin est lancé
- Un patch est en cours d'édition

### Scénario nominal

1. L'utilisateur sélectionne `KEYBOARD MODE` = `UNISON`
2. Le plugin active le contrôle `LEGATO PORTA` (n'est plus grisé)
3. L'utilisateur active `LEGATO PORTA` = `ON`
4. Le plugin envoie le SysEx correspondant
5. Le synthé applique la configuration

### Scénarios alternatifs

**1a. L'utilisateur sélectionne KEYBOARD MODE ≠ UNISON**

- 1a.1. Le plugin grise automatiquement le contrôle `LEGATO PORTA`
- 1a.2. `LEGATO PORTA` n'est pas modifiable

**3a. LEGATO PORTA = ON et l'utilisateur change KEYBOARD MODE de UNISON vers une autre valeur**

- 3a.1. Le plugin détecte l'incohérence
- 3a.2. Le plugin force `LEGATO PORTA` = `OFF`
- 3a.3. Le plugin envoie le SysEx correspondant pour LEGATO PORTA
- 3a.4. La barre de message affiche : `"LEGATO PORTA can only be activated if KEYBOARD MODE = UNISON"`
- 3a.5. Le contrôle `LEGATO PORTA` devient grisé

### Postconditions

- La contrainte métier est respectée
- L'utilisateur est informé en cas de modification automatique

---

## UC-10 : Valider un fichier .syx avant chargement

**Acteur :** Utilisateur  
**Objectif :** S'assurer qu'un fichier .syx est valide avant de le charger  
**Déclencheur :** Clic sur bouton `OPEN` du module COMPUTER PATCHES

### Préconditions

- Le plugin est lancé
- L'utilisateur a des fichiers .syx sur son ordinateur

### Scénario nominal

1. L'utilisateur clique sur le bouton `OPEN`
2. Une boîte de dialogue système s'ouvre
3. L'utilisateur sélectionne le dossier `/Music/SynthPatches/`
4. Le plugin scanne tous les fichiers .syx du dossier
5. Le plugin valide chaque fichier (structure F0...F7 + checksum Oberheim)
6. Le plugin trouve 7 fichiers : 5 valides, 2 invalides
7. Le plugin trie les 5 fichiers valides par ordre alphabétique
8. Le plugin charge le premier patch valide de la liste
9. Le plugin met à jour la GUI avec les paramètres du patch
10. La ComboBox `Patch File` affiche la liste des 5 patches valides uniquement
11. La barre de message affiche : `"Found 7 .syx files (5 valid, 2 invalid)"`

### Scénarios alternatifs

**6a. Aucun fichier valide trouvé**

- 6a.1. La ComboBox reste grisée
- 6a.2. La ComboBox affiche `"No Patch !"`
- 6a.3. Les boutons `<` et `>` restent grisés
- 6a.4. La barre de message affiche : `"Found 3 .syx files (0 valid, 3 invalid)"`

**6b. Tous les fichiers sont valides**

- 6b.1. Tous les fichiers sont affichés dans la ComboBox
- 6b.2. La barre de message affiche : `"Found 10 .syx files (10 valid, 0 invalid)"`

**5a. Fichier corrompu détecté**

- 5a.1. Structure SysEx invalide (pas F0...F7)
- 5a.2. Le fichier est ignoré (non affiché dans la ComboBox)
- Retour à l'étape 6

**5b. Checksum incorrect**

- 5b.1. Algorithme Oberheim détecte une erreur
- 5b.2. Le fichier est ignoré
- Retour à l'étape 6

### Postconditions

- Seuls les fichiers .syx valides sont affichés et chargés
- L'utilisateur est informé du résultat du scan

---

## UC-11 : Auto-détection du synthé au démarrage

**Acteur :** Système (automatique)  
**Objectif :** Vérifier que l'appareil MIDI connecté est un Oberheim Matrix-1000  
**Déclencheur :** Démarrage du plugin ou changement de port MIDI sélectionné

### Préconditions

- Le plugin est lancé
- Un port MIDI de sortie est sélectionné

### Scénario nominal

1. Le plugin en démarrage exécute une auto-détection du synthé
2. Le plugin envoie une requête Device Inquiry au port MIDI sélectionné
3. Le plugin attend la réponse Device ID du synthé (timeout 2 secondes)
4. Le plugin analyse la réponse reçue et vérifie les critères d'identification :
   - Manufacturer ID = 10H (Oberheim)
   - Family = 06H (Matrix-6/6R/1000)
   - Member = 02H (Matrix-1000)
5. Tous les critères sont validés ✓
6. Le plugin affiche : `"Matrix-1000 detected. Version: X.XX"`
7. Le plugin active tous les contrôles de synthé (sliders, boutons, etc.)
8. Le plugin procède au FETCH initial du patch courant (request type 4)
9. Le plugin procède au FETCH initial des master parameters (request type 3)
10. Le barre de message affiche : `"Synth connected and synchronized"`

### Scénarios alternatifs

**2a. Aucun port MIDI sélectionné**

- 2a.1. Le plugin ne peut pas exécuter l'auto-détection
- 2a.2. Le plugin affiche : `"No MIDI output port selected"`
- 2a.3. Les contrôles de synthé restent grisés

**3a. Timeout MIDI (synthé ne répond pas dans les 2 secondes)**

- 3a.1. La requête Device Inquiry reste sans réponse
- 3a.2. Le plugin affiche : `"Connected device not responding. Is it a Matrix-1000?"`
- 3a.3. Les contrôles de synthé restent grisés
- 3a.4. L'utilisateur peut réessayer en changeant de port

**4a. Manufacturer ID incorrect**

- 4a.1. Exemple : le périphérique répondu avec Manufacturer ID = FFH
- 4a.2. Le plugin affiche : `"Connected device is not a Matrix-1000. Please check your connection."`
- 4a.3. Les contrôles de synthé restent grisés

**4b. Family ou Member incorrect**

- 4b.1. Exemple : c'est un Matrix-6 ou un Matrix-6R (pas un Matrix-1000)
- 4b.2. Le plugin affiche : `"Connected device is not a Matrix-1000. Please check your connection."`
- 4b.3. Les contrôles de synthé restent grisés

**8a. Timeout lors du FETCH du patch courant**

- 8a.1. Le synthé ne répond pas au request type 4
- 8a.2. Le plugin affiche : `"Failed to fetch current patch from synth"`
- 8a.3. Les contrôles restent grisés
- 8a.4. L'utilisateur peut réessayer manuellement

**9a. Timeout lors du FETCH des master parameters**

- 9a.1. Le synthé ne répond pas au request type 3
- 9a.2. Le plugin affiche : `"Failed to fetch master parameters from synth"`
- 9a.3. Les contrôles restent grisés
- 9a.4. L'utilisateur peut réessayer manuellement

### Postconditions

- Le synthé connecté est confirmé comme Matrix-1000 (ou message d'erreur)
- Les contrôles de synthé sont activés/désactivés en conséquence
- Les paramètres du patch courant et des master sont synchronisés (ou non)
- Le plugin est prêt pour l'édition (cas nominal) ou attend action utilisateur (cas erreur)

---

Copyright © 2025 Ten Square Software. Tous droits réservés.
