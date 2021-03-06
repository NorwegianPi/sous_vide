// Vorking read from adc
// Original file c_code/spi3.c
// sudo rmate jarle_spi2.c.c
// Compiling:
// gcc -o jarle_spi2 -l rt jarle_spi2.c -l bcm2835


#include <stdio.h>
#include <bcm2835.h>

// From python:
// send [192, 0, 0] = 11000000 0 0
//#define ch0_r       0b0000000110000000
#define ch0_r       0b1100000000000000  // leser av In1 = in2 = 255 tot = 511
//#define ch0_r       0b0011000000000000 // Leser av 127 ved 3.3v etter kjoring av adafruit mcp3008 python 
#define WORD_SIZE   2U

void makeWord(char eightBits[], unsigned short sixteenBits)
{
  eightBits[0] = sixteenBits >> 8;
  eightBits[1] = sixteenBits & 0x00FF; // & 0x00FF creates a mask of the lower eight bits. e.g. only read the lower 8 bits.
}

// Backconverting word to number :) 
unsigned short makeNum(char eightBits[])
{
  unsigned short sixteenBits;
  sixteenBits = (eightBits[0] << 8) | eightBits[1];
  return sixteenBits;
}

int main(int argc, char **argv)
{

  if (!bcm2835_init())
  {
    return 1;
  }
bcm2835_spi_begin();
bcm2835_spi_setBitOrder(BCM2835_SPI_BIT_ORDER_MSBFIRST);      
bcm2835_spi_setDataMode(BCM2835_SPI_MODE0); //Data comes in on falling edge                
bcm2835_spi_setClockDivider(BCM2835_SPI_CLOCK_DIVIDER_256); //250MHz / 256 = 976.5kHz 
bcm2835_spi_chipSelect(BCM2835_SPI_CS0); //Slave Select on CS0                    
bcm2835_spi_setChipSelectPolarity(BCM2835_SPI_CS0, LOW);   
 
// Tester dette------------
char spiIn[2];
char spiOut[2];
makeWord(spiOut, ch0_r);
//bcm2835_spi_transfern(spiOut, WORD_SIZE); // Only sending data
bcm2835_spi_transfernb(spiOut, spiIn, WORD_SIZE);
//unsigned char result = spiOut;
printf("Sender\n");
printf("Out1: %u\n", spiOut[0]);
printf("Out2: %u\n", spiOut[1]);
printf("Mottar\n");
printf("In1: %u\n", spiIn[0]);
printf("In2: %u\n", spiIn[1]);


unsigned short ReturnValue = makeNum(spiIn);
printf("Input bit: %u\n", ch0_r); 
printf("Return bit: %u\n", ReturnValue);
 // -------------------------
 
 /*
  uint8_t send_data = 0x01;
  char eightBits[0] = send_data;
  uint8_t dumData = bcm2835_spi_transfer(send_data);
    printf("%i\n", send_data);
  send_data = 0x80;
  eightBits[1] = send_data;
  printf("%i\n", send_data);
  uint8_t msb = bcm2835_spi_transfer(send_data);
  int msbRead = msb & 0b00000011

  send_data = 0;
  eightBits[2] = send_data;    
  //uint8_t lsb = bcm2835_spi_transfer(send_data);

  int adcRead = (msbRead << 8) | lsb;

  printf("%d\n", adcRead);
  printf("%d\n", msb);
  printf("%d\n", msbRead);
  printf("%d\n", lsb);
  printf("%d\n", dumbData);
*/
bcm2835_spi_end();
bcm2835_close();
return 0;

}