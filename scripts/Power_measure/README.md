# SCRIPTS PARA EL USO DE RIOT EN CONJUNTO CON PLACA DE MEDICION DE CONSUMO PARA MCU'S

Los archivos son:
  - main.c : Script en RIOT que permite usar 3 perifericos analogicos para
  lectura de voltage, 4 GPIO para calibracion y 1 para control.
  Las variables MODE, DELAY, SAMPLES_ADC y SAMPLES_CAL no son macros
  pues se busca que en un futuro puedan ser modificables online de acuerdo
  a lo solicitado por el usuario desde el computador.

  Su forma de funcionamiento es:

    1. Inicialización de pines con pins_init() (GPIO de resistores en LOW y GPIO de VMCU en HIGH para cortar voltaje del MCU objetivo)
    2. Inicialización de ADC's con adc_lines_init()
    3. Calibrar con calibrate(). Esta funcion envia un buffer bien definido con los valores obtenidos en la calibracion.
    4. Set HIGH a GPIO de VMCU para activar voltaje al MCU objetivo.
    5. Realizar lecturas de acuerdo al tiempo especificado en la variable DELAY y enviarlas en un buffer bien definido.


  - master.py: Script en python tipo master pues tiene como objetivo controlar las
    acciones del microcontrolador de lectura. Por el momento solo recibe data.
    Las variables al comienzo son utiles para multiples calculos y deben ser
    especificadas.
    El proceso está continuamente en un loop que lee lo que entrega el puerto serial.
    De acuerdo al modo recibido se efectuara una calibracion o una medicion.

  - Makefile: makefile para main.c que especifica el directorio de RIOT y algunos modulos
    usados.
