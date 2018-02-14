/*
  This scripts allows usage of INA226's shunt resistor voltage measurement ADC for any
  high resolution voltage measurement application thanks to its high bit range (16 bit),
  from -81.9175 to 81.92 mV fixed (according to datasheet).

  It is assumed that INA226 doesn't have any shunt resistor connected to IN+ and IN- inputs.

  INA226 library by Korneliusz J.


 * @{
 *
 * @file        main.c
 * @brief       Test for RIOT's peripherial ADC intended to measure current across a shunt resistor.
 *
 * @author      Juan Pablo Futalef <juan.futalef@ing.uchile.cl>
 *
 * @}

*/

#include <Wire.h>
#include <INA226.h>

INA226 ina;

void checkConfig()
{
  Serial.print("Mode:                  ");
  switch (ina.getMode())
  {
    case INA226_MODE_POWER_DOWN:      Serial.println("Power-Down"); break;
    case INA226_MODE_SHUNT_TRIG:      Serial.println("Shunt Voltage, Triggered"); break;
    case INA226_MODE_BUS_TRIG:        Serial.println("Bus Voltage, Triggered"); break;
    case INA226_MODE_SHUNT_BUS_TRIG:  Serial.println("Shunt and Bus, Triggered"); break;
    case INA226_MODE_ADC_OFF:         Serial.println("ADC Off"); break;
    case INA226_MODE_SHUNT_CONT:      Serial.println("Shunt Voltage, Continuous"); break;
    case INA226_MODE_BUS_CONT:        Serial.println("Bus Voltage, Continuous"); break;
    case INA226_MODE_SHUNT_BUS_CONT:  Serial.println("Shunt and Bus, Continuous"); break;
    default: Serial.println("unknown");
  }

  Serial.print("Samples average:       ");
  switch (ina.getAverages())
  {
    case INA226_AVERAGES_1:           Serial.println("1 sample"); break;
    case INA226_AVERAGES_4:           Serial.println("4 samples"); break;
    case INA226_AVERAGES_16:          Serial.println("16 samples"); break;
    case INA226_AVERAGES_64:          Serial.println("64 samples"); break;
    case INA226_AVERAGES_128:         Serial.println("128 samples"); break;
    case INA226_AVERAGES_256:         Serial.println("256 samples"); break;
    case INA226_AVERAGES_512:         Serial.println("512 samples"); break;
    case INA226_AVERAGES_1024:        Serial.println("1024 samples"); break;
    default: Serial.println("unknown");
  }

  Serial.print("Bus conversion time:   ");
  switch (ina.getBusConversionTime())
  {
    case INA226_BUS_CONV_TIME_140US:  Serial.println("140uS"); break;
    case INA226_BUS_CONV_TIME_204US:  Serial.println("204uS"); break;
    case INA226_BUS_CONV_TIME_332US:  Serial.println("332uS"); break;
    case INA226_BUS_CONV_TIME_588US:  Serial.println("558uS"); break;
    case INA226_BUS_CONV_TIME_1100US: Serial.println("1.100ms"); break;
    case INA226_BUS_CONV_TIME_2116US: Serial.println("2.116ms"); break;
    case INA226_BUS_CONV_TIME_4156US: Serial.println("4.156ms"); break;
    case INA226_BUS_CONV_TIME_8244US: Serial.println("8.244ms"); break;
    default: Serial.println("unknown");
  }

  Serial.print("Shunt conversion time: ");
  switch (ina.getShuntConversionTime())
  {
    case INA226_SHUNT_CONV_TIME_140US:  Serial.println("140uS"); break;
    case INA226_SHUNT_CONV_TIME_204US:  Serial.println("204uS"); break;
    case INA226_SHUNT_CONV_TIME_332US:  Serial.println("332uS"); break;
    case INA226_SHUNT_CONV_TIME_588US:  Serial.println("558uS"); break;
    case INA226_SHUNT_CONV_TIME_1100US: Serial.println("1.100ms"); break;
    case INA226_SHUNT_CONV_TIME_2116US: Serial.println("2.116ms"); break;
    case INA226_SHUNT_CONV_TIME_4156US: Serial.println("4.156ms"); break;
    case INA226_SHUNT_CONV_TIME_8244US: Serial.println("8.244ms"); break;
    default: Serial.println("unknown");
  }

  Serial.print("Max possible current:  ");
  Serial.print(ina.getMaxPossibleCurrent());
  Serial.println(" A");

  Serial.print("Max current:           ");
  Serial.print(ina.getMaxCurrent());
  Serial.println(" A");

  Serial.print("Max shunt voltage:     ");
  Serial.print(ina.getMaxShuntVoltage());
  Serial.println(" V");

  Serial.print("Max power:             ");
  Serial.print(ina.getMaxPower());
  Serial.println(" W");
}

void setup()
{
  Serial.begin(115200);

  Serial.println("Initialize INA226");
  Serial.println("-----------------------------------------------");

  // Default INA226 address is 0x40
  ina.begin();

  // Configure INA226
  ina.configure(INA226_AVERAGES_256, INA226_BUS_CONV_TIME_140US, INA226_SHUNT_CONV_TIME_140US, INA226_MODE_SHUNT_BUS_CONT);

  // Calibrate INA226. Rshunt = 0.1 ohm, Max excepted current = 1A
  ina.calibrate(0.1, 1);

  // Display configuration
  checkConfig();

  Serial.println("-----------------------------------------------");
}

void loop()
{
  Serial.print("BUS VOLTAGE:   ");
  Serial.print(ina.readBusVoltage(), 5);
  Serial.println(" V");

  Serial.print("SHUNT VOLTAGE: ");
  Serial.print(ina.readShuntVoltage()*1000+2.5/1000000.0, 5);
  Serial.println(" mV");

  Serial.println("");
  delay(1000);
}
