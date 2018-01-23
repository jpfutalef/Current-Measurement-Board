#include <stdio.h>
#include <math.h>

#include "xtimer.h"
#include "timex.h"
#include "periph/adc.h"

#define BIT_R           12      // bit resolution of ADC
#define ADC_POS_REF	    3.3     // positive voltage reference of ADC
#define SHUNT_VAL       4.7    // Shunt resistor value in ohms
#define DELAY 		      2000    // every DELAY milliseconds read a value
#define SAMPLES_ADC     10     // take SAMPLES_ADC measures a get a mean value, comment to disable

// calibration resistors and values
const int CAL_RES[4] = {1000,10000,100000,1000000};
int CAL_VAL[4] = {0,0,0,0};

// stage gains
#define FIRST_STAGE_GAIN    10.0
#define LOW_CURRENT_STAGE   19.7
#define HIGH_CURRENT_STAGE  1.51
#define SUP_VOLTAGE_STAGE   2.0

#if BIT_R == 8
  #define RES ADC_RES_12BIT
#elif BIT_R == 10
  #define RES ADC_RES_10BIT
#elif BIT_R == 12
  #define RES ADC_RES_12BIT
#endif

void pause(int mil)
{
  uint32_t now = xtimer_usec_from_ticks(xtimer_now());
  uint32_t before = now;
  while(now-before < mil*1000.0)
  {
	now = xtimer_usec_from_ticks(xtimer_now());
  }
}

uint32_t measure(adc_t line, adc_res_t RES, int samples)
{
  uint32_t sum = 0;

  for (int i = 0; i < samples; i++) {
    sum = adc_sample(line, RES) + sum;
  }
  return sum/samples;
}

void calibrate(void)
{
  puts("Calibrating...\n");

  int sample[3];

  for(unsigned i = 0; i < sizeof(CAL_RES)/sizeof(CAL_RES[0]); i++)
  {
    #ifdef SAMPLES_ADC
      sample[0] = measure(ADC_LINE(0), RES, SAMPLES_ADC);
      sample[1] = measure(ADC_LINE(1), RES, SAMPLES_ADC);
    #else
      sample[0] = adc_sample(ADC_LINE(0), RES);
      sample[1] = adc_sample(ADC_LINE(1), RES);
    #endif

    printf("1: %i\n", sample[0]);
    printf("2: %i\n", sample[1]);
  }
}

int main(void)
{
    puts("TScript for power measurement using RIOT's ADC peripherial\n");
    puts("and power measurement board.\n");
    puts("Make sure of connections:\n");
    puts("  - Supply connected to VBAT\n");
    puts("  - A0 connected to CURRENT LOW\n");
    puts("  - A1 connected to CURRENT HIGH\n");
    puts("  - A2 connected to ADC_VOLT\n");

    pause(2000);

    int sample = 0;

    // check if there are at least 3 ADC lines available.
    if (ADC_NUMOF < 2) {
      printf("Number of ADC lines available less than 3.\n");
    }

    else
    {
      // initialize ADC lines 0 to 1
      for (int i = 0; i < 3; i++) {
          if (adc_init(ADC_LINE(i)) < 0) {
              printf("Initialization of ADC_LINE(%u) failed\n", i);
              return 1;
          } else {
              printf("Successfully initialized ADC_LINE(%u)\n", i);
          }
      }

      // check if resolution applies to every line
      for (int i = 0; i < 3; i++)
      {
        sample = adc_sample(ADC_LINE(i), RES);
        if (sample < 0) {
            printf("ADC_LINE(%u): %i-bit resolution not applicable\n", i, BIT_R);
            return 0;
        }
      	else
      	{
      	    printf("ADC_LINE(%u) resolution: %i\n", i, BIT_R);
      	}
      }

      // Make calibration
      calibrate();

      // Useful values for data acqusition
      float measurement[3];
      float shuntVoltageLow, shuntVoltageHigh, supplyVoltage;
      char* v[3];
      v[0] = "Low current";
      v[1] = "High Current";
      v[2] = "Supply voltage";

      // display to user if multiple samples mean is enabled
      #ifdef SAMPLES_ADC
        printf("Samples mean enabled with %i samples.\n\n", SAMPLES_ADC);
      #else
        printf("Samples mean NOT enabled.\n\n");
      #endif

      // measure!
      while(1)
      {
        for (unsigned i = 0; i < 3; i++)
        {
          #ifdef SAMPLES_ADC
            sample = measure(ADC_LINE(i), RES, SAMPLES_ADC);
          #else
            sample = adc_sample(ADC_LINE(i), RES);
          #endif

          measurement[i] = (float)sample*ADC_POS_REF/pow(2.0, BIT_R);
	        // printf("Measure ADC_LINE(%i):  %u\n", i, sample);
          printf("%s ADC sample: %.4f [V]\n", v[i], measurement[i]);
        }

        shuntVoltageLow = measurement[0]*FIRST_STAGE_GAIN*LOW_CURRENT_STAGE;
        shuntVoltageHigh = measurement[1]*FIRST_STAGE_GAIN*HIGH_CURRENT_STAGE;
        supplyVoltage = measurement[2]*SUP_VOLTAGE_STAGE;

        printf("Shunt voltaje low: %.4f [mV]\n", shuntVoltageLow*1000.0);
        printf("Shunt voltage high: %.4f [mV]\n", shuntVoltageHigh*1000.0);
        printf("Supply voltage: %.4f [V]\n\n", supplyVoltage);
        pause(DELAY);
      }
    }

    return 0;
}
