# Tiva_ring_buf
Example ring buffer application for Tiva microcontroller

## Usage
### Building and flashing the program
You can build this project using Make or using TI's Code Composer Studio (CCS).

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

You can enter any of these commands in your terminal emulator and, after pressing `ENTER`, your command will be sent to the Tiva.

#### Tangent/rant on printing floats over UART
Caution: the purpose of printing `xxxxxx.00` is to verify that the code _can_ print floating point numbers over the UART.
That doesn't mean it's always a good idea!
Printing a float over UART requires many clock cycles, so avoid it whenever speed matters.

For example, if your code is printing controller data over UART and formatting the data as floats, the data will print much slower than if they were formatted as integers. This in turn will require you to allocate more memory to the ring buffer shared between your control loop and the Tiva-to-PC code, which takes away from memory that you could allocate for something more important.

The obvious solution (not necessarily the most practical solution) is to code all the control computations using integer math so you never have to think about printing floats.
The Tiva's hardware floating-point unit (FPU) means you can get away with floating point math in your control loop, but it doesn't mean you can get away with printing floats over UART. Something to keep in mind.

</rant>

## Theory
