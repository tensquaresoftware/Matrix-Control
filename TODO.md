# Matrix-Control / TODO

---

- **Auteur:** Guillaume DUPONT
- **Organisation:** Ten Square Software
- **Date de révision:** 2026-01-30

---

## Général

- GitHub : Fais un commit et crée un nouveau tag GitHub, dans la continuité des tags existants.
- Faire appliquer mes cursorrules en profondeur pour améliorer le code
- Gérer la lourdeur toujours persistante du code au lancement du plugin
- Télécharger le code source de Vital Audio et le faire analyser par l'IA



## GUI

- Dans HeaderPanel, créer un label "THEME:" puis boutons BLACK & CREAM, puis label "ZOOM:" puis ComboBox avec facteurs de zoom prédéfinis
- Gérer le facteur de zoom avec AffineTransformation et un zoom par défaut sauvé dans l'APVTS
- Créer ComboBox `MIDI Input` et `MIDI Output`
- Créer classe Led, ajouter leds pour message MIDI entrants / sortants
- Section Patch Manager : imaginer nouveau module `Patch Mutation`
- Améliorer les couleurs du thème Cream
- Prévoir un thème User et un éditeur de thème ?
- Supprimer la dépendance Widgets / Theme : Créer une interface ITheme (à faire plus tard)



## Core

- Visualiser l'APVTS
- Retester le MIDI entrant
- Relire code SysExEncoder, SysExDecoder, SysExValidator
- Constuire PatchModel (ou PatchDataModel) & MasterModel (ou MasterDataModel)
- Redondance PatchModel / MasterModel avec APVTS, ou pas ?
- Construire Clipboard Type Aware (DCO, ENV, LFO)

---

Copyright © 2026 Ten Square Software. Tous droits réservés.