
// This is not a complete header file, but it is intendet to be combined with readMCP3008.c
// Using the header file would require us to add the readMCP3008.c file to the compiler(as with the bcm3835.c file).
void MCP3008_init (uint8_t CS)
unsigned short adcRead(unsigned short adc_number) // This have to be transformed to a void fcn