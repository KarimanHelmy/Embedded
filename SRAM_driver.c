/*
 * SRAM.c
 *
 * Created: 10.09.2016 15:05:32
 *  Author: Whiskey Dicks
 */

#include <stdlib.h>
#include <stdint.h>
#include <util/delay.h>
#include "SRAM_driver.h"
#include "UART_driver.h"
#ifndef SRAM_FIRST_ADDRESS
#define SRAM_FIRST_ADDRESS (volatile char*)0x1800	//Start address fpr the SRAM
#endif

void SRAM_init() {
    MCUCR = (1 << SRE); // Enable SRAM i ATmega162
    SFIOR = (1 << XMM2); // Maskerer PC4-PC7 på ATmega162. Dvs passer på at vi ikke kuker med JTAG-pinnene. Sjekk "Table 4", side 32 i ATmega162-databladet.
}

int SRAM_write(uint16_t address, char data) {

    if (address > 0x7FF) {
        printf("SRAM error, trying to write to an address that is too big\n");
        return EXIT_FAILURE;
    }

    volatile char* ext_ram = SRAM_FIRST_ADDRESS; //Create a pointer to the array of all addresses we will write to. SRAM starting at 0x1800. ext_ram[0x7FF] is maximum because 0x1800 + 0x7FF = 0x1FFF! 
    ext_ram[address] = data;

    return 0;
}

char SRAM_read(uint16_t address) {

    if (address > 0x7FF) {
        printf("SRAM error, trying to read an address that is too big\r\n");
        return EXIT_FAILURE;
    }

    volatile char* ext_ram = SRAM_FIRST_ADDRESS;
    return ext_ram[address];

}

void SRAM_test(void) {

    volatile char *ext_ram = (char *) 0x1800; // Start address for the SRAM
    uint16_t ext_ram_size = 0x800;
    uint16_t write_errors = 0;
    uint16_t retrieval_errors = 0;
    printf("Starting SRAM test...\r\n");
    // rand() stores some internal state, so calling this function in a loop will
    // yield different seeds each time (unless srand() is called before this function)
    uint16_t seed = rand();
    // Write phase: Immediately check that the correct value was stored
    srand(seed);
    for (uint16_t i = 0; i < ext_ram_size; i++) {
        uint8_t some_value = rand();
        //_delay_ms(5);
        ext_ram[i] = some_value;
        //_delay_ms(5);
        uint8_t retreived_value = ext_ram[i];
        //_delay_ms(5);
        if (retreived_value != some_value) {
            printf("Write phase error: ext_ram[%4d] = %02X (should be %02X)\r\n", i, retreived_value, some_value);
            write_errors++;
        }
    }
    printf("Write errorrs ...%d\r\n", write_errors);
    // Retrieval phase: Check that no values were changed during or after the write phase
    srand(seed); // reset the PRNG to the state it had before the write phase
    for (uint16_t i = 0; i < ext_ram_size; i++) {
        uint8_t some_value = rand();
        //_delay_ms(5);
        uint8_t retreived_value = ext_ram[i];
        //_delay_ms(5);
        if (retreived_value != some_value) {
            printf("Retrieval phase error: ext_ram[%4d] = %02X (should be %02X)\r\n", i, retreived_value, some_value);
            retrieval_errors++;
        }
    }
    printf("retrieval_errors  ...%d", retrieval_errors);
    //retrieval_errors++;
 }  