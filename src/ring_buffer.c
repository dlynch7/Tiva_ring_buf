// ring_buffer.c is based on a program of the same name
// in Chapter 11 (UART) of NU's mechatronics textbook.
// Here I adapt it into a library for the TM4C123G...
#include "ring_buffer.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>

#include "inc/hw_memmap.h"
#include "driverlib/gpio.h"

#include "setup.h" // provides access to LED color macros

static volatile uint16_t read = 0, write = 0;	// circ buffer indices
static volatile buffer_t data_buf;  // array that stores the data
static volatile buf_out_t data_out;

void buffer_reset(void) { // resets read and write indices
    read = 0;
    write = 0;
}

void buffer_erase(void) { // erases buffer by setting everything to 0
    uint32_t i = 0;
    for (i=0; i<BUFLEN; i++)
    {
        buffer_write(0,0,0,0,0,0,0,0);
    }
    buffer_reset();
}

uint16_t get_read_index(void) { // return the value of the read index
	return read;
}
uint16_t get_write_index(void) {// return the value of the write index
	return write;
}

uint8_t buffer_empty(void) { // return true if the buffer is empty (read=write)
	return read==write;
}

uint8_t buffer_full(void) { // return true if the buffer is full
	return ((write + 1) % BUFLEN) == read;
}

// reads from current buffer index and increment read index by 1:
void buffer_read(buf_out_t *data_out) // input arg is pointer to struct of type buf_out_struct
{
	while(buffer_empty())  // wait for data to be in the buffer
    {
        //GPIOPinWrite(GPIO_PORTF_BASE, LED_BLUE, LED_BLUE); // LED on
    }
    //GPIOPinWrite(GPIO_PORTF_BASE, LED_BLUE, 0); // LED off

    data_out->thing1 = data_buf.thing1_b[read];
	data_out->thing2 = data_buf.thing2_b[read];
	data_out->thing3 = data_buf.thing3_b[read];
    data_out->thing4 = data_buf.thing4_b[read];
    data_out->thing5 = data_buf.thing5_b[read];
    data_out->thing6 = data_buf.thing6_b[read];
    data_out->readhead = data_buf.readhead_b[read];
    data_out->writehead = data_buf.writehead_b[read];
	++read;
	if(read >= BUFLEN) // wraparound
    {
		read = 0;
	}
}

// write to the buffer and increment the write index by 1:
void buffer_write(  uint16_t    thing1,\
                    uint16_t    thing2,\
                    int16_t     thing3,\
                    int16_t     thing4,\
                    int16_t     thing5,\
                    uint8_t     thing6,\
                    uint16_t    readhead,\
                    uint16_t    writehead)
{
	if(!buffer_full()) { // if the buffer is full, the data is lost
		data_buf.thing1_b[write] = thing1;
		data_buf.thing2_b[write] = thing2;
		data_buf.thing3_b[write] = thing3;
        data_buf.thing4_b[write] = thing4;
        data_buf.thing5_b[write] = thing5;
        data_buf.thing6_b[write] = thing6;
        data_buf.readhead_b[write] = readhead;
        data_buf.writehead_b[write] = writehead;
		++write;
		if(write >= BUFLEN) // wraparound
        {
			write = 0;
		}
    }
    //else UARTprintf("Buffer is full.\n");
}
