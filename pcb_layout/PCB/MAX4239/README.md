# PCB layouts para placas basadas en MAX 4239

#### Todas las especificaciones son para la version 3 (MAX4239basedV3)

El MAX4239 es un OP AMP auto-zero, con un voltaje offset de entrada de 0.1uV tipico y 2uV en todo el rango de temperatura, además de un CMRR de  120dB minimos y 140dB típico.

### Especificaciones:
- Alimentacion de 5V desde USB
- Rango: ~10uA - 50mA
- Primera etapa ganancia de 100
- Segunda etapa amplifica 197 para bajas corrientes y 1.51 para altas corrientes.

### Funcionamiento (resumen)

Hay una primera etapa de amplificacion para la caida de tension en la resistencia de shunt. Una segunda etapa de amplificacion sirve como buffer para ADC y amplificacion para adecuar los voltajes necesarios para el ADC. Dos amplificadores en la segunda etapa permiten sensar rangos de microamperes y miliamperes.

Una etapa de reduccion de voltaje en la fuente de alimentacion del MCU permite leer el voltaje con bastante precision.

Finalmente, un conjunto de resistencia y switch analogos permiten calibrar para compensar la zona no lineal en la que el amplificador entra al acercarse al voltaje de offset en la entrada.

### Funcionamiento (detallado)

La primera etapa se encarga de amplificar el voltage en la resistencia de shunt mediante configuracion de amplificador diferencial. El IC debe ser tal que su offset de entrada sea bajo y no se compare al voltaje que se está amplificando (para microamperes hay una caida de voltaje de microvolts en la resistencia de shunt). Además debe disminuirse lo más posible el CMRR, para obtener una amplificacion a lazo cerrado ojalá solamente diferencial.

CMRR depende de la tolerancia de las resistencias usadas:

<div align="center">
<a href="http://www.codecogs.com/eqnedit.php?latex=CMRR&space;=&space;\dfrac{A_d&plus;1}{4t}" target="_blank"><img src="http://latex.codecogs.com/gif.latex?CMRR&space;=&space;\dfrac{A_d}{1&plus;4t}" title="CMRR = \dfrac{A_d}{1+4t}" /></a></div>

Ad es la ganancia diferencial y t la tolerancia. Se ha concluido que deben usarse resistencias de tolerancia 0.1% o menos.

Las resistencias R1,R2,R4 y R5 conforman una ganancia diferencial de 100 (100k/1k) entre VBAT y VS- (terminales de la resistencia de shunt). R6 y C3 son añadidos para estabilidad, C3 además ayuda a disminuir los peaks de carga del auto-zero. C2 ayuda a mantener la fuente estable.

![alt text](https://github.com/jpfutalef/Current-Measurement-Board/blob/master/pcb_layout/PCB/MAX4239/Images/IM2.png)

La segunda etapa es conformada por

![alt text](https://github.com/jpfutalef/Current-Measurement-Board/blob/master/pcb_layout/PCB/MAX4239/Images/IM5.png)
