[![English](https://img.shields.io/badge/EN-B31942)](README.md)
[![Français](https://img.shields.io/badge/FR-002654)](README.fr.md)

# HGSS_SafariZoneEditor
A tool made for editing Safari Zone day counters in Pokémon HeartGold and SoulSilver save files.

> [!WARNING]
> This program is still experimental. Always make sure to backup your save files before overwriting.

## Description
In the Pokémon HGSS Safari Zone, different Pokémon species may appear depending on the blocks placed in the various available areas. Each area has an internal counter stored in the save file that acts as a multiplier for the block power, which can range from x1 (0 day) to x7 (255 days). Every time you save your game, these counters are updated to reflect the time elapsed since your last save.

While most Pokémon can appear by simply placing the required amount of blocks in their respective area, some of them require a multiplier to appear. As a shinyhunter who enjoys hunting in the HGSS Safari Zone, I've often been frustrated by planning a target only to discover that I need to wait over 100 days for it to show up. If you still intend to wait the required number of days, keep in mind that switching consoles or changing your system's date and time may freeze the counting mechanism, adding to an already frustrating experience...

Although some techniques allow you to speed up the increase of the day counters, they remain tedious to use and do not show the actual values of each counter. I developed this tool primarily to provide an easy and efficient way to visualize and edit the day counters, giving users complete control over the Safari Zone.

## Support
Support is provided for **Windows** and **Linux** distributions. The provided **MacOS** instructions for CMake have been auto-generated and support is not tested or guaranteed on this platform.

## Installation
This project uses CMake as its build system.

### Prerequisites
- [CMake](https://cmake.org/) (version 3.5 or later)
- [Qt](https://www.qt.io/) (version 6)
- [nlohmann/json](https://github.com/nlohmann/json) (tested with version 3.11.3)

### Build instructions
1. Open a terminal and navigate to the project's root directory

2. Enter the following commands:
```
mkdir build && cd build
cmake ..
make
```
If any of the required libraries cannot be found, ensure they are correctly installed on your system and that the ```CMAKE_PREFIX_PATH``` variable includes the path to the libraries.

3. The built executable will be available in the ```build``` directory. Ensure the ```data``` directory remains with the executable file if you want to move it elsewhere.

## License
The source code of this project is licensed under the [MIT License](LICENSE.MIT).

Copyright © 2024 Yoyovicti

Additional libraries:
- This project uses nlohmann/json which is licensed under the [MIT License](LICENSE.MIT). Copyright © 2013-2024 Niels Lohmann
- This project uses the Qt Library which is licensed under [GNU LGPLv3](LICENSE.LGPL). Copyright © 2017 The Qt Company Ltd.
