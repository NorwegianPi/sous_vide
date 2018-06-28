/*==========================================================================
FILENAME  : readMCP3008.c
AUTHOR    : Jarle Horneland 
DATE      : 10-Jun-2018 17:39 
REVISION  : 1.00 (10-Jun-2018 17:39)
Compiling:  gcc -o spi3 -l rt spi3.c -l bcm2835
            sudo ./spi3

Opening in VS Code: sudo rmate spi3.c.c
*==========================================================================*/

//#include <stdio.h>
//#include <bcm2835.h>
#define WORD_SIZE   3U

void MCP3008_init (uint8_t CS)
{
  // CS0 = BCM2835_SPI_CS0
  // CS1 = BCM2835_SPI_CS1
  bcm2835_spi_begin();
  bcm2835_spi_setBitOrder(BCM2835_SPI_BIT_ORDER_MSBFIRST);      
  bcm2835_spi_setDataMode(BCM2835_SPI_MODE0); //Data comes in on falling edge                
  bcm2835_spi_setClockDivider(BCM2835_SPI_CLOCK_DIVIDER_256); //250MHz / 256 = 976.5kHz 
  bcm2835_spi_chipSelect(CS); //Slave Select on CS0                    
  bcm2835_spi_setChipSelectPolarity(CS, LOW);

}

unsigned short adcRead(unsigned short adc_number)
{
  unsigned short value;

  char spiIn[3];
  char spiOut[3];

  spiOut[0] = 0b00000001; //Start bit
  spiOut[1] = (0b1 << 7) | (adc_number << 4);
  spiOut[2] = 0b00000000; //Dont care

  bcm2835_spi_transfernb(spiOut, spiIn, WORD_SIZE);

  value = (spiIn[1] & 0x3) << 8 | spiIn[2];
  return value;
}

/* Old methods for converting values from adc
void adcChannel(char spiOut[], int adc_number)
{
  spiOut[0] = 0b00000001; //Start bit
  spiOut[1] = (0b1 << 7) | (adc_number << 4);
  spiOut[2] = 0b00000000; //Dont care
}

unsigned short adcReturn(char returnBits[])
{
  unsigned short returnValue;
  returnValue = (returnBits[1] & 0x3) << 8 | returnBits[2];
  return returnValue;
}
*/

/*
// Example of a simple main function.
int main(int argc, char **argv)
{

  if (!bcm2835_init())
  {
    return 1;
  }
  MCP3008_init(BCM2835_SPI_CS0);

  int adc_number = 0;
  unsigned short value = adcRead(1);
  printf("Return Value: %u\n", value);

  bcm2835_spi_end();
  bcm2835_close();
  return 0;

}
*/
