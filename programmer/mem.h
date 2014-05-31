// File: mem.h
// Author: Jarielle Catbagan
// Created: 30, May 2014


#ifndef MEM_H
#define MEM_H

void mem_init(void);
void mem_write(uint16_t address, uint8_t data);
uint8_t mem_read(uint16_t address);
void mem_erase(void);

#endif
