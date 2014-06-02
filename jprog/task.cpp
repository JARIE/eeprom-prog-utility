// File: task.cpp
// Author: Jarielle Catbagan
// Created: 01, June 2014

#include <Windows.h>
#include <ftd2xx.h>
#include <stdint.h>
#include <stdio.h>
#include "task.h"
#include "defines.h"

void retrieve_memvals(FT_HANDLE ft_handle, uint16_t start_memval, uint16_t end_memval) {
        FT_STATUS ft_status;
        int index, boundary;
        uint8_t control_byte, acknowledge_byte, data_byte;
	DWORD n_bytes;
        uint16_t address;
        uint8_t tx_buffer[3];
        uint16_t vals_oncurrline = 0;

        index = start_memval - start_memval;
        boundary = end_memval - start_memval;

        address = start_memval;
        control_byte = READ;

        for(; index <= boundary; ++index, ++address) {
                tx_buffer[0] = control_byte;
                tx_buffer[1] = (uint8_t) address;
                tx_buffer[2] = (uint8_t) (address >> 8);
                ft_status = FT_Write(ft_handle, tx_buffer, 3, &n_bytes);
                if(ft_status != FT_OK) {
                        STDERR("device read failed\n");
                        EFAILURE;
                }

		acknowledge_byte = 0;

                ft_status = FT_Read(ft_handle, &acknowledge_byte, 1, &n_bytes);
                if(ft_status != FT_OK) {
                        STDERR("acknowledge byte was not received\n");
                        EFAILURE;
                }
		else if(acknowledge_byte != 0xFE) {
			STDERR("invalid acknowledge byte value received\n");
			EFAILURE;
		}

		ft_status = FT_Read(ft_handle, &data_byte, 1, &n_bytes);
		if(ft_status != FT_OK) {
			STDERR("data byte was not received\n");
			EFAILURE;
		}

		if(vals_oncurrline == 0) {
			print_16bitaddress(address);
			STDOUT(" ");
			print_8bitvalue(data_byte);
		}
		else if(vals_oncurrline < 16) {
			STDOUT(" ");
			print_8bitvalue(data_byte);
		}
		else {
			STDOUT("\n");
			vals_oncurrline = 0;
			print_16bitaddress(address);
			STDOUT(" ");
			print_8bitvalue(data_byte);
		}

		++vals_oncurrline;
        }
}

void print_16bitaddress(uint16_t address) {
	uint16_t dividend, divisor, quotient;
	char buffer[5];
	dividend = address;
	
	buffer[4] = '\0';
	for(int index = 3; index >= 0; --index) {
		quotient = dividend % 16;
		if(quotient < 10)
			buffer[index] = quotient + 48;
		else
			buffer[index] = quotient + 55;
		dividend = dividend / 16;
	}
	printf("%s", buffer);
}

void print_8bitvalue(uint8_t value) {
	uint8_t dividend, divisor, quotient;
	char buffer[3];

	dividend = value;
	
	buffer[2] = '\0';
	for(int index = 1; index >= 0; --index) {
		quotient = dividend % 16;
		if(quotient < 10)
			buffer[index] = quotient + 48;
		else
			buffer[index] = quotient + 55;
		dividend = dividend / 16;
	}
	printf("%s", buffer);
}

