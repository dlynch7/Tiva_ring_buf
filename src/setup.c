#include "setup.h"

#include <math.h>
#include <stdbool.h>
#include <stdint.h>

#include "inc/hw_memmap.h"
#include "inc/hw_uart.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"
#include "utils/uartstdio.h"

//Setup LED on port F
void LED_Init(void)
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF); // enable the GPIO port used for the on-board LED
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOF)); // check if peripheral access enabled
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, LED_RED); // enable GPIO pin for red LED (PF1) as ouput
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, LED_BLUE); // enable GPIO pin for blue LED (PF2) as ouput
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, LED_GREEN); // enable GPIO pin for green LED (PF3) as ouput
    GPIOPinWrite(GPIO_PORTF_BASE, LED_RED, 0x0); // RED LED OFF
    GPIOPinWrite(GPIO_PORTF_BASE, LED_BLUE, 0x0); // BLUE LED OFF
    GPIOPinWrite(GPIO_PORTF_BASE, LED_GREEN, 0x0); // GREEN LED OFF
    UARTprintf("LED on port F initialized.\n"); // assumes InitConsole() has already been called.
}

void diff_dig_out_init(void)
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD); // enable GPIO Port D
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOD)); // wait until ready
    GPIOPinTypeGPIOOutput(GPIO_PORTD_BASE, DIFF_OUT_0); // enable differential digital output PD0
    GPIOPinTypeGPIOOutput(GPIO_PORTD_BASE, DIFF_OUT_1); // enable differential digital output PD1
    GPIOPinWrite(GPIO_PORTD_BASE, DIFF_OUT_0, DIFF_OUT_0_REST); // set 'rest' value (non-contact)
    GPIOPinWrite(GPIO_PORTD_BASE, DIFF_OUT_1, DIFF_OUT_1_REST); // set 'rest' value (non-contact)
    UARTprintf("Differential digital output on PD{1:0} initialized.\n");
}
