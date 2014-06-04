// File: task.h
// Author: Jarielle Catbagan
// Created: 01, June 2014

#include <Windows.h>
#include <ftd2xx.h>
#include <stdint.h>
#include "defines.h"

#ifndef TASK_H
#define TASK_H

#define READ 0xA5
#define WRITE 0x8C
#define ERASE 0xE3

void retrieve_memvals(FT_HANDLE ft_handle, uint16_t start_memval, uint16_t end_memval);
void print_16bitaddress(uint16_t address);
void print_8bitvalue(uint8_t value);
void program_datavals(FT_HANDLE ft_handle, binval_parameters_t *binval_list,
                      uint16_t binval_list_size);
void erase_memvals(FT_HANDLE ft_handle);

#endif
