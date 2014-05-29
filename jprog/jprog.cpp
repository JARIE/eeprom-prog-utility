// File: main.cpp
// Author: Jarielle Catbagan
// Created: 23, April 2014

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

#define STDERR(str, ...) fprintf(stderr, "error: " str, ##__VA_ARGS__)
#define EFAILURE exit(EXIT_FAILURE);
#define ESUCCESS exit(EXIT_SUCCESS);

uint8_t hexliteral_to8bitnum(char *hexvalue_literal) {
        int index;
        uint8_t value = 0, multiplicand = 0, exponent = 0;
        
        for(index = 1; index >= 0; --index) {
                if(hexvalue_literal[index] >= 'A' &&
                   hexvalue_literal[index] <= 'F')
                        multiplicand = hexvalue_literal[index] - 55;
                else
                        multiplicand = hexvalue_literal[index] - 48;

                value += multiplicand * pow(16, exponent++);
        }

        return value;
}

uint16_t hexliteral_to16bitnum(char *hexvalue_literal) {
        int index;
        uint8_t value = 0, multiplicand = 0, exponent = 0;

        for(index = 3; index >= 0; --index) {
                if(hexvalue_literal[index] >= 'A' &&
                   hexvalue_literal[index] <= 'F')
                        multiplicand = hexvalue_literal[index] - 55;
                else
                        multiplicand = hexvalue_literal[index] - 48;

                value += multiplicand * pow(16, exponent++);
        }

        return value;
}

int main(int argc, char ** argv) {
	FT_STATUS ft_status;
	FT_DEVICE_LIST_INFO_NODE *dev_info;
	FT_HANDLE ft_handle;
	DWORD num_of_devs;
        DWORD baud = 125000;
	enum status_t {NOT_FOUND = 0, FOUND} device_endpoint;
	device_endpoint = NOT_FOUND;

	DWORD n_bytes;
	char user_input[1];
	char message[10];
	enum {EXIT = 0, RUN} user_status;
	user_status = RUN;

        int file_status, c, index, i;
        FILE *hexfile_handle;
        char *hexfile_name;

        char bytecount_literal[3], data_literal[3], address_literal[5], record_literal[5],
                checksum_literal[3];
        uint8_t bytecount, data[16], checksum;
        uint16_t address;

        enum record_type_t {END_OF_FILE = 0, DATA} record_type;

        enum program_status_t {STOP_PARSE = 0, PARSE_HEXFILE} program_status;

        uint8_t start_code = 0xA5, acknowledge_byte;

        uint8_t tx_buffer[50], rx_buffer[50];

        /* Make sure that the right number of arguments are specified. */
        if(argc > 2) {
                STDERR("invalid number of arguments specified\n");
        }
        else if(argc < 2) {
                STDERR("no hex file specified\n");
        }

        if(argc > 2 || argc < 2) {
                fprintf(stdout, "\nusage: jprog [file]\n");
                EFAILURE;
        }

        /* Determine if the file specified is in the correct format. In this case, the
           file format must be a hex file. */
        if(strstr(argv[1], ".hex") == NULL) {
                STDERR("the specified file is not a hex file\n");
                EFAILURE;
        }
        else 
                hexfile_name = argv[1];

        /* Open the file for reading only. */
        hexfile_handle = fopen(hexfile_name, "r");
        if(hexfile_handle == NULL) {
                STDERR("the file \"%s\" could not be opened\n", hexfile_name);
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

// development area start

        program_status = PARSE_HEXFILE;
        
        while(program_status == PARSE_HEXFILE) {
                c = fgetc(hexfile_handle);
                if(c != ':') {
                        STDERR("no start code found in \"%s\"\n", hexfile_name);
                        file_status = fclose(hexfile_handle);
                        ft_status = FT_Close(ft_handle);
                        EFAILURE;
                }

                for(index = 0; index < 2; ++index) {
                        c = fgetc(hexfile_handle);
                        bytecount_literal[index] = c;
                }
                bytecount_literal[index] = '\0';

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

                        for(index = 0; index < bytecount; ++index) {
                                for(i = 0; i < 2; ++i) {
                                        c = fgetc(hexfile_handle);
                                        data_literal[i] = c;
                                }
                                data_literal[i] = '\0';
                                data[index] = hexliteral_to8bitnum(data_literal);
                        }

                        for(index = 0; index < 2; ++index) {
                                c = fgetc(hexfile_handle);
                                checksum_literal[index] = c;
                        }
                        checksum_literal[index] = '\0';

                        checksum = hexliteral_to8bitnum(checksum_literal);

                        do {
                                c = fgetc(hexfile_handle);
                        } while(c != '\n');

                        ft_status = FT_Write(ft_handle, &start_code, 1, &n_bytes);
                        if(ft_status != FT_OK) {
                                STDERR("data write failed\n");
                                file_status = fclose(hexfile_handle);
                                ft_status = FT_Close(ft_handle);
                                EFAILURE;
                        }

                        printf("start code %#x sent\n", start_code);
                        
#ifdef FT_READ_READY                        
                        ft_status = FT_Read(ft_handle, &acknowledge_byte, 1, &n_bytes);
                        if(ft_status != FT_OK) {
                                STDERR("data read failed\n");
                                file_status = fclse(hexfile_handle);
                                ft_status = FT_Close(ft_handle);
                                EFAILURE;
                        }
                        else if(acknowledge_byte != 0xFE) {
                                STDERR("invalid acknowledge byte received\n");
                                file_status = fclse(hexfile_handle);
                                ft_status = FT_Close(ft_handle);
                                EFAILURE;
                        }
#endif
                        
                        tx_buffer[0] = (uint8_t) address;
                        tx_buffer[1] = (uint8_t) (address >> 8);
                        tx_buffer[2] = bytecount;

                        for(index = 3, i = 0; i < bytecount; ++index, ++i)
                                tx_buffer[index] = data[i];

                        ft_status = FT_Write(ft_handle, tx_buffer, (bytecount + 3),
                                             &n_bytes);
                        if(ft_status != FT_OK) {
                                STDERR("data write failed\n");
                                file_status = fclose(hexfile_handle);
                                ft_status = FT_Close(ft_handle);
                                EFAILURE;                                
                        }

			printf("tx buffer sent\n");
			for(index = 0; index < (bytecount + 3); ++index)
				printf("%#x ", tx_buffer[index]);
			printf("\n");

#ifdef FT_READ_READY
                        ft_status = FT_Read(ft_handle, &acknowledge_byte, 1, &n_bytes);
                        if(ft_status != FT_OK) {
                                STDERR("data read failed\n");
                                file_status = fclse(hexfile_handle);
                                ft_status = FT_Close(ft_handle);
                                EFAILURE;
                        }
                        else if(acknowledge_byte != 0xFE) {
                                STDERR("invalid acknowledge byte received\n");
                                file_status = fclse(hexfile_handle);
                                ft_status = FT_Close(ft_handle);
                                EFAILURE;
                        }
#endif                        

                        
                        
                        program_status = PARSE_HEXFILE;
                }
                else {
                        program_status = STOP_PARSE;
                }
        }
        


// development area end
        

        /* Close the file opened and the device communication. */
        file_status = fclose(hexfile_handle);
        if(file_status == EOF) 
                STDERR("the file \"%s\" could could not be closed\n", hexfile_name);

	ft_status = FT_Close(ft_handle);
        if(ft_status != FT_OK)
                STDERR("device communication could not be closed\n");

        if(file_status == EOF || ft_status != FT_OK) 
                EFAILURE;

        ESUCCESS;
}
