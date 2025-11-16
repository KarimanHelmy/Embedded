/*This is SRAM function*/


#include <stdlib.h>
#include <stdint.h>
#include <util/delay.h>
#include "SRAM_driver.h"
#include "UART_driver.h"
#ifndef SRAM_FIRST_ADDRESS
#define SRAM_FIRST_ADDRESS (volatile char*)0x1800	//Start address fpr the SRAM
#endif

    //Enable SRAM and masking the JTAG pins so it doesn't interfere with the JTAG pins 
void SRAM_init() {
    MCUCR = (1 << SRE); 
    SFIOR = (1 << XMM2); 
}
// Pointer to array containing all addresses which we can write in 
int SRAM_write(uint16_t address, char data) {

    if (address > 0x7FF) {
        printf("error trying to write in an address\n");
        return EXIT_FAILURE;
    }

    volatile char* ext_ram = SRAM_FIRST_ADDRESS;  
    ext_ram[address] = data;

    return 0;
}
//read an address
char SRAM_read(uint16_t address) {

    if (address > 0x7FF) {
        printf("SRAM error, trying to read an address that is too big\r\n");
        return EXIT_FAILURE;
    }

    volatile char* ext_ram = SRAM_FIRST_ADDRESS;
    return ext_ram[address];

}
//TEST the Sram is working, and check if there is any error during read.write cycle 
void SRAM_test(void) {

    volatile char *ext_ram = (char *) 0x1800; // Start address for the SRAM
    uint16_t ext_ram_size = 0x800;
    uint16_t write_errors = 0;
    uint16_t retrieval_errors = 0;
    printf("Starting SRAM test...\r\n");
 
    uint16_t seed = rand();
    srand(seed);
    for (uint16_t i = 0; i < ext_ram_size; i++) {
        uint8_t some_value = rand();
   
        ext_ram[i] = some_value;
 
        uint8_t retreived_value = ext_ram[i];
  
        if (retreived_value != some_value) {
            printf("Write  error: ext_ram[%4d] = %02X (should be %02X)\r\n", i, retreived_value, some_value);
            write_errors++;
        }
    }
    printf("Write errorrs ...%d\r\n", write_errors);
    
    srand(seed);
    for (uint16_t i = 0; i < ext_ram_size; i++) {
        uint8_t some_value = rand();

        uint8_t retreived_value = ext_ram[i];

        if (retreived_value != some_value) {
            printf("Retrieval  error: ext_ram[%4d] = %02X (should be %02X)\r\n", i, retreived_value, some_value);
            retrieval_errors++;
        }
    }
    printf("retrieval_errors  ...%d", retrieval_errors);
    

 }  

