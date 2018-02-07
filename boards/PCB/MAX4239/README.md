# PCB eagle schematics and boards for MAX 4239

El MAX4239 es un OP AMP auto-zero, con un voltaje offset de entrada de 0.1uV tipico y 2uV en todo el rango de temperatura, además de un CMRR de  120dB minimos y 140dB típico.

## MAX4239 V2

## MAX4239 V3

### Especificaciones:
- Alimentación de 5V desde USB
- Rango: ~10uA - 50mA
- Primera etapa ganancia de 100
- Segunda etapa amplifica 197 para bajas corrientes y 1.51 para altas corrientes.

### Funcionamiento (resumen)

Hay una primera etapa de amplificacion para la caída de tension en la resistencia de shunt. Una segunda etapa de amplificacion sirve como buffer para ADC y amplificacion para adecuar los voltajes necesarios para el ADC. Dos amplificadores en la segunda etapa permiten sensar rangos de microamperes y miliamperes.

Una etapa de reduccion de voltaje en la fuente de alimentacion del MCU permite leer el voltaje con bastante precision.

Finalmente, un conjunto de resistencia y switch analogos permiten calibrar para compensar la zona no lineal en la que el amplificador entra al acercarse al voltaje de offset en la entrada.

### Working (detailed)

La primera etapa se encarga de amplificar el voltage en la resistencia de shunt mediante configuracion de amplificador diferencial. El IC debe ser tal que su offset de entrada sea bajo y no se compare al voltaje que se está amplificando (para microamperes hay una caida de voltaje de microvolts en la resistencia de shunt). Además debe disminuirse lo más posible el CMRR, para obtener una amplificacion a lazo cerrado ojalá solamente diferencial.

Las resistencias R1,R2,R4 y R5 conforman una ganancia diferencial de 100 (100k/1k) entre VBAT y VS- (terminales de la resistencia de shunt). R6 y C3 son añadidos para estabilidad, C3 además ayuda a disminuir los peaks de carga del auto-zero. C2 ayuda a mantener la fuente estable.

![alt text](https://github.com/jpfutalef/Current-Measurement-Board/blob/master/pcb_layout/PCB/MAX4239/Images/IM2.png)

La segunda etapa es conformada por un DUAL OP AMP LTC6241 de alta precision. Hay dos rangos de medicion que tienen distintas ganancias cada uno:
- Low current range: 0uA-600uA
- High current tange: 600uA-50mA

Para corrientes bajas, la segunda etapa amplifica x197, util para lectura en ADC. Para corrientes altas solo se amplifica x1.51. Las resistencias de 49.9R son utiles para aislar la salida del ADC y evitar oscilaciones.

![alt text](https://github.com/jpfutalef/Current-Measurement-Board/blob/master/pcb_layout/PCB/MAX4239/Images/IM5.png)

La etapa de buffer para la fuente de poder utiliza un LTC6240 de alta precision, configurado a ganancia x0.5 con tal de evitar que valores altos de voltajes no puedan ser leidos por ADC de referencias de voltaje bajo (5V fuente -> 2.5V ADC puede ser leido con referencia de 3.3V). Además permite aislar el nodo del ADC.

![alt text](https://github.com/jpfutalef/Current-Measurement-Board/blob/master/pcb_layout/PCB/MAX4239/Images/IM3.png)
