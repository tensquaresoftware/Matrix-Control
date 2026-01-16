# Matrix-Control | TODO

---

- **Auteur:** Guillaume DUPONT
- **Organisation:** Ten Square Software
- **Date de révision:** 2026-01-16

---

## Général

Implémenter une configuration CMake pour ne plus avoir à gérer de chemins relatifs dans les sources :

Voici une conversation que j'ai eu avec Claude IA au sujet des chemins relatifs de mon projet qui commencent à être fastidieux à gérer dans les source :

Guillaume : Dans mon projet JUCE en cours de développement avec l'IDE Cursor, j'ai  maintenant un grand nombre de fichiers source avec une arborescence de  fichiers assez élaborée. Les chemins relatifs utilisés dans les #include comment à être un peu pénibles à écrire, avec des "../" à plusieurs  niveaux. Existe-t-il une solution élégante et robuste avec CMake, pour éviter ces  noms de chemins à rallonge dans les sources ? Peut-être l'utilisation  d'alias plus courts, avec les chemins renseignés une fois quelque part,  dans un fichier central ?

Claude IA : Avec CMake, vous avez plusieurs solutions élégantes pour gérer vos chemins d'inclusion. Si vous structurez bien votre projet, vous pouvez n'ajouter que `Source/` :

target_include_directories(YourPlugin
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/Source
)

## GUI

- Supprimer la dépendance Widgets / Theme : Créer une interface ITheme (à faire plus tard)
- Faire relire et valider mes cursorrules à l'IA
- Faire appliquer profondément mes cursorrules pour améliorer le code
- Gérer la lourdeur toujours persistante du code au lancement du plugin
- Dans PluginDimensions, renommer :
  - namespace MasterEditSection en MasterEditPanel
  - namespace PatchEditSection en PatchEditPanel
  - namespace MatrixModulationSection en MatrixModulationPanel
  - namespace PatchManagerSection en PatchManagerPanel

- Implémenter dans PluginDimensions les dimensions des :
  - TopPanel, MiddlePanel et Bottom de PatchEdit
  - Modules de PatchEdit et MasterEdit (factorisées)
  - MatrixModulationPanel
  - BankUtility, InternalPatches et ComputerPatches de PatchManager
  - Autres ?

- 

---

Copyright © 2026 Ten Square Software. Tous droits réservés.