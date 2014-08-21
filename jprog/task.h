// File: task.h
// Created: 01, June 2014

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
