# Matrix-Control / Classes

---

## Classes Core - Communication MIDI/SysEx

### 1. **`SysExMessageBuilder`**

Responsabilité : construire les messages SysEx spécifiques à chaque modèle de synthé (Matrix-1000, Matrix-6/6R).

### 2. **`SysExMessageParser`**

Responsabilité : décoder et valider les messages SysEx reçus du synthé (checksum, structure, intégrité).

### 3. **`MidiMessageSender`**

Responsabilité : envoyer des messages MIDI (SysEx, Program Change) via le port MIDI de sortie sélectionné.

### 4. **`MidiMessageReceiver`**

Responsabilité : écouter et recevoir les messages MIDI entrants (SysEx de patch, confirmation du synthé).

---

## Classes Métier - Gestion du Patch

### 5. **`PatchModel`**

Responsabilité : modèle de données représentant l'état complet d'un patch (tous les paramètres du synthé). Doit être serializable/deserializable.

### 6. **`PatchValidator`**

Responsabilité : valider la cohérence et les plages de valeurs des paramètres d'un patch avant envoi ou sauvegarde.

### 7. **`PatchConverter`**

Responsabilité : convertir un `PatchModel` en message SysEx (et inversement), en tenant compte du format spécifique du synthé.

---

## Classes de Persistance - Fichiers .syx

### 8. **`SysExFileReader`**

Responsabilité : lire un fichier .syx et extraire le ou les patches qu'il contient.

### 9. **`SysExFileWriter`**

Responsabilité : écrire un `PatchModel` dans un fichier .syx compatible avec SysEx Librarian.

### 10. **`PatchLibraryScanner`**

Responsabilité : scanner un dossier de l'ordinateur et lister tous les fichiers .syx disponibles (avec métadonnées : nom, date, etc.).

---

## Classes de Gestion - Banques et Mémoires

### 11. **`BankManager`**

Responsabilité : gérer le changement de banque sur le Matrix-1000 (envoi du SysEx de sélection de banque, verrouillage/déverrouillage).

### 12. **`ProgramChangeHandler`**

Responsabilité : gérer l'envoi de Program Change et attendre/traiter la réponse SysEx du synthé.

### 13. **`PatchMemoryManager`**

Responsabilité : sauvegarder le patch en cours d'édition dans une mémoire spécifique du synthé (envoi du SysEx de sauvegarde).

---

## Classes Utilitaires - Mapping et Synchronisation

### 14. **`ParameterMapper`**

Responsabilité : mapper les valeurs normalisées de l'APVTS (0.0–1.0) vers les valeurs brutes du synthé (ex : 0–63, 0–127) et vice-versa.

### 15. **`SynthModelDetector`**

Responsabilité : détecter automatiquement (ou configurer manuellement) le modèle de synthé connecté (Matrix-1000, Matrix-6, Matrix-6R) pour adapter les messages SysEx.

### 16. **`PatchSynchronizer`**

Responsabilité : synchroniser l'état du `PatchModel` avec l'APVTS (bidirectionnel : GUI → modèle et modèle → GUI).

---

## Classes de Contrôle - Navigation dans les Patches

### 17. **`PatchNavigator`**

Responsabilité : gérer la navigation dans une liste de patches (Next, Previous, Jump to Index) et notifier le système de charger le patch sélectionné.

### 18. **`PatchLibraryModel`**

Responsabilité : modèle de données représentant la liste des patches disponibles dans le dossier sélectionné (paths, noms, index courant).

---

## Classes de Validation et Logging

### 19. **`SysExChecksumValidator`**

Responsabilité : calculer et valider les checksums des messages SysEx Oberheim (algorithme spécifique).

### 20. **`MidiLogger`** *(optionnel mais recommandé)*

Responsabilité : logger tous les messages MIDI envoyés/reçus pour debug et traçabilité (avec timestamps).

---

## Recommandations d'Architecture

- **Injection de dépendances** : le `PluginProcessor` doit injecter les dépendances (ex : `MidiMessageSender`, `PatchModel`) dans les classes qui en ont besoin.
- **Pattern Observer** : utilise `juce::ValueTree::Listener` ou `juce::ChangeBroadcaster` pour notifier les changements de patch entre couches.
- **Thread safety** : attention à la synchronisation entre le thread audio (MIDI I/O) et le thread GUI (APVTS). Utilise `juce::MessageManager::callAsync` si nécessaire.
- **Tests unitaires** : ces petites classes sont parfaites pour être testées individuellement (mock des messages MIDI, validation des checksums, etc.).

---

## Prochaines Étapes

1. **Commence par les classes de base** : `SysExMessageBuilder`, `SysExMessageParser`, `PatchModel`, `MidiMessageSender`.
2. **Valide le protocole MIDI** : teste l'envoi/réception de SysEx avec un vrai Matrix-1000/6 ou un émulateur.
3. **Ajoute la persistance** : `SysExFileReader`/`Writer` pour gérer les fichiers .syx.
4. **Intègre l'APVTS** : `ParameterMapper` et `PatchSynchronizer`.

---

Besoin que je détaille l'interface ou l'implémentation d'une de ces classes en particulier ? 🚀
