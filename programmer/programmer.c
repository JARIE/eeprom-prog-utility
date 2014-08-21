// File: programmer.c
// Created: 28, May 2014

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

#include <avr/io.h>
#include <util/atomic.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include "shiftreg.h"
#include "mem.h"

#define READ 0xA5
#define WRITE 0x8C
#define ERASE 0xE3

volatile enum data_status_t {NO_DATA = 0, NEW_DATA} data_status;
volatile uint8_t rx_buffer[4], tx_buffer[2];
volatile uint8_t rx_index, tx_index, rx_buffer_size, tx_buffer_size;

int main(void) {
        uint8_t index, lval, data_nbytes;
        uint16_t address;
        uint8_t data;

        rx_index = 0;
        rx_buffer_size = 0;
        tx_index = 0;
        tx_buffer_size = 0;
        data_status = NO_DATA;

        mem_init();
        
        // disable power reduction to USART module
        PRR &= ~(1 << PRUSART0);
        // for asynchronous operation set to normal mode, value for UBRR0 is 
        // determined by UBRR0 = fosc\(16 * BAUD) - 1
        // set the UBRR value to 9 for 125000 baud
        UBRR0H = 0;
        UBRR0L = 0x09;
        // set the USART module to asynchronous operation
        UCSR0C &= ~(1 << UMSEL00 | 1 << UMSEL01);
        // set the asynchronous operation to normal mode
        UCSR0A &= ~(1 << U2X0);
        // set the frame format to 8-bit
        UCSR0B &= ~(1 << UCSZ02);
        UCSR0C |= (1 << UCSZ00 | 1 << UCSZ01);
        // disable the parity generation/detection
        UCSR0C &= ~(1 << UPM00 | 1 << UPM01);
        // set the transmitter to send 2 stop-bits
        UCSR0C |= 1 << USBS0;
        // enable the receiver and transmitter
        UCSR0B |= (1 << RXEN0 | 1 << TXEN0);
        // enable the interrupt when data has been received
        UCSR0B |= 1 << RXCIE0;
        // enable global interrupts
        sei();

        while(1) {
                if(data_status == NEW_DATA) {
                        if(rx_buffer[0] == READ) {
                                address = rx_buffer[2];
                                address = address << 8;
                                address |= rx_buffer[1];
                                tx_buffer[0] = 0xFE;
                                tx_buffer[1] = mem_read(address);
                                tx_buffer_size = 2;
                                UCSR0B |= 1 << UDRIE0;
                        }
                        else if(rx_buffer[0] == WRITE) {
                                address = rx_buffer[2];
                                address = address << 8;
                                address |= rx_buffer[1];
                                data = rx_buffer[3];
                                mem_write(address, data);
                                tx_buffer[0] = 0xFE;
                                tx_buffer_size = 1;
                                UCSR0B |= 1 << UDRIE0;
                                
                        }
                        else if(rx_buffer[0] == ERASE) {
                                mem_erase();
                                tx_buffer[0] = 0xFE;
                                tx_buffer_size = 1;
                                UCSR0B |= 1 << UDRIE0;
                        }
                        data_status = NO_DATA;
                }
        }
        
        return 0;
}

ISR(USART_RX_vect) {
        rx_buffer[rx_index] = UDR0;

        if(rx_index == 0) {
                if(rx_buffer[rx_index] == READ)
                        rx_buffer_size = 3;
                else if(rx_buffer[rx_index] == WRITE)
                        rx_buffer_size = 4;
                else if(rx_buffer[rx_index] == ERASE)
                        rx_buffer_size = 1;
        }

        ++rx_index;
        if(rx_index == rx_buffer_size) {
                rx_index = 0;
                data_status = NEW_DATA;
        }
}

ISR(USART_UDRE_vect) {
        UDR0 = tx_buffer[tx_index];
        ++tx_index;
        if(tx_index == tx_buffer_size) {
                tx_index = 0;
                data_status = NO_DATA;
                UCSR0B &= ~(1 << UDRIE0);
        }
}

