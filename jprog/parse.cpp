// File: parse.c
// Author: Jarielle Catbagan
// Created: 30, May 2014

#include <stdio.h>
#include "stdlib.h"
#include <stdint.h>
#include <string.h>
#include <math.h>
#include "parse.h"
#include "conversion.h"
#include "defines.h"

void parse_hexfile(FILE *hexfile_handle, binval_parameters_t **binval_list,
                   uint16_t *binval_list_size) {
        int index, c, i;
        binval_parameters_t *binval_currentlist = NULL, *binval_newlist = NULL;
        uint16_t currentsize = 0, actualsize = 0, newsize;
        char bytecount_literal[3], address_literal[5], record_literal[3], data_literal[3],
                checksum_literal[3];
        uint8_t bytecount, data;
        uint16_t address;
        enum program_status_t {PARSE_HEXFILE = 0, STOP_PARSE} program_status;
        enum record_type_t {END_OF_FILE = 0, DATA} record_type;

        program_status = PARSE_HEXFILE;
        
        while(program_status == PARSE_HEXFILE) {
                c = fgetc(hexfile_handle);
                if(c != ':') {
                        if(actualsize != 0) 
                                free(*binval_list);

                        STDERR("data storage allocation for binary files in file was ",
                               "interrupted\n");
                        EFAILURE;
                }

                for(index = 0; index < 2; ++index) {
                        c = fgetc(hexfile_handle);
                        bytecount_literal[index] = c;
                }
                bytecount_literal[index] = c;

                for(index = 0; index < 4; ++index) {
                        c = fgetc(hexfile_handle);
                        address_literal[index] = c;
                }
                address_literal[index] = '\0';

                for(index = 0; index < 2; ++index) {
                        c = fgetc(hexfile_handle);
                        record_literal[index] = c;
                }
                record_literal[index] = '\0';

                if(!strcmp(record_literal, "00"))
                        record_type = DATA;
                else
                        record_type = END_OF_FILE;

                if(record_type == DATA) {
                        bytecount = hexliteral_to8bitnum(bytecount_literal);
                        address = hexliteral_to16bitnum(address_literal);

			newsize = actualsize + bytecount;
			//not allocating the right size; need to fix!!!
                        binval_newlist = (binval_parameters_t *)
                                realloc(binval_currentlist, (newsize *
                                                       sizeof(*binval_newlist)));
                        if(binval_newlist == NULL) {
                                free(binval_currentlist);
                                STDERR("data storage could not be allocated for the ",
                                       "binary values in the hex file specified\n");
                                EFAILURE;
                        }
                        else {
                                binval_currentlist = binval_newlist;
                                actualsize = newsize;
                        }

                        for(index = 0; index < bytecount; ++index) {
                                for(i = 0; i < 2; ++i) {
                                        c = fgetc(hexfile_handle);
                                        data_literal[i] = c;
                                }
                                data_literal[i] = '\0';
                                data = hexliteral_to8bitnum(data_literal);
                                binval_currentlist[currentsize].address = address;
                                binval_currentlist[currentsize].data = data;
                                ++currentsize;
                                ++address;
                        }

                        do {
                                c = fgetc(hexfile_handle);
                        } while(c != '\n');
                }
                else {
                        *binval_list = binval_currentlist;
                        *binval_list_size = actualsize;
                        program_status = STOP_PARSE;
                }

        }
        
}

validity_status_t parsevalidity_peekarg(char *peek_arg, uint16_t *start_memval,
                                        uint16_t *end_memval) {
        int boundary;
        uint8_t peek_arglength;
        validity_status_t validity_status = UNKNOWN;
        char start_memliteral[20], end_memliteral[20];
        int index1, index2;
        uint16_t lval1, lval2;

        peek_arglength = strlen(peek_arg);
        boundary = 0;
        for(index1 = 0; index1 < peek_arglength; ++index1) {
                if(peek_arg[index1] == ':')
                        boundary = index1;
        }

        if(boundary == 0 || boundary == (peek_arglength - 1))
                validity_status = INVALID;
        else {
                for(index1 = 0; index1 < boundary; ++index1) 
                        start_memliteral[index1] = peek_arg[index1];

                start_memliteral[index1] = '\0';

                for(index1 = (boundary + 1), index2 = 0; index1 < peek_arglength;
                    ++index1, ++index2) 
                        end_memliteral[index2] = peek_arg[index1];

                end_memliteral[index2] = '\0';

                validity_status = VALID;

                index1 = strlen(start_memliteral) - 1;
                if(start_memliteral[index1] == 'h' || start_memliteral[index1] == 'H')
                        boundary = index1;
                else
                        boundary = strlen(start_memliteral);

                for(index1 = 0; index1 < boundary; ++index1)
                        if((start_memliteral[index1] < '0' ||
                            start_memliteral[index1] > '9') &&
                           (start_memliteral[index1] < 'A' ||
                            start_memliteral[index1] > 'F') &&
                           (start_memliteral[index1] < 'a' ||
                            start_memliteral[index1] > 'f'))
                                validity_status = INVALID;

                index1 = strlen(end_memliteral) - 1;
                if(end_memliteral[index1] == 'h' || end_memliteral[index1] == 'H')
                        boundary = index1;
                else
                        boundary = strlen(end_memliteral);

                for(index1 = 0; index1 < boundary; ++index1)
                        if((end_memliteral[index1] < '0' ||
                            end_memliteral[index1] > '9') &&
                           (end_memliteral[index1] < 'A' ||
                            end_memliteral[index1] > 'F') &&
                           (end_memliteral[index1] < 'a' ||
                            end_memliteral[index1] > 'f'))
                                validity_status = INVALID;

                if(validity_status != INVALID) {
                        lval1 = numliteral_tonumval(start_memliteral);
                        lval2 = numliteral_tonumval(end_memliteral);

                        if(lval1 > lval2)
                                validity_status = INVALID;
                        else {
                                validity_status = VALID;
                                *start_memval = lval1;
                                *end_memval = lval2;
                        }
                }
        }
        
        return validity_status;
}

uint16_t numliteral_tonumval(char *val_toconv) {
        int index, boundary;
        uint16_t multiplicand, exponent = 0, value = 0;

        index = strlen(val_toconv) - 1;
        if(val_toconv[index] == 'h' || val_toconv[index] == 'H') {
                index -= 1;
                for(; index >= 0; --index) {
                        if(val_toconv[index] >= '0' && val_toconv[index] <= '9')
                                multiplicand = val_toconv[index] - 48;
                        else
                                multiplicand = val_toconv[index] - 55;

                        value += multiplicand * pow(16.0, exponent++);
                }
        }
        else {
                index = strlen(val_toconv) - 1;
                for(; index >= 0; --index) {
                        multiplicand = val_toconv[index] - 48;
                        value += multiplicand * pow(10.0, exponent++);
                }
        }
        
        return value;
}
