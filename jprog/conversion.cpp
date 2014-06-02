// File: conversion.cpp
// Author: Jarielle Catbagan
// Created: 30, May 2014

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
