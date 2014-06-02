// File: shiftreg.c
// Author: Jarielle Catbagan
// Created: 30, May 2014

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
