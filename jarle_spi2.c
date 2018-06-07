// spaz_spi.c
//
// Jarle Horneland 21.04.18 
// Working on SPI vith ADC

// Compiling:
// gcc -o jarle_spi -l rt jarle_spi.c -l bcm2835 

#include <bcm2835.h>
#include <stdio.h>

// Original config from spazTech video 16 bit words
/*
#define GPIOCONFIG  0b0100000000000111
#define ADCCONFIG   0b0010000000001000  
#define DACCONFIG   0b0010100001000000
#define GPCONFIG    0b0001100000000000
#define ADCSEQUENCE 0b0001001000001000
#define ONE         0b0100100000000001
#define TWO         0b0100100000000010
#define THREE       0b0100100000000011
#define FOUR        0b0100100000000100
#define FIVE        0b0100100000000101
#define SIX         0b0100100000000110
#define SEVEN       0b0100100000000111
#define WORD_SIZE   2U
*/

#define GPIOCONFIG  0b0100000000000111
#define ADCCONFIG   0b0010000000001000  
#define DACCONFIG   0b0010100001000000
#define GPCONFIG    0b0001100000000000
#define ADCSEQUENCE 0b0001001000001000
#define ONE         0b0100100000000001
#define TWO         0b0100100000000010
#define THREE       0b0100100000000011
#define FOUR        0b0100100000000100
#define FIVE        0b0100100000000101
#define SIX         0b0100100000000110
#define SEVEN       0b0100100000000111
#define WORD_SIZE   2U

// make word to 16 bit device from two eight bit words
void makeWord(char eightBits[], unsigned short sixteenBits)
{
    eightBits[0] = sixteenBits >> 8;
    eightBits[1] = sixteenBits & 0x00FF; // & 0x00FF creates a mask of the lower eight bits. e.g. only read the lower 8 bits.
}

int main(int arc, char **argv)
{
	
    // Creates an array that sets up the device
    unsigned short config[] = 
    {
        GPIOCONFIG,
        ADCCONFIG,
        DACCONFIG,
        GPCONFIG,
        ADCSEQUENCE,
    };
	

    // Two element arrays containing data we are passing to SPI module 
    char spiOut[2];
    char spiIn[2];


    // Decimal value of voltage signal 2^12 = 4095 = 3.3V
	unsigned short dacData = 1000;
    dacData = 0b1110000000000000 | dacData; //Tells pi that it is DA->AN conv end what pin to use
                    // 0b D/A command[1] pin6[110] data [000000000000] 


    if(!bcm2835_init()) return 1;

    bcm2835_spi_begin();
    bcm2835_spi_setBitOrder(BCM2835_SPI_BIT_ORDER_MSBFIRST);      // The default
    bcm2835_spi_setDataMode(BCM2835_SPI_MODE0);                   // The default maybe MODE1?
    bcm2835_spi_setClockDivider(BCM2835_SPI_CLOCK_DIVIDER_64); 	  // 4MHz clock with _64 or _16?
    bcm2835_spi_chipSelect(BCM2835_SPI_CS0);                      // The default
    bcm2835_spi_setChipSelectPolarity(BCM2835_SPI_CS0, LOW);      // the default

    //for(int j = 0 ; j < sizeof(config) ; j++)
    //{
    //    makeWord(spiOut, config[j]);
    //    bcm2835_spi_transfern(spiOut, WORD_SIZE); // Only sending data
    //}
    int m = 0;
    while(m < 1000)
    {

        spiOut[0] = dacData>>8;
        spiOut[1] = dacData & 0x00FF;
        //bcm2835_spi_transfernb(spiOut, spiIn, WORD_SIZE);
        printf("spi: %i \n",spiOut );
        m++;
    }
}