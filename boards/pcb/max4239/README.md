# PCB eagle schematics and boards for MAX 4239

MAX4239 is a auto-zero amplifier, with low input voltage offset of 1.0uV (typical) and 2uV (full temp range), it also has a high CMRR of 120dB (min), 140dB (typ).

### Some notation
* current measuring board (CMB): the designed circuit that measure shunt voltage, and supply voltage.
* mcu: the microcontroller that is connect to the CMB, performs calibrations and converts the supply voltage and current measurements.
* node: the target mcu which pwoer consumption the CMB is measuring.

### How to use

* connect switch controllers (SWITCH_CTRL_1 to SWITCH_CTRL_5) to 5 different GPIO on any mcu.
* connect the low current, high current and supply voltage output to 3 different ADC inputs on your mcu.
* power the CMB by either connecting it to it's microUSB port ore the 5V supply pin.
* bypass the targeted node power supply by either connecting it's power source to VBAT or the USB port on CMB.
* if the targeted node supply source is connected to VBAT then the node can be powered by connecting it's power input to VMCU, if it's USB powered a jumper must be connected between 5V and VBAT on CMB.

### Ratings V3:
- Power supply: 5V from USB
- Current range(ideal): ~1uA - 50mA
- First stage gain x10
- Second stage gain x197 for low currents and x1.51 for high currents.

### Ratings V3:
- Power supply: 5V from USB
- Current range(ideal): ~1uA - 50mA
- First stage gain x109
- Second stage gain x19.7 for low currents and x0.151 for high currents.


### MAX4239 V3 (detailed)

The first stage amplifies the voltage measured at the shunt by applying a differential gain. R1, R2, R4 & R5 form a x100 gain. R6 and C3 are added for stability, C3 also helps to reduce currents peaks. C2 stabilizes the power supply.

![max4239_gain_stage](IM2.png)

The second stage is conformed by a dual LTC6241 high precision opamp. There are two current ranges with to different gains:

- Low current range: 0uA-600uA, x197 gain
- High current range: 600uA-50mA, x 1.51 gain

These gains values adjust to the ADC input ranges. The 49.9 resistor are put in place to isolate the amplifier output and the ADC input, thus preventing oscillation.

![ltc6241_gain_stage](IM5.png)

4 analog switches (usually open are) used for calibration, this is done by measuring current flow threw resistors R9, R10, R13 and R14. TS5A23166DCUR switches where chosen because of the high voltage input range, high max current and low Ron (0.8 ohms).

A normally closed switch is connected between the target node and it's supply source. The TS5A23167DCUR wash chosen with similar characteristics as the TS5A23167DCUR.

C1 is connected between the supply source and ground to stabilize it.

![switches](IM1.png)

The buffer stage to measure the supply source uses a LTC6240 high precision amplifier with a x0.5 gain stage to allow supply voltage higher than the mcu supply (3.3V) to be converted. This buffer stages also isolated the two.

![voltage_buffer](IM3.png)

As a supply source a microUSB type B female connector is used. This supply a 5V power supply. To reduce high frequency noise a high impedance, low frequency ferrite is used (1K@1Mhz).

A female type A USB connector can be used to power the targeted node. Virtual and digital ground are separated by using two schottky diodes, CUS08F30 because of their high response and low activation voltage.

![power_supply_sources](IM6.png)

Multiple input/output pins are supplied (R21 to R25 are pull down resistors so calibration resistors and node supply are normally turned off). These are:

- SWITCH_CTRL_1: switch's on power supply for targeted node.
- SWITCH_CTRL_2 a 5: switches on or off calibration resistors.
- VMCU: output voltage that powers the targeted node it's short-circuited to the female type A USB.
- GND: ground
- OUT_STAGE1: first amplifying stage gain output, for debugging purposes.
- VS-: shunt resistor low voltage pin.
- VBAT: pin where targeted node supply voltage is connected to bypass it.
- +5V: 5V output, its separated from the USB by the ferrite.

![input_output_pins](IM7.png)

MAX4239 maximum supply voltage is 6V & it's differential inputs can only take inputs 0.3V bellow its supply voltage. It has been observed that for 5V inputs in one of the differential inputs the MAX4239 voltage output oscillates. This is fixed by using a MIC2250 booster to boost supply to 5.5 volts. The configurations used is the one recommended by manufacturer.

