// File: conversion.h
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

#include <stdint.h>

#ifndef CONVERION_H
#define CONVERSION_H

uint8_t hexliteral_to8bitnum(char *hexvalue_literal);
uint16_t hexliteral_to16bitnum(char *hexvalue_literal);
uint16_t numericalliteral_tonum(char *value_toconvert);

#endif
