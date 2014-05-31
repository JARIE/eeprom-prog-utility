// File: sst.h
// Author: Jarielle Catbagan
// Created: 30, May 2014

#ifndef SST_H
#define SST_H

#define SST_CTRL_WE PIND5
#define SST_CTRL_OE PIND6
#define SST_CTRL_PORT1 PORTD
#define SST_CTRL_DDR1 DDRD
#define SST_CTRL_CE PINB2
#define SST_CTRL_PORT2 PORTB
#define SST_CTRL_DDR2 DDRB

#define SST_DATA_D0 PINC0
#define SST_DATA_D1 PINC1
#define SST_DATA_D2 PINC2
#define SST_DATA_D3 PINC3
#define SST_DATA_D4 PINC4
#define SST_DATA_D5 PINC5
#define SST_DATA_PORT1 PORTC
#define SST_DATA_DDR1 DDRC
#define SST_DATA_PIN1 PINC

#define SST_DATA_D6 PINB0
#define SST_DATA_D7 PINB1
#define SST_DATA_PORT2 PORTB
#define SST_DATA_DDR2 DDRB
#define SST_DATA_PIN2 PINB

void sst_init(void);
void sst_address(uint16_t address);
void sst_data(uint8_t data);
void sst_CEhigh(void);
void sst_CElow(void);
void sst_WEhigh(void);
void sst_WElow(void);
void sst_OEhigh(void);
void sst_OElow(void);
void sst_clearaddr(void);
void sst_cleardata(void);
void sst_datapoll(uint8_t val_tocmp);
void sst_setdatain(void);
void sst_setdataout(void);

#endif
