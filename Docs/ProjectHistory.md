# Project History

## ??/??/???? - Version 0.7 (WIP 28/10/2022)
- Editor: ajout du undo/redo, d'un fichier ProjectSettings, d'un fichier EditorConfig, et d'une popup de sauvegarde à la fermeture.
- Editor: Ajout du ParticleEffect editor, de l'AnimSet editor, d'un viewer pour les textures, et de l'OutputLog.
- Editor: Ajout d'un gestionnaire de dépendances entre Resources.
- Rangement des demos dans differents dossiers (separation des tests, des demos de systemes, de la demo HelloWorld, et de la demo Game).
- Ajout d'un ManagerVisualEffects et d'un ParticleSystem (nouvelle ressource ParticleEffect).
- Ajout de UnitTests (MathUtility, SystemUtility, Elements).
- Refacto Events (WindowEventHandler et ElementEventhandler, utilisation de listes de listeners plutot qu'un parsing de toute l'arborescence, split InteractionEvents et ElementEvents).
- Refacto DeltaTime (ajout unscaled time).
- Refacto des calculs de la loop centrale.
- Iteration sur l'alignement au pixel des Elements (arrondi sur les UDim et la camera).
- Refacto ElementButton (utilisation d'un ElementSpriteGroup).
- Separation entre ElementText et ElementEditableText.
- Ajout d'un système de template xml sur les ElementSpriteGroup.
- Itération datasheets (flag "abstract", declaration de fonctions dans le binding, attribut "code" optionnel sur les membres).
- Ajout d'un attribut SerializationVersion sur les différents fichiers de Resources.
- Refacto de World/Level en ManagerScenes/Scene.
- Separation entre Renderer (virtual) et DefaultRenderer.
- Refacto de la commande speed (multiplicateur).
- Update des scripts de build (wrapper pour un menu par defaut, option pour les subrepos).
- Ajout des commandes ruler, pause, time.
- Ajout des fonctions de rounding, de random dans un cercle/anneau, et de random pondéré.
- Itération SystemUtility (manipulation de paths).
- Ajout de ImGuiUtility, renommage de PugiXmlWrap en PugiXmlUtility.
- Update FileInfo.
- Clean Element (suppression de Step/Update, suppression des parametres de size sur les callbacks de resize, renommage des fonctions de Transform).
- Utilisation de l'event TextEntered sur ElementText.
- Clean Logger (meilleure separation entre Logger et EngineLogger, frame count, output dans visual studio).
- Mise à jour premake 5.0.0-beta1 (visual 2022).

## 01/10/2021 - Version 0.6
- Ajout du nouvel Editor, basé sur ImGui (datasheet editor).
- Ajout d'un ManagerAnimation, refacto SpriteAnimation, suppression d'ElementSpriteAnimation.
- Itération datasheets (dissociation entre ressource et data gameplay, force const sur les datas, format xml, gestion des references/instances null).
- Refacto des classes de sprite (Ajout d'ElementSpriteBase, suppression d'ElementTile, refacto d'ElementSpriteGroup pour heriter d'ElementSpriteBase).
- Refacto ElementTileMap.
- Clean et ajout de callbacks (internes et publiques) sur la classe Element.
- Refacto Grids (square + hex grids), ajout d'algos generiques de breadth-first-search.
- Refacto des Actions et Delegates via des std::function (Callback wrapper).
- Refacto des fonctions de lancement (RunApplication).
- Ajout d'un gugu::Vector2 pour encapsuler sf::Vector2.
- Update des differents formats de fichier xml.
- Refacto de la commande stats.
- Itération démo Game (interfaces, loop gameplay, skill system).
- Clean des scripts de build.
- Reorganisation des dossiers.
- Ajout ImGui 1.84.1 (integration via ImGui-SFML 2.3).
- Mise à jour SFML 2.5.1.
- Mise à jour PugiXml 1.11.4.

## 06/04/2020 - Version 0.5.2
- Migration Bitbucket vers Github.
- Ajout des commandes speed et bounds.
- Ajout de la gestion des joysticks.
- Ajout des fonctions d'interpolation.
- Ajout du zoom sur les cameras.

## 04/07/2018 - Version 0.5.1
- Script de conversion des tabs en espaces.
- Mise à jour SFML 2.5.0.

## 30/06/2018 - Version 0.5
- Grosse refacto generale (arbo, renommages, camelCase) en preparation de la mise en open-source.
- Update/Optimisation des ElementSprite et ElementTile avec l'utilisation d'un VertexArray à la place de sf::sprites.
- Ajout de ElementTileMap et ElementSpriteGroup.
- Separation entre les assets de la demo Game et des autres demos.
- Separation des sources de PugiXml dans un projet dédié.

## 19/12/2017 - Version 0.4
- Ajout démos (Camera, Audio, Callback, UI, Network, Math, System).
- Ajout d'un état Disabled sur les ElementButtons.
- Ajout de l'outil de Scope Trace (generation d'un json exploitable sur chrome/opera).
- Mise à jour des Flip (Flip generique autour du pivot pour les Element, FlipTexture pour les sprites).
- Itération commande console (ajout commandes stats, fps, trace).
- Itération World/Level (render, camera, levels, actors).
- Itération Animations (root-motion).
- Itération Audio (Music playlist).
- Itération du Build process (mise en place simplifiée pour un nouveau projet, flags de compilation accelérée).
- Itération System Utility (FormatString).
- Mise à jour SFML 2.4.2.
- Mise à jour Editor Qt 5.7.

## 20/05/2015 - Version 0.3
- Ajout de petites démos (HelloWorld, Datasheets, Game).
- Ajout de la gestion d'un Renderer avec passes de rendu.
- Remplacement du ManagerEvent par un HandlerEvents associé à chaque Window.
- Itération sur les Datasheets (héritage, enums).
- Clean Element : Gestion du sf::Transform en interne à la place d'un héritage sur sf::Transformable.
- const correctness.
- Mise à jour SFML trunk vers SFML 2.2 + integration des sources dans le projet.
- Abandon du module 3D (Ogre).

## 03/08/2014 - Version 0.2
- Ajout de la gestion des resources par nom.
- Ajout des bases pour les Datasheets.
- Mise à jour du système Audio (SoundCues, MusicLayers and fades).
- Itération de l'Editeur (ImageSets et AnimSets).
- Ajout du MeshViewer.
- Mise à jour Ogre 1.8 + bases d'archi pour le World 3D.

## 11/06/2013 - Version 0.1

## Early Prototyping
- 07/07/2009 - Obtention d'un dépôt SVN, premier archivage (julz & pmcc hosted).
- 05/07/2009 - Grosse passe de clean : namespaces, includes, Game -> Engine (hamster check).
- 05/05/2009 - Ajout de la gestion d'acteurs. Bases de Gui presque finies. Première passe de clean général.
- 03/04/2009 - Base plutôt bien avancée, première version des images de la démo Volley. Gestion des déplacements parent/child.
- 21/03/2009 - Reboot du projet de moteur de jeu 2D sur SFML.
- 01/01/2009 - Prototype de moteur de jeu 2D basé sur SDL.
- 01/12/2008 - Prototype de moteur de jeu 3D basé sur OpenGl.
- 01/10/2008 - Prototype de machine à états.
