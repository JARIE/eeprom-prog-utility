// File: task.cpp
// Author: Jarielle Catbagan
// Created: 01, June 2014

#include "task.h"

void retrieve_memvals(FT_HANDLE ft_handle, uint16_t start_memval, uint16_t end_memval) {
        FT_STATUS ft_status;
        int index, boundary;
        uint8_t control_byte, acknowledge_byte, n_bytes, data_byte;
        uint16_t address;
        uint8_t tx_buffer[3];
        uint16_t vals_oncurrline = 0;

        index = start_memval - start_memval;
        boundary = end_memval - start_memval;

        address = start_memval;
        control_byte = READ;

        for(; index <= boundary; ++index) {
                tx_buffer[0] = control_byte;
                tx_buffer[1] = (uint8_t) address;
                tx_buffer[2] = (uint8_t) (address >> 8);
                ft_status = FT_Write(ft_handle, tx_buffer, 3, &n_bytes);
                if(ft_status != FT_OK) {
                        STDERR("device read failed\n");
                        EFAILURE;
                }

                ft_status = FT_Read(ft_handle, &acknowledge_byte, 1, &n_bytes);
                if(ft_status != FT_OK) {
                        STDERR("acknowledge byte was not received\n");
                        EFAILURE;
                }

                ft_status = FT_Read(ft_handle, &data_byte, 1, &n_bytes);
                if(ft_status != FT_OK) {
                        STDERR("data value could not be retrieved\n");
                        EFAILURE;
                }

                
        }
}

