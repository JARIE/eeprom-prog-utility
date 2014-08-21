// File: shiftreg.c
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
#include "shiftreg.h"

void shiftreg_init(void) {
        SR_DDR |= (1 << SR_DATA | 1 << SR_LATCH | 1 << SR_CLOCK | 1 << SR_CLEAR);
        SR_PORT &= ~(1 << SR_DATA | 1 << SR_LATCH | 1 << SR_CLOCK);
        SR_PORT |= 1 << SR_CLEAR;
}

void shift_8bits(uint8_t value) {
        uint8_t bitmask = 0b10000000, lval;
        int i;

        SR_PORT &= ~(1 << SR_CLEAR);
        SR_PORT |= 1 << SR_CLEAR;
        
        for(i = 0; i < 8; ++i) {
                lval = value & bitmask;
                if(lval == bitmask) {
                        SR_PORT |= 1 << SR_DATA;
                        SR_PORT |= 1 << SR_CLOCK;
                        SR_PORT &= ~(1 << SR_CLOCK);
                        SR_PORT &= ~(1 << SR_DATA);
                }
                else {
                        SR_PORT &= ~(1 << SR_DATA);
                        SR_PORT |= 1 << SR_CLOCK;
                        SR_PORT &= ~(1 << SR_CLOCK);
                }
                bitmask = bitmask >> 1;
        }

        SR_PORT |= 1 << SR_LATCH;
        SR_PORT &= ~(1 << SR_LATCH);

}

void shift_16bits(uint16_t value) {
        uint16_t bitmask = 0x8000, lval;
        int i;

        SR_PORT &= ~(1 << SR_CLEAR);
        SR_PORT |= 1 << SR_CLEAR;

        for(i = 0; i < 16; ++i) {
                lval = value & bitmask;
                if(lval == bitmask) {
                        SR_PORT |= 1 << SR_DATA;
                        SR_PORT |= 1 << SR_CLOCK;
                        SR_PORT &= ~(1 << SR_CLOCK);
                        SR_PORT &= ~(1 << SR_DATA);
                }
                else {
                        SR_PORT &= ~(1 << SR_DATA);
                        SR_PORT |= 1 << SR_CLOCK;
                        SR_PORT &= ~(1 << SR_CLOCK);
                }
                bitmask = bitmask >> 1;                
        }

        SR_PORT |= 1 << SR_LATCH;
        SR_PORT &= ~(1 << SR_LATCH);
}
