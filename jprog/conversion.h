// File: conversion.h
// Author: Jarielle Catbagan
// Created: 30, May 2014

#include <stdint.h>

#ifndef CONVERION_H
#define CONVERSION_H

uint8_t hexliteral_to8bitnum(char *hexvalue_literal);
uint16_t hexliteral_to16bitnum(char *hexvalue_literal);
uint16_t numericalliteral_tonum(char *value_toconvert);

#endif
