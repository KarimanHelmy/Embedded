/*
 * SPI_driver.h
 *
 *  Author: Mohamed Abosreea , Kariman Helmy , Marwa Elbadawy
 */

#ifndef SPI_DRIVER_H_
#define SPI_DRIVER_H_


void SPI_init();

void SPI_write(char);

char SPI_read(void);

uint8_t SPI_transfer(uint8_t cData);

#endif /* SPI_DRIVER_H_ */