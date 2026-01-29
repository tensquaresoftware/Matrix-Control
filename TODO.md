# Matrix-Control / TODO

---

- **Auteur:** Guillaume DUPONT
- **Organisation:** Ten Square Software
- **Date de révision:** 2026-01-29

---

## Général

- Faire appliquer mes cursorrules en profondeur pour améliorer le code
- Gérer la lourdeur toujours persistante du code au lancement du plugin



## Core

- Visualiser l'APVTS
- Retester le MIDI entrant
- Relire code SysExEncoder, SysExDecoder, SysExValidator
- Constuire PatchModel (ou PatchDataModel) & MasterModel (ou MasterDataModel)
- Redondance PatchModel / MasterModel avec APVTS, ou pas ?
- Construire Clipboard Type Aware (DCO, ENV, LFO)



## GUI

- Fond des widgets et panels transparents pour faire apparaître naturellement la couleur de fond de la GUI ?
- Supprimer les couleurs inutiles (Background) dans ThemeColours.h
- Simplifiier ThemeColours : kCommonBackgroundBlack, kCommonBackgroundCream
- Dco1Panel a été refactorisé avec BaseModulePanel, mais pas les autres modules !
- Créer ComboBox `MIDI Input` et `MIDI Output`
- Créer classe Led, ajouter leds pour message MIDI entrants / sortants
- Section Patch Manager : imaginer nouveau module `Patch Mutation`
- Gérer le zoom avec AffineTransformation
- Améliorer les couleurs du thème Cream
- Prévoir un thème User et un éditeur de thème ?
- Supprimer la dépendance Widgets / Theme : Créer une interface ITheme (à faire plus tard)



---

Copyright © 2026 Ten Square Software. Tous droits réservés.