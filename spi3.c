// Vorking read from adc
// Original file c_code/spi3.c
// sudo rmate spi3.c.c
// Compiling:
// gcc -o spi3 -l rt spi3.c -l bcm2835
// sudo ./spi3

 
#include <stdio.h>
#include <bcm2835.h>

// From python:
// send [192, 0, 0] = 11000000 0 0

#define ch0_r       0b1100000000000000  // leser av In1 = in2 = 255 tot = 511
#define ch1_r       0b1000000000000000  // leser av In1 = in2 = 255 tot = 511
#define ch0_adr     0b1100000000  // leser av In1 = in2 = 255 tot = 511
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
  sixteenBits = (eightBits[0] << 9) | eightBits[1]<< 1;
  return sixteenBits;
}

// Backconverting word to number :) 
unsigned short makeNum2(char eightBits[])
{
  unsigned short sixteenBits;
  //sixteenBits = (eightBits[0] << 8) | eightBits[1];
  sixteenBits = (eightBits[0] & 0x01) << 9;  //# first bits of 10 bits when 1
  sixteenBits |= (eightBits[1] & 0xFF) << 1; //# the next nine bits

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
makeWord(spiOut, ch1_r);
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


/*
// Testing of possible ways to determine adc channel
int adc_number = 1;
int command = 0b11 << 8; // 11 and then 8 zeros
printf("Command: %i\n",command);
command |= (adc_number & 0x07) << 5;
printf("Command: %i\n",command);
// Need to add six zeros 
command = command << 0b11;

printf("Command: %i\n",command);
printf("Channel adress: %u\n", ch0_adr); 
*/



//
bcm2835_spi_end();
bcm2835_close();
return 0;

}