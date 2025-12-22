#include <stdio.h>

#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "hardware/dma.h"

#define SCK1 30
#define MI1 28
#define MO1 31

#define SPI_BAUD 24000000

int dma_tx = -1;
dma_channel_config dma_conf;
uint8_t *dma_buffer;

void SPI1_init() {
// Automatic with SPI1
// SCK clock - GPIO30 (SCK1) - SCK Pin 
// MO out - GPIO31 (TX1) - MO Pin

	spi_init(spi1, SPI_BAUD);
	gpio_set_function(SCK1, GPIO_FUNC_SPI);
	gpio_set_function(MO1, GPIO_FUNC_SPI);

	if (dma_tx == -1) {
		dma_tx = dma_claim_unused_channel(true);
		dma_conf = dma_channel_get_default_config(dma_tx);
		channel_config_set_transfer_data_size(&dma_conf, DMA_SIZE_8);
		channel_config_set_dreq(&dma_conf, spi_get_dreq(spi1, true));
		channel_config_set_read_increment(&dma_conf, true);
		channel_config_set_write_increment(&dma_conf, false);
	}

	return;
}

void SPI1_DMA_wait() {
printf("SPI1: Waiting for DMA...\n");
	if (dma_channel_is_busy(dma_tx))
		dma_channel_wait_for_finish_blocking(dma_tx);
printf("SPI1: DMA now free...\n");
}

void SPI1_DMA_set_buf(uint8_t *txbuf, size_t len) {
	dma_buffer = txbuf;
	dma_channel_configure(dma_tx, &dma_conf,
	                      &spi_get_hw(spi1)->dr,
	                      txbuf,
	                      len,
	                      false);
}

void SPI1_DMA_start_tx() {
printf("SPI1: Blocking if busy...\n");
	SPI1_DMA_wait();

printf("SPI1: Reset buffer address and start...\n");
	dma_channel_set_read_addr(dma_tx, dma_buffer, true);
}

void SPI1_write(uint8_t* buffer, size_t len) {
	SPI1_DMA_wait();
	spi_write_blocking(spi1, buffer, len);
}
