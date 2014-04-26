// File: main.cpp
// Author: Jarielle Catbagan
// Created: 23, April 2014

// Description: 

/* This is the main source file that will form the basis of the program utility dubbed 'jprog'. 
   This program is responsible for transferring binaries betwween the host and target platform.
   Requires an FTDI TTL232R USB to UART Converter Cable and a device endpoint to handle the 
   UART communications on the other end to function. */

// Code:

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Windows.h>
#include <ftd2xx.h>

int main(int argc, char ** argv) {
	FT_STATUS ft_status;
	FT_DEVICE_LIST_INFO_NODE *dev_info;
	FT_HANDLE ft_handle;
	DWORD num_of_devs;
	enum {NOT_FOUND = 0, FOUND} device_endpoint;
	device_endpoint = NOT_FOUND;

	DWORD num_of_bytes_written;
	char user_input[1];
	char message[10];
	enum {EXIT = 0, RUN} user_status;
	user_status = RUN;

	ft_status = FT_CreateDeviceInfoList(&num_of_devs);
	if(ft_status != FT_OK) {
		fprintf(stderr, "error: device info list could not be created\n");
		fprintf(stderr, "       @FT_CreateDeviceInfoList()\n");
		exit(EXIT_FAILURE);
	}

	if(num_of_devs == 0) {
		fprintf(stderr, "error: no TTL232R connected\n");
		exit(EXIT_FAILURE);
	}

	dev_info = (FT_DEVICE_LIST_INFO_NODE *) malloc(sizeof(FT_DEVICE_LIST_INFO_NODE) *num_of_devs);
	if(dev_info == NULL) {
		fprintf(stderr, "error: memory for device info list could not be allocated\n");
		exit(EXIT_FAILURE);
	}

	ft_status = FT_GetDeviceInfoList(dev_info, &num_of_devs);
	if(ft_status != FT_OK) {
		fprintf(stderr, "error: device info list could not be retrieved\n");
		fprintf(stderr, "       @FT_GetDeviceInfoList()\n");
		exit(EXIT_FAILURE);
	}

	for(int i = 0; i < num_of_devs; ++i) 
		if(!strcmp(dev_info[i].Description, "TTL232R"))
			device_endpoint = FOUND;
	
	if(device_endpoint == NOT_FOUND) {
		fprintf(stderr, "error: no TTL232R connected\n");
		exit(EXIT_FAILURE);
	}

	ft_status = FT_OpenEx("TTL232R", FT_OPEN_BY_DESCRIPTION, &ft_handle);
	if(ft_status != FT_OK) {
		fprintf(stderr, "error: no connection couuld established with TTL232R\n");
		fprintf(stderr, "       @FT_OpenEx\n");
		exit(EXIT_FAILURE);
	}

	ft_status = FT_SetBaudRate(ft_handle, 125000);
	if(ft_status != FT_OK) {
		fprintf(stderr, "error: specified baud rate could not be set\n");
		fprintf(stderr, "       @FT_SetBaudRate()\n");
		exit(EXIT_FAILURE);
	}

	ft_status = FT_SetDataCharacteristics(ft_handle, FT_BITS_8, FT_STOP_BITS_2, FT_PARITY_NONE);
	if(ft_status != FT_OK) {
		fprintf(stderr, "error: data characteristics of communication could not be set\n");
		fprintf(stderr, "       @FT_SetDataCharacteristics()\n");
		exit(EXIT_FAILURE);
	}

	do {
		user_input[0] = (char) fgetc(stdin);
		if(user_input[0] != '\n') {
			if(user_input[0] == 'e')
				user_status = EXIT;
			else {
				ft_status = FT_Write(ft_handle, user_input, 1, &num_of_bytes_written);
				if(ft_status != FT_OK) {
					fprintf(stderr, "error: data write failed\n");
					fprintf(stderr, "       @FT_Write()\n");
				}
				ft_status = FT_Read(ft_handle, message, 8, &num_of_bytes_written);
				if(ft_status != FT_OK) {
					fprintf(stderr, "error: data read failed\n");
					fprintf(stderr, "       @FT_Read()\n");
				}
				else {
					for(int i = 0; i < 8; ++i) {
						fprintf(stdout, "%c", message[i]);
					}
					fprintf(stdout, "\n");
				}
			}
		}
	} while(user_status == RUN);

	ft_status = FT_Close(ft_handle);
	if(ft_status != FT_OK) {
		fprintf(stderr, "error: device endpoint could not be closed\n");
		fprintf(stderr,"        @FT_Close\n");
		exit(EXIT_FAILURE);
	}
	return 0;
}