# Matrix-Control / Plan d'Optimisation de la GUI

**Date de création** : 26 janvier 2026  
**Objectif** : Optimiser les performances de la GUI (lancement, réactivité, redimensionnement)  
**Problèmes identifiés** :
- Lancement lent du plugin
- Lag lors des interactions (sliders, boutons)
- Ralentissements lors du déplacement de la fenêtre
- ~300 widgets vectoriels sans cache
- Cascade de `repaint()` non optimisée dans `setTheme()`

---

## Vue d'ensemble - 4 Phases

```
Phase 1 : Nettoyage du design (EN COURS)
  └─> Simplifier le code de rendu des widgets
  └─> Supprimer les éléments graphiques inutiles
  └─> Durée estimée : quelques sessions

Phase 2 : Quick Win - Optimisation setTheme()
  └─> Corriger la cascade de repaint()
  └─> Impact immédiat au lancement et changement de thème
  └─> Gain attendu : 2-5x

Phase 3 : Mesure et Profiling
  └─> Utiliser Instruments / Time Profiler
  └─> Identifier les hotspots réels
  └─> Prioriser les optimisations

Phase 4 : Optimisation ciblée
  └─> Implémenter le cache d'images
  └─> Lazy loading des composants
  └─> Gain attendu : 5-20x sur le rendu
```

---

## Phase 1 : Nettoyage du Design des Widgets

### Objectif
Simplifier le code de rendu en supprimant les éléments graphiques inutiles (fonds, bordures, etc.) et clarifier la responsabilité de chaque widget.

### Approche : Revue Systématique

Pour **chaque widget**, se poser ces questions :

#### Questions de Design
1. **Quel est son rôle visuel final ?**
   - Texte seul ?
   - Fond + texte ?
   - Forme complexe ?
   - Éléments interactifs ?

2. **A-t-il besoin d'un fond opaque ?**
   - OUI → `setOpaque(true)` + `fillRect()` dans `paint()`
   - NON → `setOpaque(false)` + pas de `drawBackground()`

