# Project History

## Version 0.8.1 &nbsp; _(03/12/2024)_
- Remplacement des configurations de build "Debug" et "Release" par les configurations "DevDebug", DevRelease" et "ProdMaster".
- Ajout des macros GUGU_DEVELOPMENT_BUILD (DevDebug et DevRelease) et GUGU_PRODUCTION_BUILD (ProdMaster).
- Le flag de compilation IMGUI_DISABLE_OBSOLETE_FUNCTIONS est maintenant optionnel (désactivé par défaut).
- Update/Clean du fichier premake.
- Ajout de la méthode RecomputeIfNeeded sur les Element.
- Utilisation de stable_sort pour l'application du z-index.
- Imageset Editor : auto-scroll de la liste sur l'item sélectionné via le cadre de rendu.
- Mise à jour SFML 2.6.2.
- Mise à jour ImGui 1.91.4 (docking).
- Mise à jour ImGui-SFML.
- Mise à jour PugiXml 1.14.
- Mise à jour premake 5.0.0-beta3.

## Version 0.8 &nbsp; _(20/10/2024)_
- Ajout d'un nouveau logo !
- Ajout de ElementLayoutGroup, ElementCheckbox, ElementSlider.
- Ajout des ElementWidgets (ressource xml représentant une hierarchie d'Elements), refactor de la serialisation des Elements (ElementData).
- Editor: Version 0.3.
- Editor: Ajout de l'ElementWidget editor, ajout d'un générateur de Box9, ajout d'un shortcut pour generer imageset et animset à partir d'une texture.
- Ajout des Datasaves, clean du code de Databinding (séparation entre code commun, et spécifique Datasheet/Datasave, ajout d'une classe de base DataObject).
- Refactor serialisation v2 des datasheets (liste flat d'objets avec UUIDs, override d'instances au sein de la hierarchie).
- Datasheet Editor : update interface pour pouvoir réordonner les arrays, ajout d'un icone sur les references entre datasheets.
- Split de SystemUtility.h en Container.h, Memory.h, Path.h, Platform.h, String.h et Time.h.
- Ajout des utilitaires Handle, UUID, Signal.
- Ajout de LateUpdate et UpdateImGui dans la loop centrale.
- Ajout du parametre maxUpdateDeltaTime dans EngineConfig (par defaut à 100ms).
- Ajout de UnitTests (Xml, Databinding, picking).
- Update des Element UnifiedSize pour pouvoir se baser sur un Element donné (au lieu du parent).
- Ajout de FitWidth comme option de resize sur les ElementText.
- Update ParticleSystem : Gestion de la rotation de l'emitter, setting pour utiliser l'unscaledDeltaTime.
- Update Databindings : gestion des Vector2i et Vector2f.
- Update de GetFiles, GetDirectories et FileInfo pour se baser sur std::filesystem, ajout de getters utf8 sur FileInfo.
- Update de certains utilities pour utiliser std::string_view au lieu de générer des std::string (getters de FileInfo).
- Update DeltaTime (float pour les milliseconds).
- Update ManagerAudio (methodes de controle du master volume).
- Itération SystemUtility (StringEquals, StdMapTryGet).
- Itération MathUtility (DistanceCheck, Sign, Power2, renommage IsInRange en IsInBounds, Modulo variants).
- Refactor des utilities xml (Read, Parse, TryParse), gestion des Vector2, update de la serialisation des UDim2.
- Clean sur la nomenclature Remove/Delete dans divers Elements.
- Bugfix: ajout d'une safety pour la détection du focus au lancement de l'application.
- Mise à jour SFML 2.6.1.
- Mise à jour ImGui 1.87 (docking).
- Mise à jour ImGui-SFML 2.6.
- Mise à jour PugiXml 1.13.

## Version 0.7.1 &nbsp; _(10/12/2022)_
- Clean du build 64bits, retrait du build 32bits.
- Mise à jour du build de C++14 vers C++17.
- Fix du build pour vs2022.
- Simplification des scripts de build (encapsulation des appels redondants entre chaque projet).
- Retrait des sources de SFML, ImGui, ImGui-SFML et PugiXml, au profit de submodules (rangés dans /Externals/*).
- Mise à jour SFML 2.6.x.
- Mise à jour ImGui 1.85 (docking).

## Version 0.7 &nbsp; _(06/11/2022)_
- Editor: ajout du undo/redo, d'un fichier ProjectSettings, d'un fichier EditorConfig, et d'une popup de sauvegarde à la fermeture.
- Editor: Ajout du ParticleEffect editor, de l'AnimSet editor, d'un viewer pour les textures, et de l'OutputLog.
- Editor: Ajout d'un gestionnaire de dépendances entre Resources.
- Rangement des demos dans differents dossiers (separation des tests, des demos de systemes, de la demo HelloWorld, et de la demo Game).
- Ajout de UnitTests (MathUtility, SystemUtility, Elements).
- Ajout d'un ManagerVisualEffects et d'un ParticleSystem (nouvelle ressource ParticleEffect).
- Refacto Events (WindowEventHandler et ElementEventhandler, utilisation de listes de listeners plutot qu'un parsing de toute l'arborescence, split InteractionEvents et ElementEvents).
- Refacto de World/Level en ManagerScenes/Scene.
- Separation entre Renderer (virtual) et DefaultRenderer.
- Refacto DeltaTime (ajout unscaled time).
- Refacto des calculs de la loop centrale.
- Clean Element (suppression de Step/Update, suppression des parametres de size sur les callbacks de resize, renommage des fonctions de Transform).
- Iteration sur l'alignement au pixel des Elements (arrondi sur les UDim et la camera).
- Refacto ElementButton (utilisation d'un ElementSpriteGroup).
- Separation entre ElementText et ElementEditableText, utilisation de l'event TextEntered sur ElementEditableText.
- Ajout d'un système de template xml sur les ElementSpriteGroup.
- Itération datasheets (flag "abstract", declaration de fonctions dans le binding, attribut "code" optionnel sur les membres).
- Ajout d'un attribut SerializationVersion sur les différents fichiers de Resources.
- Ajout des commandes ruler, pause, time.
- Refacto de la commande speed (multiplicateur).
- Ajout des fonctions de rounding, de random dans un cercle/anneau, et de random pondéré.
- Itération SystemUtility (manipulation de paths).
- Update FileInfo (renommage des getters sous la forme GetFilePath, GetFileName, GetDirectoryPath).
- Ajout de ImGuiUtility, renommage de PugiXmlWrap en PugiXmlUtility.
- Clean Logger (meilleure separation entre Logger et EngineLogger, frame count, output dans visual studio).
- Update des scripts de build (wrapper pour un menu par defaut, option pour les subrepos).
- Mise à jour premake 5.0.0-beta1 (visual 2022).

## Version 0.6 &nbsp; _(01/10/2021)_
- Ajout du nouvel Editor (version 0.2), basé sur ImGui (datasheet editor).
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

## Version 0.5.2 &nbsp; _(06/04/2020)_
- Migration Bitbucket vers Github.
- Ajout des commandes speed et bounds.
- Ajout de la gestion des joysticks.
- Ajout des fonctions d'interpolation.
- Ajout du zoom sur les cameras.

## Version 0.5.1 &nbsp; _(04/07/2018)_
- Script de conversion des tabs en espaces.
- Mise à jour SFML 2.5.0.

## Version 0.5 &nbsp; _(30/06/2018)_
- Grosse refacto generale (arbo, renommages, camelCase) en preparation de la mise en open-source.
- Update/Optimisation des ElementSprite et ElementTile avec l'utilisation d'un VertexArray à la place de sf::sprites.
- Ajout de ElementTileMap et ElementSpriteGroup.
- Separation entre les assets de la demo Game et des autres demos.
- Separation des sources de PugiXml dans un projet dédié.

## Version 0.4 &nbsp; _(19/12/2017)_
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

## Version 0.3 &nbsp; _(20/05/2015)_
- Ajout de petites démos (HelloWorld, Datasheets, Game).
- Ajout de la gestion d'un Renderer avec passes de rendu.
- Remplacement du ManagerEvent par un HandlerEvents associé à chaque Window.
- Itération sur les Datasheets (héritage, enums).
- Clean Element : Gestion du sf::Transform en interne à la place d'un héritage sur sf::Transformable.
- const correctness.
- Mise à jour SFML trunk vers SFML 2.2 + integration des sources dans le projet.
- Abandon du module 3D (Ogre).

## Version 0.2 &nbsp; _(03/08/2014)_
- Ajout de la gestion des resources par nom.
- Ajout des bases pour les Datasheets.
- Mise à jour du système Audio (SoundCues, MusicLayers and fades).
- Itération de l'Editeur (ImageSets et AnimSets).
- Ajout du MeshViewer.
- Mise à jour Ogre 1.8 + bases d'archi pour le World 3D.

## Version 0.1 &nbsp; _(11/06/2013)_

## Early Prototyping &nbsp; _(2008 - 2013)_
- 07/07/2009 - Obtention d'un dépôt SVN, premier archivage (julz & pmcc hosted).
- 05/07/2009 - Grosse passe de clean : namespaces, includes, Game -> Engine (hamster check).
- 05/05/2009 - Ajout de la gestion d'acteurs. Bases de Gui presque finies. Première passe de clean général.
- 03/04/2009 - Base plutôt bien avancée, première version des images de la démo Volley. Gestion des déplacements parent/child.
- 21/03/2009 - Reboot du projet de moteur de jeu 2D sur SFML.
- 01/01/2009 - Prototype de moteur de jeu 2D basé sur SDL.
- 01/12/2008 - Prototype de moteur de jeu 3D basé sur OpenGl.
- 01/10/2008 - Prototype de machine à états.
