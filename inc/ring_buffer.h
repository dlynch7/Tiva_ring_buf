#ifndef RING_BUFFER_H
#define RING_BUFFER_H
// Header file for ring_buffer.c
// helps implement a circular buffer

// ring_buffer.c is based on a program of the same name
// in Chapter 11 (UART) of NU's mechatronics textbook.
// Here I adapt it into a library for the TM4C123G...

#include <stdio.h>
#include <stdint.h>

#define BUFLEN 128 // 
#define DECIMATION 1 // only store every DECIMATION samples in the circular buffer
#define NSAMPLES (400/DECIMATION)

typedef struct {
    uint16_t    thing1_b[BUFLEN];
    uint16_t    thing2_b[BUFLEN];
    int16_t     thing3_b[BUFLEN];
    int16_t     thing4_b[BUFLEN];
    int16_t     thing5_b[BUFLEN];
    uint8_t     thing6_b[BUFLEN];
    uint16_t    readhead_b[BUFLEN]; // we're even buffering the read head!
    uint16_t    writehead_b[BUFLEN];// and the write head! Just for devel purposes.
} buffer_t;

typedef struct {
    uint16_t    thing1;
    uint16_t    thing2;
    int16_t     thing3;
    int16_t     thing4;
    int16_t     thing5;
    uint8_t     thing6;
    uint16_t    readhead;
    uint16_t    writehead;
} buf_out_t;

void buffer_reset(void); // resets read and write indices
void buffer_erase(void); // erases buffer by setting everything to 0
uint16_t get_read_index(void); // return the value of the read index
uint16_t get_write_index(void);// return the value of the write index
uint8_t buffer_empty(void); // return true if the buffer is empty (read=write)
uint8_t buffer_full(void);  // return true if the buffer is full

// reads from current buffer index and increment read index by 1:
void buffer_read(buf_out_t *);
// write to the buffer and increment the write index by 1:
void buffer_write(  uint16_t,\
                    uint16_t,\
                    int16_t,\
                    int16_t,\
                    int16_t,\
                    uint8_t,\
                    uint16_t,\
                    uint16_t);

#endif