3. **Quelles sont les parties statiques vs dynamiques ?**
   - Statique : ne change jamais (fond, bordure, labels)
   - Dynamique : change souvent (valeur d'un slider, texte d'un input)
   - → Utile pour la Phase 4 (cache d'images)

4. **Quelles opérations de dessin sont coûteuses ?**
   - `fillPath()` avec Path complexe
   - `drawText()` avec police custom
   - Anti-aliasing intensif
   - Gradients, ombres portées

#### Actions par Widget

- [x] **Label** - Texte simple, transparent ✅ **TERMINÉ** (26/01/2026)
- [x] **GroupLabel** - Texte + lignes gauche/droite ✅ **TERMINÉ** (26/01/2026)
- [x] **Button** - Fond + bordure + texte + états (hover/down) ✅ **TERMINÉ** (26/01/2026)
- [x] **NumberBox** - Champ numérique éditable + dot ✅ **TERMINÉ** (26/01/2026)
- [x] **SectionHeader** - Texte + lignes décoratives ✅ **TERMINÉ** (26/01/2026)
- [x] **ModuleHeader** - Texte en gras + ligne horizontale ✅ **TERMINÉ** (26/01/2026)
- [x] **HorizontalSeparator** - Ligne horizontale ✅ **TERMINÉ** (26/01/2026)
- [x] **VerticalSeparator** - Ligne verticale + padding ✅ **TERMINÉ** (26/01/2026)
- [x] **Slider** - Track + texte + bordure focus ✅ **TERMINÉ** (26/01/2026)
- [ ] **ComboBox** - Fond + texte + flèche déroulante
- [ ] **PopupMenu** (pour ComboBox) - Menu déroulant stylisé
- [ ] **EnvelopeDisplay** - Courbe d'enveloppe (points + segments)
- [ ] **TrackGeneratorDisplay** - Visualisation de séquence
- [ ] **PatchNameDisplay** - Affichage du nom de patch
- [ ] **ModulationBusHeader** - En-tête de bus de modulation

---

## Phase 2 : Quick Win - Optimisation `setTheme()`

### Problème Actuel

```cpp
void MainComponent::setTheme(Theme& theme)
{
    theme_ = &theme;
    headerPanel.setTheme(theme);  // -> appelle repaint() dans headerPanel
    bodyPanel.setTheme(theme);    // -> appelle repaint() dans bodyPanel
    footerPanel.setTheme(theme);  // -> appelle repaint() dans footerPanel
    repaint();                    // -> repaint() redondant à la racine !
}
```

**Résultat** : Chaque niveau de la hiérarchie appelle `repaint()` → cascade de repaints redondants.

### Solution

**Approche "bottom-up" : repaint unique à la racine**

```cpp
// Dans TOUS les widgets/panneaux intermédiaires
void MyWidget::setTheme(Theme& theme)
{
    theme_ = &theme;
    // PAS de repaint() ici !
}

// SEULEMENT à la racine (MainComponent ou PluginEditor)
void MainComponent::setTheme(Theme& theme)
{
    theme_ = &theme;
    headerPanel.setTheme(theme);  // mise à jour sans repaint
    bodyPanel.setTheme(theme);    // mise à jour sans repaint
    footerPanel.setTheme(theme);  // mise à jour sans repaint
    repaint();                    // UN SEUL repaint à la racine
}
```

### Fichiers à Modifier

Supprimer les `repaint()` dans `setTheme()` de tous les fichiers sauf la racine :

- [ ] `Source/GUI/Widgets/Label.cpp`
- [ ] `Source/GUI/Widgets/GroupLabel.cpp`
- [ ] `Source/GUI/Widgets/Button.cpp`
- [ ] `Source/GUI/Widgets/NumberBox.cpp`
- [ ] `Source/GUI/Widgets/Slider.cpp`
- [ ] `Source/GUI/Widgets/ComboBox.cpp`
- [ ] `Source/GUI/Widgets/PopupMenu.cpp`
- [ ] `Source/GUI/Panels/.../*.cpp` (tous les panneaux intermédiaires)
- [x] Garder `repaint()` UNIQUEMENT dans `MainComponent::setTheme()`

### Gain Attendu
- **2x à 5x** plus rapide au lancement
- Changement de thème instantané

---

## Phase 3 : Mesure et Profiling avec Instruments

### Objectif
Identifier les **vraies** sources de lenteur avant d'implémenter des optimisations lourdes.

### Outils
- **Instruments** (macOS) - Time Profiler
- **Alternative** : `juce::PerformanceCounter` dans le code

### Procédure

#### 1. Compiler en mode RelWithDebInfo

```bash
cd /Volumes/Guillaume/Dev/Projects/MAO/Plugins/Matrix-Control
cmake --preset <ton-preset> -DCMAKE_BUILD_TYPE=RelWithDebInfo
cmake --build --preset <ton-preset>
```

**RelWithDebInfo** = optimisations ON + symboles de debug ON

#### 2. Lancer Instruments

```bash
open -a Instruments
```

Ou depuis Xcode :  
`Xcode > Open Developer Tool > Instruments`

#### 3. Choisir "Time Profiler"

1. Clique sur **"Time Profiler"** dans la liste des templates
2. Clique sur le cercle rouge pour démarrer l'enregistrement
3. Choisis le processus :
   - **Standalone** : cherche le nom de ton plugin `.app`
   - **Dans un DAW** : cherche "Reaper", "Logic Pro", etc.

#### 4. Reproduire les Scénarios de Performance

Dans ton plugin, effectue ces actions pendant 10-30 secondes :

- [x] Ouvrir l'interface graphique (mesure du lancement)
- [x] Changer de thème (mesure de `setTheme()`)
- [x] Redimensionner la fenêtre (mesure du `resized()` + `paint()`)
- [x] Interagir avec des sliders (mesure de la réactivité)
- [x] Interagir avec des boutons et ComboBox

#### 5. Arrêter et Analyser

1. Clique sur le carré noir pour arrêter
2. Dans la barre de recherche, filtre par `tss::` (ton namespace)
3. Trie par **"Self Time"** (temps passé dans cette fonction, hors appels enfants)

#### 6. Identifier les Hotspots

Cherche les fonctions avec **> 5% du temps CPU total** :

```
Call Tree
---------
45.2%  tss::Slider::paint()          <- HOTSPOT !
  22.1%  juce::Graphics::fillPath()
  18.3%  juce::Graphics::drawText()
12.8%  tss::ComboBox::paint()        <- HOTSPOT !
8.5%   tss::Button::paintButton()
3.2%   tss::Label::paint()           <- OK, pas critique
```

#### 7. Prioriser les Optimisations

**Règle : Optimiser les 20% de code qui consomment 80% du temps.**

Créer une liste des widgets à optimiser en priorité :

1. ________________ (% du temps CPU)
2. ________________
3. ________________

---

## Phase 4 : Optimisation Ciblée - Cache d'Images

### Objectif
Pré-rendre les parties statiques des widgets en images, puis dessiner ces images dans `paint()` au lieu de recalculer le rendu vectoriel à chaque frame.

### Principe du Cache d'Images

#### Avant (rendu vectoriel direct)

```cpp
void Slider::paint(juce::Graphics& g)
{
    // Recalcule tout à chaque paint() !
    g.fillAll(bgColour);              // 1. Fond
    g.drawRect(bounds, 1.0f);         // 2. Bordure
    g.fillRect(trackBounds);          // 3. Piste
    g.fillEllipse(thumbBounds);       // 4. Curseur (dynamique)
    g.drawText(valueText, ...);       // 5. Texte de valeur (dynamique)
}
```

**Coût** : ~100-500 µs par paint() × 50 sliders = 5-25 ms par frame

#### Après (cache d'image)

```cpp
class Slider : public juce::Component
{
public:
    void resized() override
    {
        regenerateCache();  // Pré-rendre le fond + bordure + piste
    }

    void paint(juce::Graphics& g) override
    {
        // 1. Dessiner le cache (très rapide !)
        g.drawImageAt(cachedBackground_, 0, 0);

        // 2. Dessiner uniquement les parties dynamiques
        g.fillEllipse(thumbBounds);       // Curseur
        g.drawText(valueText, ...);       // Valeur
    }

private:
    void regenerateCache()
    {
        cachedBackground_ = juce::Image(
            juce::Image::ARGB,
            getWidth(), getHeight(),
            true  // clear to transparent
        );

        juce::Graphics g(cachedBackground_);
        g.fillAll(bgColour);              // Une seule fois !
        g.drawRect(bounds, 1.0f);         // Une seule fois !
        g.fillRect(trackBounds);          // Une seule fois !
    }

    juce::Image cachedBackground_;
};
```

**Coût** : ~10-50 µs par paint() × 50 sliders = 0.5-2.5 ms par frame  
**Gain** : **10x à 20x** !

### Stratégie d'Implémentation

#### Widgets Statiques (cache complet)

Ces widgets ne changent jamais après création :

- **Label** - Texte fixe
- **GroupLabel** - Texte + cadre fixe
- **SectionHeader** - Titre de section
- **Separators** - Lignes fixes

**Stratégie** : Cache tout, dessine l'image dans `paint()`.

#### Widgets Semi-Dynamiques (cache partiel)

Ces widgets ont des parties fixes + des parties qui bougent :

- **Button** - Fond + bordure (fixe) + état hover/down (dynamique)
- **Slider** - Piste (fixe) + curseur + valeur (dynamique)
- **ComboBox** - Fond + bordure (fixe) + texte sélectionné (dynamique)

**Stratégie** : Cache le fond, dessine les parties dynamiques par-dessus.

#### Widgets Dynamiques (cache conditionnel)

Ces widgets changent fréquemment :

- **EnvelopeDisplay** - Courbe modifiée par l'utilisateur
- **TrackGeneratorDisplay** - Séquence modifiée
- **PatchNameDisplay** - Nom de patch change

**Stratégie** : 
- Cache quand même le fond + grille
- Redessine les données vectorielles (courbes, notes)
- Invalide le cache uniquement si les données changent

### Gestion du Zoom (Transformation Affine)

#### Problème
Si on applique un zoom avec `setTransform()`, les images mises à l'échelle deviennent floues.

#### Solution : Régénération du Cache au Changement de Zoom

```cpp
class MyWidget : public juce::Component
{
public:
    void setZoomFactor(float zoom)
    {
        if (juce::approximatelyEqual(zoomFactor_, zoom))
            return;  // Pas de changement

        zoomFactor_ = zoom;
        regenerateCache();  // Redessine en vectoriel au bon zoom
    }

private:
    float zoomFactor_ {1.0f};
};
```

**Note** : Le changement de zoom est rare (fait par l'utilisateur 1 fois au lancement), donc la régénération n'est pas un problème de performance.

### Checklist d'Implémentation

- [ ] Implémenter le cache sur un widget pilote (`Label` ou `Button`)
- [ ] Tester et valider le gain de performance
- [ ] Déployer sur les widgets identifiés comme hotspots (Phase 3)
- [ ] Implémenter la régénération du cache au changement de zoom
- [ ] Tester avec différentes résolutions d'écran

---

## Optimisations Complémentaires

### 1. Lazy Loading des Composants

**Principe** : Ne créer que les widgets visibles au démarrage.

```cpp
void BodyPanel::showModule(int moduleIndex)
{
    if (modules_[moduleIndex] == nullptr)
    {
        // Créer le module à la demande
        modules_[moduleIndex] = std::make_unique<DcoPanel>(...);
        addAndMakeVisible(*modules_[moduleIndex]);
    }

    modules_[moduleIndex]->setVisible(true);
}
```

**Gain** : Lancement 3-10x plus rapide.

### 2. `setOpaque(true)` pour les Composants Opaques

JUCE optimise le rendu si un composant est opaque (pas besoin de dessiner les composants en dessous).

```cpp
MyPanel::MyPanel()
{
    setOpaque(true);  // Ce composant a un fond opaque, pas de transparence
}
```

**Règle** :
- `setOpaque(true)` → si le widget remplit tout son bounds avec une couleur opaque
- `setOpaque(false)` → si le widget est transparent ou partiellement transparent

### 3. `setBufferedToImage(false)` par Défaut

JUCE peut bufferiser automatiquement un composant en image. **Désactive cette option** car on gère notre propre cache.

```cpp
MyWidget::MyWidget()
{
    setBufferedToImage(false);  // On gère notre propre cache
}
```

---

## Métriques de Succès

### Avant Optimisation (État Actuel)

À mesurer avec Instruments :

- Temps de lancement du plugin : ________ ms
- Temps de changement de thème : ________ ms
- Temps de redimensionnement : ________ ms
- FPS lors du déplacement de la fenêtre : ________ fps

### Après Optimisation (Objectifs)

- Temps de lancement : **< 500 ms** (objectif idéal : < 200 ms)
- Temps de changement de thème : **< 50 ms** (objectif : instantané)
- Temps de redimensionnement : **< 100 ms**
- FPS lors du déplacement : **> 30 fps** (objectif : 60 fps)

---

## Notes et Décisions

### Décisions Architecturales

- **Hiérarchie de panneaux** : On garde la structure actuelle (6 niveaux), le gain de tout refactoriser n'en vaut pas la peine.
- **Widgets custom** : On garde les `Component` JUCE, on optimise leur rendu.
- **Zoom** : Régénération du cache à chaque changement de zoom (solution pro, rendu net).

### Risques et Mitigations

| Risque | Impact | Mitigation |
|--------|--------|-----------|
| Cache consomme trop de mémoire | Moyen | Partager les caches entre widgets identiques |
| Régénération du cache au zoom est lente | Faible | Le zoom est rare, 1-2s de régénération est acceptable |
| Bugs visuels lors du cache | Moyen | Tester exhaustivement chaque widget après implémentation |

---

## Ressources et Références

### Documentation JUCE
- [Component::setOpaque()](https://docs.juce.com/master/classComponent.html#a8c6b6e4c4c9e9d8e5e5e5e5e5e5e5e5e)
- [Component::setBufferedToImage()](https://docs.juce.com/master/classComponent.html#a7d5c5e4d4d9e9e8e5e5e5e5e5e5e5e5e)
- [Graphics class](https://docs.juce.com/master/classGraphics.html)
- [Image class](https://docs.juce.com/master/classImage.html)

### Plugins de Référence (à analyser)
- **Vital** - Rendu ultra-performant, zoom jusqu'à 200%
- **Serum** - Interface vectorielle rapide
- **Diva** - GUI simple mais fluide

### Fichiers Clés du Projet
- `Source/GUI/Themes/Theme.h` - Système de thème
- `Source/GUI/MainComponent.cpp` - Racine de la hiérarchie
- `Source/GUI/Widgets/*.cpp` - Tous les widgets custom

---

**Dernière mise à jour** : 26 janvier 2026  
**Statut** : Phase 1 en cours (Nettoyage du design)
