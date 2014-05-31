// File: programmer.c
// Author: Jarielle Catbagan
// Created: 28, May 2014

#include <avr/io.h>
#include <util/atomic.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include "shiftreg.h"
#include "mem.h"

volatile enum data_status_t {NO_DATA = 0, NEW_DATA} data_status;
volatile uint8_t rx_value, tx_value;

int main(void) {
        enum expect_status_t {CONTROL_BYTE = 0, DATA_PACKET} expect_status;
        uint8_t rx_buffer[30];
        uint8_t index, lval, data_nbytes;
        uint16_t address;
        
        data_status = NO_DATA;
        expect_status = CONTROL_BYTE;
        program_status = RECEIVING;


        //mighit remove shiftreg_init() and move all statements into meminit()
        shiftreg_init();
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
                        if(expect_status == CONTROL_BYTE) {
                                if((rx_value & 0xC0) == 0xC0) {
                                        data_nbytes = rx_value & 0x3F;
                                        expect_status = DATA_PACKET;
                                        index = 0;
                                        tx_value = 0xFE;
                                        UCSR0B |= 1 << UDRIE0;
                                }
                        }
                        else { //expect_status == DATA_PACKET
                                if(index < data_nbytes) 
                                        rx_buffer[index++] = rx_value;

                                if(index == data_nbytes) {
                                        expect_status = CONTROL_BYTE;
                                        
                                        address = rx_buffer[1];
                                        address = address << 8;
                                        address |= rx_buffer[0];

                                        shift_16bits(address);
                                        
                                        tx_value = 0xFE;
                                        UCSR0B |= 1 << UDRIE0;
                                }
                                        
                        }
                        data_status = NO_DATA;
                }
        }
        
        return 0;
}

ISR(USART_RX_vect) {
        data_status = NEW_DATA;
        rx_value = UDR0;
}

ISR(USART_UDRE_vect) {
        UDR0 = tx_value;
        UCSR0B &= ~(1 << UDRIE0);
}

