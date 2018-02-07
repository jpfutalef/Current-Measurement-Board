# Power measurement Scripts

This folder contains resources to control the measuring node and register different current measurements. The files contained here are:

* **master.py**: python script that controls the measure_node. It opens a serial terminal, reads/receives calibration values from the node, and proceeds to specify sampling rate for the node. Measured values (ADC voltage readings) are transformed to current  values.

* **measure_node**: contains c code based on RIOT to flash on node. It's assumes that RIOT is in the same directory as this repo.

## Future work

As of now master.py just captures serial data but isn't logging it anywhere. This should be interfaced/adapted to work with FIT IoT-Lab code or other.