![max4239_supply_boost](IM3.png)

# Board versions

## MAX4239 V1 (no prototipada)
Esquemático es equivalente al de la versión 2. El único cambio es de la pcb board que se descarta por ser de gran tamaño y no tener bien las reglas de diseño.

## MAX4239 V2 (prototipada)
Esta versión de la placa fue prototipada. La PCB fue manufacturada por OSHPARK y los componentes fueron comprados en DigiKey. El soldado de componentes y pruebas de laboratorio fueron realizados en el IOT LAB de Inria Chile.

### Acerca de

Esta versión de la placa considera una alimentación de 5V para todos los IC, proveniente de la entrada micro USB.

La primera etapa es un amplificador diferencial de ganancia 10. La segunda etapa consta de los rango Low Current (ganancia de 19,7) y high current (ganancia de 1,51).

Para lecturas de la alimentacion del MCU objetivo, se utiliza una etapa que reduce el voltage a la mitad.

Tierras digital y analoga son separadas con diodos schottky.

Fuente de 5v de USB es filtrada con ferrita 1k@1MHz.

La carcasa del USB tipo A es conectado a tierra digital.

### Conclusiones a partir del prototipado de la placa

#### Observaciones
- Placa manufacturada
- Soldado de componentes OK
- Todos los IC reciben alimentación debida.
- Todos los IC reciben tierra debida.
- Conectando multiples resistencias para emular consumo se han logrado medir en el alto rango corrientes de 600uA.
- Utilizando los switch para calibrar con las resistencias, se han observado valores que debiesen permitir calibrar.

#### Qué falta hacer
- Checkear el rango efectivo de medición.
- Realizar medición con microcontrolador. Que este haga la calibración y controle los switches.

#### Issues
- Para alimentación de 5V se observan oscilaciones en la primera etapa. Se han agragado resistencias de 1KHz a tierra a la salida, observándose disminución aún continúa. El problema es solucionado alimentando externamente el circuito con 5.5V o más. Se concluye que el problema es debido a que en las entradas del MAX4239 hay un máximo de voltaje limitado por la alimentación.
- Condensador C2 está mla puesto, debe conectarse a tierra. De todas formas puede quitarse y hacer un puente sin problemas.
- La ganancia de la etapa de low current debiese ser 187, no 19,7. Esto fue debido a que la resistencia de 10K es incorrecta, debiese ser de 1K.
- Los switch de control de alimentación al MCU están mal conectados. Esto fue solucionado soldando un cable a las patas que lo controlan.
- Agregar resistencias de pulldown a los controles de los switch como precaución en caso de no existir.
- En la placa, hay nombres en la silkscreen cambiados.
- Con respecto al laboratorio: muchos problemas de estática, ocurrieron muchos casos de descarga electroestática que producían oscilaciones en el circuito y que podrían dañar a IC. El osciloscopio tiene un problema, en cierto momento se detiene y debe ser reiniciado, al momento de reiniciarse el canal 1 siempre ponía impedancia de 50ohm e invertido, afectando la observación.

#### Qué se podría mejorar
- Mejorar CMRR incrementando tolerancia de TODAS las resistencias, que sean de 0,1% o menos. También puede incrementarse aumentando la ganancia diferencial, pero esto afecta los rangos de medición.
- Los micro USB ojalá que sean Thru-hole para evitar que se salgan.
- Para el switch que entrega alimentación al MCU objetivo, unir los dos switch y así permitir más paso de corriente.

#### Tips
- Uso de pasta de soldar y pistola de calor es recomendable para componentes de tamaño muy reducido
- Cautín de punta plana y malla de cobre con flux ayuda a remover exceso de estaño
- USB thru-hole ojalá soldar con estaño de alto punto de fusión pues es más duro
- Descarga a tierra puede hacerse con salida a tierra de los equipos

## MAX4239 V3 (no prototipada)

Esta versión debiese solucionar todos los issues presentados en la versión anterior.
- El problema de la alimentación es solucionado utilizando un DC booster que entregue 5V5 de alimentación (MIC2250).
- C2 está bien puesto.
- La ganancia de low current es solucionada cambiando la resistencia de 10K por 1K.
- Los switch están bien conectados y reorganizados.
- Se incluyen resistencias de pull-down en las lineas de control de los switch.

La placa aún no se ha diseñado, sólo está el esquemático.
