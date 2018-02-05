# Power-Monitoring-Board
This repo contains scripts, details and specifications for a board designed to measure very low power consumption in MCU's with high accuracy.


## Organization

The repo is organized as follows

* RIOT: RIOT submodule, RIOT is used as the underlying OS for the mcu used to calibrate the current measuring circuit.
* ltspice: in this folder can be found the different spice schematics to simulate the different current measurement boards.
* scripts: in this folder can be found different c, python, etc. scripts used to read data from the current measurement circuit or calibrate the respective board.
* boards: the actual eagle schematics can be found in this folder, either for the circuits or breakout's for small smd-components.
