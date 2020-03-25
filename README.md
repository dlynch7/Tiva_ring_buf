# Tiva_ring_buf
Example ring buffer application for Tiva microcontroller

## Usage
### Building and flashing the program
You can build this project using Make or using TI's Code Composer Studio (CCS).

#### Using CCS

#### Using Make
If you choose to use Make, I have provided a Makefile in the project's root directory.
You may have to edit some variables in the Makefile:
* `XC_PATH`: the Makefile uses the GCC ARM cross compiler that CCS installs.
* `PORT`: this is your computer's serial port
* `TERMEMU`: this is your computer's terminal emulator
* `TIVAWARE_PATH`: this is the location of your Tivaware library
* `FLASH_PATH`: this is the location where you installed lm4flash or TI's flash programmer.

### Interacting with the program
1. Once you have built and flashed your project onto the Tiva, open up your terminal emulator:
    * If you're using this project's Makefile, just run `make screen`.
    * Otherwise, you can still figure out the serial port settings you need by looking at the `make screen` part of the Makefile.

If you don't see anything in your terminal emulator, press the RESET button on the Tiva devboard (depending on your flash programmer, the code may not run immediately after flashing, not until you reset the microcontroller).

2. By now, you should see a few lines of text that the Tiva printed to the serial port:
```
Hello, world!
Ring buffer initialized.
Control loop initialized.
foo = xxxxxx.00.
Setup complete!
```

where the `xxxxxx.00` will be replaced by some (large) random number with 2 decimal places.

The `main()` function in `main.c` implements a barebones text-based menu using a `switch` statement.
Here are the existing menu options and what they do:
* `e`: erases ring buffer contents
* `h`: prints the read index and the write index of the ring buffer
* `r`: record "data" (bogus data) from the control loop and send it back to the PC over UART
* `q`: "quit" (doesn't do anything for now, you can add your own functionality as you see fit)

You can enter any of these commands in your terminal emulator and, after pressing ENTER on your keyboard, your command will be sent to the Tiva.

#### Tangent/rant on printing floats over UART
Caution: the purpose of printing `xxxxxx.00` is to verify that the code _can_ print floating point numbers over the UART.
That doesn't mean it's always a good idea!
Printing a float over UART requires many clock cycles, so avoid it whenever speed matters.

For example, if your code is printing controller data over UART and formatting the data as floats, the data will print much slower than if they were formatted as integers. This in turn will require you to allocate more memory to the ring buffer shared between your control loop and the Tiva-to-PC code, which takes away from memory that you could allocate for something more important.

The obvious solution (not necessarily the most practical solution) is to code all the control computations using integer math so you never have to think about printing floats.
The Tiva's hardware floating-point unit (FPU) means you can get away with floating point math in your control loop, but it doesn't mean you can get away with printing floats over UART. Something to keep in mind.

`</rant>`

## Theory: ring buffers in real-time control applications
A ring buffer (aka a circular buffer) is a data structure that sits between two asynchronous processes.
[Wikipedia's page](https://en.wikipedia.org/wiki/Circular_buffer) provides a good description so I won't repeat it here.
Instead, I'll explain why ring buffers (especially this particular implementation) are useful for real-time control.

In a real-time control application, two common asynchronous processes are a fixed-frequency control loop and a data-logging process that sends control data back to a PC for post-processing and analysis.
Data needs to be shared between these two processes without slowing down the control loop to an unacceptably slow frequency to accomodate the data-logging process (data logging often uses formatted print statements and is therefore slow compared to the control frequency).

Enter the ring buffer: the control loop can write to the ring buffer at the controller frequency, and the data-logger can read from the ring buffer at its own pace; I'm refraining from using the term "frequency" with the data-logger because it might not run at a fixed frequency (nor does it need to, in most cases).

### Water tank analogy
As long as the buffer is "big enough", the whole system can run smoothly for some period of time.
This is just like those water tank problems from high school or freshman physics: data is "flowing" into the buffer at one rate and "flowing out of the buffer at a different (slower) rate.
Unless the buffer is infinitely large, it will eventually fill up, and new data from the control loop will be lost (imagine water spilling out of the tank).
When will this happen? It depends on the size of the buffer and the data "flow" rates.
The upper limit to your buffer size depends on the data types you are buffering and how much memory your microcontroller has available.

### What makes this ring buffer implementation so special?
Suppose your controller requires several different data types:
* your incremental rotary encoder records angles using the `uint16_t` type
* your motor amplifier accepts current commands in the `int32_t` type
* measurements from an analog temperature sensor on your actuator are converted to the `uint8_t` type
* an IMU reports back accelerations and gyro rates as `float`s.
The list can go on and on.

The sensible thing to do is pack all this data into a custom struct:
```C
typedef struct {
    uint16_t    encoder_delta_angle;
    int32_t     current_command;
    uint8_t     motor_temp;
    float       x_accel;
    float       x_gyro;
    // etc...
} control_data_t
```

The next sensible thing to do is buffer this struct:
```C
#define BUFLEN 1024 // is this too small? too big?

typdef struct{
    uint16_t    encoder_delta_angle[BUFLEN];
    int32_t     current_command[BUFLEN];
    uint8_t     motor_temp[BUFLEN];
    float       x_accel[BUFLEN];
    float       x_gyro[BUFLEN];
    // etc...

} control_data_buffer_t
```

This is exactly what is implemented in this project's `ring_buffer.{c,h}`.
In a fit of excess, I also buffered the write and read indices so you can see how they change as data is written to and read from the buffer.

To modify my ring buffer implementation to suit your needs,
* edit `typedef struct {...} buffer_t` and `typedef struct {...} buf_out_t` in `ring_buffer.h`,
* edit the function prototype for `buffer_write(...)` in `ring_buffer.h`,
* edit the function definitions for `buffer_read(...)` and `buffer_write(...)` in `ring_buffer.c`.
