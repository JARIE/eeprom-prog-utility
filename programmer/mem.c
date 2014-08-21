// File: mem.c
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
#include "mem.h"
#include "sst.h"
#include "shiftreg.h"

void mem_init(void) {
        sst_init();
        shiftreg_init();
}

void mem_write(uint16_t address, uint8_t data) {
        uint8_t lval1, lval2;

        sst_setdataout();
        
        sst_OEhigh();
        sst_CElow();

        /* First byte of three-byte write-sequence initialization. */
        sst_address(0x5555);
        sst_data(0xAA);
        sst_WElow();
        _delay_us(1);
        sst_WEhigh();

        /* Second byte of three-byte write-sequence initialization. */
        sst_address(0x2AAA);
        sst_data(0x55);
        sst_WElow();
        _delay_us(1);
        sst_WEhigh();

        /* Third (last) byte of three-byte write-sequence initialization. */
        sst_address(0x5555);
        sst_data(0xA0);
        sst_WElow();
        _delay_us(1);
        sst_WEhigh();

        sst_address(address);
        sst_data(data);
        sst_WElow();
        _delay_us(1);
        sst_WEhigh();

        _delay_us(30);
        sst_CEhigh();
        _delay_us(1);
                
        sst_clearaddr(); 
        sst_cleardata();
}

uint8_t mem_read(uint16_t address) {
        uint8_t data;

        sst_setdatain();
        
        sst_WEhigh();
        sst_address(address);
        sst_CElow();
        _delay_us(1);
        sst_OElow();
        _delay_us(1);

        data = SST_DATA_PIN1;
        data &= 0x3F;
        data |= (SST_DATA_PIN2 << 6) & 0xC0;

        _delay_us(5);
        sst_OEhigh();
        sst_CEhigh();

        return data;
}

void mem_erase(void) {
        uint8_t data;
        
        sst_setdataout();

        sst_OEhigh();
        sst_CElow();

        /* First byte of six-byte erase-sequence initialization. */
        sst_address(0x5555);
        sst_data(0xAA);
        sst_WElow();
        _delay_us(1);
        sst_WEhigh();

        /* Second byte of six-byte erase-sequence initializeation. */
        sst_address(0x2AAA);
        sst_data(0x55);
        sst_WElow();
        _delay_us(1);
        sst_WEhigh();

        /* Third byte of six-byte erase-sequence initialization. */
        sst_address(0x5555);
        sst_data(0x80);
        sst_WElow();
        _delay_us(1);
        sst_WEhigh();

        /* Fourth byte of six-byte erase-sequence initialization. */
        sst_address(0x5555);
        sst_data(0xAA);
        sst_WElow();
        _delay_us(1);
        sst_WEhigh();

        /* Fifth byte of six-byte erase-sequence initializeation. */
        sst_address(0x2AAA);
        sst_data(0x55);
        sst_WElow();
        _delay_us(1);
        sst_WEhigh();

        /* Sixth (last) byte of six-byte erase-sequence initialization. */
        sst_address(0x5555);
        sst_data(0x10);
        sst_WElow();
        _delay_us(1);
        sst_WEhigh();

        _delay_us(5);
        data = 0x80;

        sst_CEhigh();
        _delay_us(1);

        sst_datapoll(data);

        sst_clearaddr();
        sst_cleardata();
}
 
