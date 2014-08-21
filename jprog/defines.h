// File: defines.h
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

#ifndef DEFINES_H
#define DEFINES_H

#define STDERR(str, ...) fprintf(stderr, "error: " str, ##__VA_ARGS__)
#define STDOUT(str, ...) fprintf(stdout, str, ##__VA_ARGS__)
#define DEBUG(str, ...) fprintf(stdout, "debug: " str, ##__VA_ARGS__)
#define EFAILURE exit(EXIT_FAILURE);
#define ESUCCESS exit(EXIT_SUCCESS);

typedef struct binval_parameters_t {
        uint16_t address;
        uint8_t data;
} binval_parameters_t;

typedef enum validity_status_t {UNKNOWN = 0, INVALID, VALID} validity_status_t;

#endif
