// File: conversion.cpp
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

#include <math.h>
#include <string.h>
#include "conversion.h"

uint8_t hexliteral_to8bitnum(char *hexvalue_literal) {
        int index;
        uint8_t value = 0, multiplicand = 0, exponent = 0;
        
        for(index = 1; index >= 0; --index) {
                if(hexvalue_literal[index] >= 'A' &&
                   hexvalue_literal[index] <= 'F')
                        multiplicand = hexvalue_literal[index] - 55;
                else
                        multiplicand = hexvalue_literal[index] - 48;

                value += multiplicand * pow(16.0, exponent++);
        }

        return value;
}

uint16_t hexliteral_to16bitnum(char *hexvalue_literal) {
        int index;
        uint8_t value = 0, multiplicand = 0, exponent = 0;

        for(index = 3; index >= 0; --index) {
                if(hexvalue_literal[index] >= 'A' &&
                   hexvalue_literal[index] <= 'F')
                        multiplicand = hexvalue_literal[index] - 55;
                else
                        multiplicand = hexvalue_literal[index] - 48;

                value += multiplicand * pow(16.0, exponent++);
        }

        return value;
}
