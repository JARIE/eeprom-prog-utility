// File: parse.h
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

#include <stdio.h>
#include <stdint.h>
#include "defines.h"

#ifndef PARSE_H
#define PARSE_H

void parse_hexfile(FILE *hexfile_handle, binval_parameters_t **binval_list,
                   uint16_t *binval_list_size);

validity_status_t parsevalidity_peekarg(char *peek_arg, uint16_t *start_memval,
                                        uint16_t *end_memval);

uint16_t numliteral_tonumval(char *val_toconv);

#endif
