// File: parse.h
// Author: Jarielle Catbagan
// Created: 30, May 2014

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
