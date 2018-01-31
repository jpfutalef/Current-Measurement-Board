#include <stdio.h>

#include "xtimer.h"
#include "timex.h"
#include "periph/adc.h"
#include "periph/gpio.h"

#define BIT_R       12     // bit resolution of ADC
#if BIT_R == 8
  #define RES ADC_RES_12BIT
#elif BIT_R == 10
  #define RES ADC_RES_10BIT
#elif BIT_R == 12
  #define RES ADC_RES_12BIT
#endif

int MODE        = 1;     // Operation mode
int DELAY 	    =	2000;    // every DELAY milliseconds read a value
int SAMPLES_ADC = 256;     // take SAMPLES_ADC measures a get a mean value, comment to disable
int SAMPLES_CAL = 1024;    // amount of samples for calibration

// pins values
#define D2 GPIO_PIN(PORT_A,12)   // This pin enables MCU supply
#define D3 GPIO_PIN(PORT_B,0)
#define D4 GPIO_PIN(PORT_B,7)
#define D5 GPIO_PIN(PORT_B,6)
#define D6 GPIO_PIN(PORT_B,1)

gpio_t pins[5] = {D2,D3,D4,D5,D6};

// array to store calibration values
int CAL_VALS[5] = {0,0,0,0,0};

// pauses mil milliseconds
void pause(int mil)
{
  uint32_t now = xtimer_usec_from_ticks(xtimer_now());
  uint32_t before = now;
  while(now-before < mil*1000.0)
  {
	now = xtimer_usec_from_ticks(xtimer_now());
  }
}

// measure at specified line, with resolution RES and the specified amount of samples
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
  printf("CALIBRATIONXX");

  for(unsigned i = 0; i < sizeof(CAL_VALS)/sizeof(CAL_VALS[0]); i++)
  {
    if (i == 0)
    {
      //printf("All switches open\n");
      pause(10);

      CAL_VALS[i] = measure(ADC_LINE(0), RES, 1024);
    }
    else
    {
      gpio_set(pins[i]);
      //printf("Connected resistor %i\n", i+1 );
      pause(10);

      CAL_VALS[i] = measure(ADC_LINE(0), RES, 1024);

      //printf("V_CAL[%i]: %i\n", i, CAL_VALS[i]);

      gpio_clear(pins[i]);
    }
    printf("%i", CAL_VALS[i]);
  }

  printf("\n");
}

// Initialize GPIO pins as output
void pins_init(void)
{
  for (int i = 0; i < 5; i++)
  {
    gpio_init(pins[i], GPIO_OUT);
    if(i == 0)
    {
        gpio_set(D2); // blocks MCU power
    }
    else
    {
        gpio_clear(pins[i]);
    }
  }
}

// initialize ADC lines
int adc_lines_init(void)
{
  int sample = 0;
  // check if there are at least 3 ADC lines available.
  if (ADC_NUMOF < 2) {
    printf("Number of ADC lines available less than 3.\n");
    return 1;
  }

  else
  {
    // initialize ADC lines 0 to 1
    for (int i = 0; i < 3; i++)
    {
      if (adc_init(ADC_LINE(i)) < 0)
      {
          printf("Initialization of ADC_LINE(%u) failed\n", i);
          return 2;
      }

      else
      {
        printf("Successfully initialized ADC_LINE(%u)\n", i);
        // check if resolution applies
        sample = adc_sample(ADC_LINE(i), RES);
        if (sample < 0) {
            printf("ADC_LINE(%u): %i-bit resolution not applicable\n", i, BIT_R);
            return 3;
        }
      	else
      	{
      	    printf("ADC_LINE(%u) resolution: %i\n", i, BIT_R);
      	}
      }
    }
  }
  return 0;
}


int main(void)
{
  // Display message
  /*
  puts("Script for power measurement using RIOT's ADC peripherial\n");
  puts("and power measurement board.\n");
  puts("Make sure of connections:\n");
  puts("  - Supply connected to VBAT\n");
  puts("  - A0 connected to CURRENT LOW\n");
  puts("  - A1 connected to CURRENT HIGH\n");
  puts("  - A2 connected to ADC_VOLT\n");
  */

  // initialize gpio pins
  pins_init();

  // initialize adc lines
  adc_lines_init();

  // Make calibration
  calibrate();

  // Useful for data acquisition
  int measurement;

  // measure!
  gpio_set(D2); // enable MCU power

  while(1)
  {
    printf("READING");
    for (int i = 0; i < 3; i++)
    {
      measurement = measure(ADC_LINE(i), RES, SAMPLES_ADC);
      //printf("Measure from line[%i]: %i\n", i, measurement[i]);
      printf("XX%i", measurement);
    }
    printf("\n");
    pause(DELAY);
  }

return 0;

}
