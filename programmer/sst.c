// File: sst.c
// Author: Jarielle Catbagan
// Created: 30, May 2014

#include <avr/io.h>
#include "sst.h"
#include "shiftreg.h"

void sst_init(void) {
        SST_CTRL_DDR1 |= (1 << SST_CTRL_WE | 1 << SST_CTRL_OE);
        SST_CTRL_PORT1 |= (1 << SST_CTRL_WE | 1 << SST_CTRL_OE);
        
        SST_CTRL_DDR2 |= 1 << SST_CTRL_CE;
        SST_CTRL_PORT2 |= 1 << SST_CTRL_CE;

        SST_DATA_DDR1 |= (1 << SST_DATA_D0 | 1 << SST_DATA_D1 | 1 << SST_DATA_D2 |
                          1 << SST_DATA_D3 | 1 << SST_DATA_D4 | 1 << SST_DATA_D5);
        SST_DATA_DDR2 |= (1 << SST_DATA_D6 | 1 << SST_DATA_D7);

        SST_DATA_PORT1 &= ~(1 << SST_DATA_D0 | 1 << SST_DATA_D1 | 1 << SST_DATA_D2 |
                            1 << SST_DATA_D3 | 1 << SST_DATA_D4 | 1 << SST_DATA_D5);
        SST_DATA_PORT2 &= ~(1 << SST_DATA_D6 | 1 << SST_DATA_D7);
}

void sst_address(uint16_t address) {
        shift_16bits(address);
}

void sst_data(uint8_t data) {
        SST_DATA_PORT1 &= 0xC0;
        SST_DATA_PORT1 |= (data & 0x3F);
        SST_DATA_PORT2 &= 0xFC;
        SST_DATA_PORT2 |= (data >> 6) & 0x03;
}

void sst_CEhigh(void) {
        SST_CTRL_PORT2 |= 1 << SST_CTRL_CE;
}
void sst_CElow(void) {
        SST_CTRL_PORT2 &= ~(1 << SST_CTRL_CE);        
}

void sst_WEhigh(void) {
        SST_CTRL_PORT1 |= 1 << SST_CTRL_WE;
}

void sst_WElow(void) {
        SST_CTRL_PORT1 &= ~(1 << SST_CTRL_WE);
}

void sst_OEhigh(void) {
        SST_CTRL_PORT1 |= 1 << SST_CTRL_OE;
}

void sst_OElow(void) {
        SST_CTRL_PORT1 &= ~(1 << SST_CTRL_OE);
}

void sst_clearaddr(void) {
        shift_16bits(0x0000);
}

void sst_cleardata(void) {
        SST_DATA_PORT1 &= 0xC0;
        SST_DATA_PORT2 &= 0xFC;
}

void sst_datapoll(uint8_t val_tocmp) {
        uint8_t lval;

        sst_setdatain();
        
        do {
                sst_CElow();
                sst_OElow();
                _delay_us(1);
                lval = SST_DATA_PIN2 & (1 << SST_DATA_D7);
                sst_CEhigh();
                _delay_us(1);
                sst_OEhigh();
                _delay_us(1);
        } while(lval != (val_tocmp & 0x80));

        sst_setdataout();
}

void sst_setdatain(void) {
        SST_DATA_DDR1 &= ~(1 << SST_DATA_D0 | 1 << SST_DATA_D1 | 1 << SST_DATA_D2 |
                           1 << SST_DATA_D3 | 1 << SST_DATA_D4 | 1 << SST_DATA_D5);
        SST_DATA_DDR2 &= ~(1 << SST_DATA_D6 | 1 << SST_DATA_D7);
        
        SST_DATA_PORT1 &= ~(1 << SST_DATA_D0 | 1 << SST_DATA_D1 | 1 << SST_DATA_D2 |
                            1 << SST_DATA_D3 | 1 << SST_DATA_D4 | 1 << SST_DATA_D5);
        SST_DATA_PORT2 &= ~(1 << SST_DATA_D6 | 1 << SST_DATA_D7);
}

void sst_setdataout(void) {
        SST_DATA_DDR1 |= (1 << SST_DATA_D0 | 1 << SST_DATA_D1 | 1 << SST_DATA_D2 |
                          1 << SST_DATA_D3 | 1 << SST_DATA_D4 | 1 << SST_DATA_D5);
        SST_DATA_DDR2 |= (1 << SST_DATA_D6 | 1 << SST_DATA_D7);

        SST_DATA_PORT1 &= ~(1 << SST_DATA_D0 | 1 << SST_DATA_D1 | 1 << SST_DATA_D2 |
                            1 << SST_DATA_D3 | 1 << SST_DATA_D4 | 1 << SST_DATA_D5);
        SST_DATA_PORT2 &= ~(1 << SST_DATA_D6 | 1 << SST_DATA_D7);
}
