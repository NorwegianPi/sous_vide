/*==========================================================================
FILENAME  : sous_vide_0_0.c
AUTHOR    : Jarle Horneland 
DATE      : 10-Jun-2018 17:39 
REVISION  : 0.00 (10-Jun-2018 17:39) 
Path      : cd git_projects/sous_vide
Compiling:  gcc -o sous -l rt sous_vide_0_0.c -l bcm2835
            sudo ./sous
*==========================================================================*/

#include <stdio.h>
#include <bcm2835.h>
#include "readMCP3008.c"

#define adc_number   2
int main(int argc, char **argv)
{

  if (!bcm2835_init())
  {
    return 1;
  }
  MCP3008_init(BCM2835_SPI_CS0);

  unsigned short value = adcRead(adc_number);
  printf("Return Value: %u\n", value);

  bcm2835_spi_end();
  bcm2835_close();
  return 0;

}