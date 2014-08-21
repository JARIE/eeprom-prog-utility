// File: jprog.cpp
// Created: 23, April 2014

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

// Description: 

/* This is the main source file that will form the basis of the program utility dubbed
   'jprog'. This program is responsible for transferring binaries betwween the host and
   target platform. Requires an FTDI TTL232R USB to UART Converter Cable and a device
   endpoint to handle the UART communications on the other end to function. */

// Code:

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdint.h>
#include <Windows.h>
#include <ftd2xx.h>
#include "conversion.h"
#include "parse.h"
#include "defines.h"
#include "task.h"

int main(int argc, char ** argv) {
	FT_STATUS ft_status;
	FT_DEVICE_LIST_INFO_NODE *dev_info;
	FT_HANDLE ft_handle;
	DWORD num_of_devs;
        DWORD baud = 125000;
	enum status_t {NOT_FOUND = 0, FOUND} device_endpoint;
	DWORD n_bytes;
        int file_status;
        FILE *hexfile_handle;
        char *hexfile_name;
        uint8_t control_byte = 0xC0, acknowledge_byte = 0;
        uint8_t tx_buffer[50], rx_buffer[50];
	binval_parameters_t *binval_list;
	uint16_t binval_list_size;
        validity_status_t validity_status;
        enum command_status_t {PEEK = 0, PORT, ERASE_MEM} command_status;
	char *command_specified, *argument_specified;
	uint16_t start_memval, end_memval;

        device_endpoint = NOT_FOUND;

        if(argc == 1) {
                STDERR("no command/option specified\n");
                EFAILURE;
        }
        else if(argc == 2) {
                command_specified = argv[1];
                if(!strcmp(command_specified, "help")) {
                        STDOUT("usage: jprog [command] [argument]\n\n");
                        ESUCCESS;
                }
                else if(!strcmp(command_specified, "erase")) {
                        STDERR("memory area to erase has not been specified\n");
                        EFAILURE;
                }
                else if(!strcmp(command_specified, "peek")) {
                        STDERR("no memory location specified\n");
                        EFAILURE;
                }
                else if(!strcmp(command_specified, "port")) {
                        STDERR("no hex file specified\n");
                        EFAILURE;
                }
                else {
                        STDERR("invalid command\n");
                        EFAILURE;
                }
        }
        else if(argc == 3) {
                command_specified = argv[1];
                argument_specified = argv[2];
                if(!strcmp(command_specified, "help")) {
                        STDERR("too many arguments specified\n");
                        EFAILURE;
                }
                else if(!strcmp(command_specified, "erase")) {
			command_status = ERASE_MEM;
                }
                else if(!strcmp(command_specified, "peek")) {
                        validity_status =
                                parsevalidity_peekarg(argument_specified,
                                                      &start_memval,
                                                      &end_memval);
                        if(validity_status == INVALID) {
                                STDERR("invalid memory location specified\n");
                                EFAILURE;
                        } 
                        else {
                                command_status = PEEK;
                        }
                        
                }
                else if(!strcmp(command_specified, "port")) {
                        if(strstr(argument_specified, ".hex") == NULL) {
                                STDERR("the specified file is not a hex file\n");
                                EFAILURE;
                        }
                     
                        command_status = PORT;
                        hexfile_name = argument_specified;
			hexfile_handle = fopen(hexfile_name, "r");
			if(hexfile_handle == NULL) {
				STDERR("the specified hex file could not be opened\n");
				EFAILURE;
			}
                }
                else {
                        STDERR("invalid command\n");
                        EFAILURE;
                }
        }
        else {
                STDERR("too many arguments specified\n");
                EFAILURE;
        }


        /* Get the number of D2XX devices connected, and created a info list about the
           devices. */
	ft_status = FT_CreateDeviceInfoList(&num_of_devs);
	if(ft_status != FT_OK) {
                STDERR("device info list could not be created\n");
                EFAILURE;
	}

	if(num_of_devs == 0) {
                STDERR("no DRXX devices connected\n");
                EFAILURE;
	}

        /* Create a device info list to store the information retrieved. */
	dev_info = (FT_DEVICE_LIST_INFO_NODE *)
                malloc(num_of_devs * sizeof(*dev_info));
	if(dev_info == NULL) {
                STDERR("device info list could not be allocated\n");
                EFAILURE;
	}

        /* Store the device information from the info list just created. */
	ft_status = FT_GetDeviceInfoList(dev_info, &num_of_devs);
	if(ft_status != FT_OK) {
                STDERR("device info list could not be retrieved\n");
                EFAILURE;
	}

        /* Check if the device at interest is connected. */
	for(int i = 0; i < num_of_devs; ++i) 
		if(!strcmp(dev_info[i].Description, "TTL232R"))
			device_endpoint = FOUND;
	
	if(device_endpoint == NOT_FOUND) {
                STDERR("no TTL232R connected\n");
                EFAILURE;
	}

        /* Open the device for communication. */
	ft_status = FT_OpenEx("TTL232R", FT_OPEN_BY_DESCRIPTION, &ft_handle);
	if(ft_status != FT_OK) {
                STDERR("connection could not be established with TTL232R\n");
                EFAILURE;
	}

        /* Set the baud rate. */
	ft_status = FT_SetBaudRate(ft_handle, baud);
	if(ft_status != FT_OK) {
                STDERR("the specifed baud rate %d could not be set\n", baud);
		exit(EXIT_FAILURE);
	}

        /* Set the communication properties. */
	ft_status = FT_SetDataCharacteristics(ft_handle, FT_BITS_8, FT_STOP_BITS_2,
                                              FT_PARITY_NONE);
	if(ft_status != FT_OK) {
                STDERR("establishing communication properties failed\n");
                EFAILURE;
	}

        /* Set the read and write timeouts. */
        ft_status = FT_SetTimeouts(ft_handle, 5000, 1000);
        if(ft_status != FT_OK) {
                STDERR("read and write timeouts could not be set\n");
                EFAILURE;
        }
        
	switch(command_status) {
	case PEEK:
                retrieve_memvals(ft_handle, start_memval, end_memval);
		break;
	case PORT:
		parse_hexfile(hexfile_handle, &binval_list, &binval_list_size);
		program_datavals(ft_handle, binval_list, binval_list_size);
		break;
	case ERASE_MEM:
		erase_memvals(ft_handle);
		break;
	}

	if(command_status == PORT) {
		file_status = fclose(hexfile_handle);
		if(file_status == EOF) 
			STDERR("the hexfile could not be closed\n");
	}

	ft_status = FT_Close(ft_handle);
        if(ft_status != FT_OK)
                STDERR("device communication could not be closed\n");

        if(file_status == EOF || ft_status != FT_OK) 
                EFAILURE;

        ESUCCESS;
}
