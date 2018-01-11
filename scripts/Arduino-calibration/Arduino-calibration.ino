#include <Wire.h>
#include <INA226.h>

// Calibration resistors
float R1 = 1000.0;
float R2 = 10000.0;
float R3 = 100000.0;
float R4 = 1000000.0;

// Calibration switch
#define CALSW 5

// INA226 structure
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
}

void calibration()
{
  for(int i = 0; i < 5; i++)
  {
    Serial.println("Push calibration button!");
    while(digitalRead(CALSW) != HIGH){
      //...
    }

    if(i == 0)
    {
      Serial.print("No resistor connected.");
      Serial.println();
      delay(2000);  //Aqui se hace los de calibracion
    }

    else
    {
      Serial.print("Calibrating resistor ");
      Serial.print(i+1);
      Serial.println();
      delay(2000);  //Aqui se hace los de calibracion
    }
  }

  Serial.println("Done!");
}

void calibrate(int resistorCase)
{
  switch(resistorCase)
  {
    case 0
    {
      Serial.println("-----------------------------------------------");
      println("Assuming: no resistor connected...");
    }
    case 1
    {
      Serial.println("-----------------------------------------------");
      println("Assuming: resistor 1");
    }
    case 2
    {
      Serial.println("-----------------------------------------------");
      println("Assuming: resistor 2");
    }
    case 3
    {
      Serial.println("-----------------------------------------------");
      println("Assuming: resistor 3");
    }
    case 4
    {
      Serial.println("-----------------------------------------------");
      println("Assuming: resistor 4");
    }
  }
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

  Serial.println("Prepare to calibrate...");
  delay(1000);

  // Calibrate
  calibration();

  Serial.println("-----------------------------------------------");
}

void loop(){
  Serial.print("BUS VOLTAGE:   ");
  Serial.print(ina.readBusVoltage(), 5);
  Serial.println(" V");

  Serial.print("SHUNT VOLTAGE: ");
  Serial.print(ina.readShuntVoltage()*1000+2.5/1000000.0, 5);
  Serial.println(" mV");

  Serial.println("");
  delay(1000);
}
