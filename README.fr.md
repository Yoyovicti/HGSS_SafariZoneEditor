[![English](https://img.shields.io/badge/EN-B31942)](README.md)
[![Français](https://img.shields.io/badge/FR-002654)](README.fr.md)

# HGSS_SafariZoneEditor
Un programme permettant de modifier les compteurs de jours du safari de Pokémon Heartgold et Soulsilver à partir d'un fichier de sauvegarde.

> [!WARNING]
> Ce programme est expérimental. Veuillez faire des copies de vos sauvegardes avant de les modifier.

## Description
Dans le Parc Safari de Pokémon HGSS, différents Pokémon peuvent apparaître en fonction des blocs qui sont placés dans les différentes zones. Chaque zone du Parc Safari possède également un compteur interne à la sauvegarde, qui agit comme un multiplicateur de l'effet des blocs placés allant de x1 (0 jour) à x7 (255 jours). À chaque fois que vous sauvegardez la partie, les compteurs sont mis à jour à partir du temps écoulé depuis votre dernière sauvegarde.

Si la plupart des Pokémon disponibles au Parc Safari peuvent être obtenus simplement en plaçant des blocs dans la zone adéquate, certains Pokémon nécessitent toutefois d'utiliser le multiplicateur pour apparaître. En tant que shinyhunter qui aime le Safari HGSS, découvrir que je dois attendre plus de 100 jours afin de pouvoir shasser ma cible n'est pas une expérience très amusante. S'il vous prend l'envie de patienter, prenez garde car changer votre jeu de console ou changer la date et l'heure vous infligera une pénalité qui empêchera le compteur de jours de fonctionner correctement...

S'il existe des techniques permettant de faire avancer le compteur de jours plus rapidement, elles sont toujours fastidieuses à utiliser et ne permettent pas de connaître avec précision la valeur de chaque compteur. J'ai développé ce petit outil avec l'objectif de pouvoir visualiser et modifier les compteurs de jours, permettant ainsi un contrôle maximal sur le Parc Safari.

## Support
Le programme est compatibles avec **Windows** et **Linux**. Les instructions CMake concernant **MacOS** ont été générées automatiquement et le support n'est en aucun cas testé ni garanti.

## Installation
Ce projet est compilé avec CMake.

### Pré-requis
- [CMake](https://cmake.org/) (version 3.5 ou plus)
- [Qt](https://www.qt.io/) (version 6)
- [nlohmann/json](https://github.com/nlohmann/json) (testé avec la version 3.11.3)

### Build instructions
1. Ouvrir un terminal et aller dans le dossier source du projet

2. Entrer les commandes suivantes :
```
mkdir build && cd build
cmake ..
make
```
Si une des librairies nécessaires n'est pas détectée, assurez-vous qu'elle est correctement installée sur votre système et que la variable ```CMAKE_PREFIX_PATH``` inclut le chemin vers la librairie correspondante.

3. Le fichier exécutable compilé devrait se trouver dans le dossier ```build```. Si vous souhaitez modifier l'emplacement du fichier exécutable, assurez-vous de déplacer le dossier ```data``` au même endroit.

## Licence
Le code source de ce projet est disponible sous [licence MIT](LICENSE.MIT).

Copyright © 2024-2025 Yoyovicti

Librairies utilisées:
- Ce projet utilise [nlohmann/json](https://github.com/nlohmann/json) disponible sous [licence MIT](LICENSE.MIT). Copyright © 2013-2024 Niels Lohmann
- Ce projet utilise [Qt](https://www.qt.io/) distribué sous licence [GNU LGPLv3](LICENSE.LGPL). Copyright © 2017 The Qt Company Ltd.
