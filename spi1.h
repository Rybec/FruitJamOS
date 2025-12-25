#ifndef SPI1_H
#define SPI1_H

void SPI1_init();
void SPI1_write(uint8_t* buffer, size_t len);
void SPI1_DMA_set_buf(uint8_t *txbuf, size_t len);
void SPI1_DMA_wait();
void SPI1_DMA_start_tx();

#endif
