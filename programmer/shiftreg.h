// File: shiftreg.h
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
