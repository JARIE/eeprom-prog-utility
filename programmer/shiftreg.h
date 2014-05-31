// File: shiftreg.h
// Author: Jarielle Catbagan
// Created: 30, May 2014

#ifndef SHIFTREG_H
#define SHIFTREG_H

#define SR_DATA PIND4
#define SR_LATCH PIND5
#define SR_CLOCK PIND6
#define SR_CLEAR PIND7
#define SR_PORT PORTD
#define SR_DDR DDRD

void shiftreg_init(void);
void shift_8bits(uint8_t value);
void shift_16bits(uint16_t value);

#endif
