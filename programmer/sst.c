// File: sst.c
// Created: 30, May 2014

/* Copyright (C) 2014 Jarielle Catbagan
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program. If not, see <http://www.gnu.org/licenses/>.
*
*/

#include <avr/io.h>
#define F_CPU 20000000UL
#include <util/delay.h>
#include "sst.h"
#include "shiftreg.h"

void sst_init(void) {
        /* Set WE and OE control pins to output. */
        SST_CTRL_DDR1 |= (1 << SST_CTRL_WE | 1 << SST_CTRL_OE);
        /* Initialize WE and OE control pins to 1 (high) since these control pins
           are active low. */
        SST_CTRL_PORT1 |= (1 << SST_CTRL_WE | 1 << SST_CTRL_OE);

        /* Set CE control pin to output. */
        SST_CTRL_DDR2 |= 1 << SST_CTRL_CE;
        /* Initialize CE control pin to 1 (high) since this control pin is active low. */
        SST_CTRL_PORT2 |= 1 << SST_CTRL_CE;

        /* Set the default direction of the data pins to output. */
        SST_DATA_DDR1 |= (1 << SST_DATA_D0 | 1 << SST_DATA_D1 | 1 << SST_DATA_D2 |
                          1 << SST_DATA_D3 | 1 << SST_DATA_D4 | 1 << SST_DATA_D5);
        SST_DATA_DDR2 |= (1 << SST_DATA_D6 | 1 << SST_DATA_D7);

        /* Initialize all data pins to 0 (off/low). */
        SST_DATA_PORT1 &= ~(1 << SST_DATA_D0 | 1 << SST_DATA_D1 | 1 << SST_DATA_D2 |
                            1 << SST_DATA_D3 | 1 << SST_DATA_D4 | 1 << SST_DATA_D5);
        SST_DATA_PORT2 &= ~(1 << SST_DATA_D6 | 1 << SST_DATA_D7);
}

void sst_address(uint16_t address) {
        shift_16bits(address);
}

void sst_data(uint8_t data) {
        sst_setdataout();
        /* Since the data port is currently split up into two different ports where
           data bits 5:0 are in pins C5:C0 and data bits 7:6 are in pins B1:B0, the data
           has to be masked and outputed to appropriate location. */
        
        /* Set the C5:C0 to '0' while masking C7:C6. */
        SST_DATA_PORT1 &= 0xC0;
        /* Mask the data area at interest and output it to the appropriate location. */
        SST_DATA_PORT1 |= (data & 0x3F);

        /* Set B1:B0 to '0' while masking B7:B2. */
        SST_DATA_PORT2 &= 0xFC;
        /* As before, mask the data and output it to the appropriate location. */
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
        enum loop_status_t {EXIT = 0, CONTINUE} loop_status;

        sst_datapollbit_in();
        loop_status = CONTINUE;

        if(0x80 & val_tocmp) {
                do {
                        sst_CElow();
                        _delay_us(1);
                        sst_OElow();
                        _delay_us(1);
                        if(bit_is_set(PINB, 1))
                                loop_status = EXIT;
                        _delay_us(5);
                        sst_OEhigh();
                        _delay_us(1);
                        sst_CEhigh();
                        _delay_us(1);
                } while(loop_status == CONTINUE);
        }
        else {
                do {
                        sst_CElow();
                        _delay_us(1);
                        sst_OElow();
                        _delay_us(1);
                        if(bit_is_clear(PINB, 1))
                                loop_status = EXIT;
                        _delay_us(5);
                        sst_OEhigh();
                        _delay_us(1);
                        sst_CEhigh();
                        _delay_us(1);
                } while (loop_status == CONTINUE);
        }

        sst_datapollbit_out();
}

void sst_setdatain(void) {
        /* Set the data pins to input. */
        SST_DATA_DDR1 &= ~(1 << SST_DATA_D0 | 1 << SST_DATA_D1 | 1 << SST_DATA_D2 |
                           1 << SST_DATA_D3 | 1 << SST_DATA_D4 | 1 << SST_DATA_D5);
        SST_DATA_DDR2 &= ~(1 << SST_DATA_D6 | 1 << SST_DATA_D7);

        /* Ensure that the internal pull-up resistors are not activated. */
        SST_DATA_PORT1 &= ~(1 << SST_DATA_D0 | 1 << SST_DATA_D1 | 1 << SST_DATA_D2 |
                            1 << SST_DATA_D3 | 1 << SST_DATA_D4 | 1 << SST_DATA_D5);
        SST_DATA_PORT2 &= ~(1 << SST_DATA_D6 | 1 << SST_DATA_D7);
}

void sst_setdataout(void) {
        /* Set the data pins to output. */
        SST_DATA_DDR1 |= (1 << SST_DATA_D0 | 1 << SST_DATA_D1 | 1 << SST_DATA_D2 |
                          1 << SST_DATA_D3 | 1 << SST_DATA_D4 | 1 << SST_DATA_D5);
        SST_DATA_DDR2 |= (1 << SST_DATA_D6 | 1 << SST_DATA_D7);

        /* Initialize the data pins to '0' (off/low). */
        SST_DATA_PORT1 &= ~(1 << SST_DATA_D0 | 1 << SST_DATA_D1 | 1 << SST_DATA_D2 |
                            1 << SST_DATA_D3 | 1 << SST_DATA_D4 | 1 << SST_DATA_D5);
        SST_DATA_PORT2 &= ~(1 << SST_DATA_D6 | 1 << SST_DATA_D7);
}

void sst_datapollbit_in(void) {
        SST_DATA_DDR2 &= ~(1 << SST_DATA_D7);
        SST_DATA_PORT2 &= ~(1 << SST_DATA_D7);
}

void sst_datapollbit_out(void) {
        SST_DATA_DDR2 |= 1 << SST_DATA_D7;
        SST_DATA_PORT2 &= ~(1 << SST_DATA_D7);
}

void sst_wait(uint16_t address, uint8_t data) {
        uint8_t lval;
        sst_setdatain();

        sst_address(address);
                
        do {
                sst_WEhigh();
                sst_CElow();
                _delay_us(1);
                sst_OElow();
                _delay_us(1);

                lval = SST_DATA_PIN1;
                lval &= 0x3F;
                lval |= (SST_DATA_PIN2 << 6) & 0xC0;

                _delay_us(5);
                sst_OEhigh();
                sst_CEhigh();
        } while(lval != data);
        
        sst_setdataout();
}
