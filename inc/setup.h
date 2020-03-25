//*************************************************************************************************
//
// setup.h
//
// Contains application-specific setup functions
//
//*************************************************************************************************

#ifndef SETUP_H
#define SETUP_H

// LED Macros
#define LED_RED     GPIO_PIN_1
#define LED_BLUE    GPIO_PIN_2  // BLUE on indicates buffer is empty
#define LED_GREEN   GPIO_PIN_3  // GREEN on indicates buffer is full

// Differential digital output macros
#define DIFF_OUT_0          GPIO_PIN_0  // PD0
#define DIFF_OUT_1          GPIO_PIN_1  // PD1
#define DIFF_OUT_0_REST     0
#define DIFF_OUT_1_REST     DIFF_OUT_1
#define DIFF_OUT_0_CONTACT  DIFF_OUT_0
#define DIFF_OUT_1_CONTACT  0
#define CONTACT_MAX_COUNT   20          // 2 --> 1 ms pulse

#define FLIGHT              0
#define STANCE              1

// function prototypes
void LED_Init(void);            // initializes RGB LEDs on GPIO Port F
void diff_dig_out_init(void);   // initializes PD{1:0} for digital outputs, sets default values

#endif
