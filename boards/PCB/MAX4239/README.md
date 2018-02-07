# PCB eagle schematics and boards for MAX 4239

El MAX4239 es un OP AMP auto-zero, con un voltaje offset de entrada de 0.1uV tipico y 2uV en todo el rango de temperatura, además de un CMRR de  120dB minimos y 140dB típico.

### Cómo usar

Considerando que se tiene una placa compatible con RIOT:

- Conectar los controles de switch (SWITCH_CTRL_1 a SWITCH_CTRL_5) a GPIO.
- Conectar las salidas de low current, high current y adc volt (medición de supply) a las entradas analógicas.
- Conectar alimentación a entrada micro USB desde computadora o 5V.
- Conectar alimentación del MCU objetivo a VBAT. En caso de querer alimentar todo desde la alimentación USB, conectar un jumper entre 5V y VBAT.
- Si el MCU objetivo puede ser conectado por USB, utilizar el puerto USB tipo A. Este puerto mantiene la comunicación serial.
- En caso de no poder utilizar un USB, la alimentación del MCU objetivo puede obtenerse desde VMCU.


### Especificaciones:
- Alimentación de 5V desde USB
- Rango: ~10uA - 50mA
- Primera etapa ganancia de 100
- Segunda etapa amplifica 197 para bajas corrientes y 1.51 para altas corrientes.

### Working of current version (3) (detailed)

La primera etapa se encarga de amplificar el voltage en la resistencia de Shunt mediante configuracion de amplificador diferencial. Las resistencias R1,R2,R4 y R5 conforman una ganancia diferencial de 100 (100k/1k) entre VBAT y VS- (terminales de la resistencia de shunt). R6 y C3 son añadidos para estabilidad, C3 además ayuda a disminuir los peaks de carga del auto-zero. C2 ayuda a mantener la fuente estable.

![alt text](https://github.com/jpfutalef/Current-Measurement-Board/blob/master/boards/PCB/MAX4239/Images/IM2.png)

La segunda etapa es conformada por un DUAL OP AMP LTC6241 de alta precision. Hay dos rangos de medicion que tienen distintas ganancias cada uno:
- Low current range: 0uA-600uA
- High current tange: 600uA-50mA

Para corrientes bajas, la segunda etapa amplifica x197. Para corrientes altas solo se amplifica x1.51. Estas ganancias acomodan el voltaje a los rangos del ADC. Las resistencias de 49.9R son utiles para aislar la salida del amplificador y evitar oscilaciones.

![alt text](https://github.com/jpfutalef/Current-Measurement-Board/blob/master/boards/PCB/MAX4239/Images/IM5.png)

Se utilizan 4 switch analogos normalmente abiertos para calibrar la medición en corrientes pequeñas utilizando las resistencias R9, R10, R13 y R14. Los TS5A23166DCUR son elegidos debido al alto voltaje soportado en sus entradas, alta corriente máxima y baja resistencia (0.7 omhs aprox.). En cambio, se elige un switch normalmente cerrado para la alimentación del MCU objetivo, un TS5A23167DCUR, de características equivalentes al TS5A23166DCUR. El capacitor C1 es puesto entre la alimentación del MCU objetivo para asegurar estabilidad.

![alt text](https://github.com/jpfutalef/Current-Measurement-Board/blob/master/boards/PCB/MAX4239/Images/IM1.png)

La etapa de buffer para la fuente de poder utiliza un LTC6240 de alta precision, configurado a ganancia x0.5 con tal de evitar que valores altos de voltajes no puedan ser leidos por ADC de referencias de voltaje bajo (5V fuente -> 2.5V ADC puede ser leido con referencia de 3.3V). Además permite aislar el nodo del ADC.

![alt text](https://github.com/jpfutalef/Current-Measurement-Board/blob/master/boards/PCB/MAX4239/Images/IM3.png)

Para la alimentación, se utiliza una entrada micro USB tipo B hembra, que proporciona 5V desde una computadora o cargador. En caso de uso con la computadora, se han cortocircuitados las líneas de datos D+ y D-. Para evitar el ruido de alta frecuencia del computador, se utiliza una ferrita de alta impedancia a baja frecuencia (1K@1Mhz). Un conector hembra USB tipo A proporciona la salida del circuito para alimentar al MCU objetivo. Finalmente, se separa la tierra virtual de la digital mediante diodos schottky CUS08F30 debido a su rápida respuesta y bajo voltaje de activación.

![alt text](https://github.com/jpfutalef/Current-Measurement-Board/blob/master/boards/PCB/MAX4239/Images/IM6.png)

Diversos pines de utilidad son puestos a disposición del usuario. Estos permiten alimentar con fuente externa, observar las salidas de las distintas etapas, controlar switchs, etc. Las resistencias 21 a 25 son de pull down.

- SWITCH_CTRL_1: controla apertura del switch que proporciona energia al MCU.
- SWITCH_CTRL_2 a 5: controla cierre de switch a resistencias de calibracion.
- ADC_VOLT: salida buffer supply.
- VMCU: voltaje que se entrega al MCU objetivo. En corto circuito con el usb.
- GND: tierra
- OUT_STAGE1: salida primera etapa correinte.
- VS-: pata negativa resistencia de shunt.
- VBAT: voltaje con el que se supone se alimenta el MCU.
- +5V: salida de 5V, separada de USB por la ferrita.


![alt text](https://github.com/jpfutalef/Current-Measurement-Board/blob/master/boards/PCB/MAX4239/Images/IM7.png)

El MAX4239 puede ser alimentado hasta 6V. Sin embargo en sus entradas diferenciales sólo pueden ingresar hasta aproximadamente 0.3V más que la fuente de voltaje del MAx4239. De todas formas, se ha observado que para entradas de 5V (que pueden provenir de un USB) correspondientes a una de las patas de la resistencia de shunt provoca oscilaciones. Esto debiese solucionarse alimentando al MAX4239 con 5.5V utilizando un DC booster MIC2250 fijado a 5.5V. La configuracion usada es la recomendada por el fabricante.

![alt text](https://github.com/jpfutalef/Current-Measurement-Board/blob/master/boards/PCB/MAX4239/Images/IM3.png)

# Versiones placas

## MAX4239 V1 (no prototipada)
Esquemático es equivalente al de la versión 2. El único cambio es de la pcb board que se descarta por ser de gran tamaño y no tener bien las reglas de diseño.

## MAX4239 V2 (prototipada)
Esta versión de la placa fue prototipada. La PCB fue manufacturada por OSHPARK y los componentes fueron comprados en DigiKey. El soldado de componentes y pruebas de laboratorio fueron realizados en el IOT LAB de Inria Chile.

### Acerca de

Esta versión de la placa considera una alimentación de 5V para todos los IC, proveniente de la entrada micro USB. 

## MAX4239 V3 (no prototipada)

# Conclusiones a partir del prototipado de la placa

## Logros
- Prototipado de placa
- Soldado de componentes
-

## Issues
