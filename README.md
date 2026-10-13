# ee186_lab1

## 2. Flashing and Debugging Code

When the project is built, **.o** (one per source file), **.elf**, **.map**, and **.list** files are generated from the source files. Each **.c** file is compiled into an **.o** file. These are linked using the linker script **.ld** as instructions to where flash and SRAM are, to produce the executable and linkable format of the **.elf** file.

The **.elf** file is written into the MCU's internal flash memory, which starts at address *0x08000000*. This represents the region of memory dedicated to code. We write the vector table (SP, handlers), program code (**.text**), constants (**.rodata**), and intiial values of initialized global variables already written in code (**.data**).

On my computer, STM32CubeIDE runs a gdb server to communicate with the hardware, and the gdb debugger connects to this server through a local network port.  The ST-LINK debugger/programmer is internally built into the board (chip labeled "STM32F"). The instructions are sent from the server to the ST-LINK over a connection facilitated by USB, and finally to the MCU in binary by the two SWD wires.

The binary is transferred with serial wire debug **SWD**, a 2-wire debug protocol by ARM. This contains a **SWDIO** data wire and **SWCLK** clock wire. ST-LINK uses SWD to access the core's debug port, which pauses the CPU and allows for read/write memory and flashing the program into memory.

Overall, the build compiles and links files to produce the **lab1.elf** file. Then, CubeIDE on my computer starts the GDB server that connects to ST-LINK over the USB connection between the board and my computer. ST-LINK connects to the board over SWD and stops the core's processing to start the write process. Any parts of flash memory that need to be wiped are erased, and then the program is written into the code sector of memory, starting at *0x08000000*. The MCU is reset. In debug mode, we are now waiting to run the code or step through it, as we have stopped at a breakpoint at the start of main.


## 3. Blinking LEDs

User LEDs **LD1**, **LD2**, and **LD3** are available on the NUCLEO-L4R5ZI-P board.

The pin name associated for each LED are: LD1 (green) 	: **PC7**, LD2 (blue) 	: **PB7**, LD3 (red)	: **PB14**. Yes, these pins are configured as GPIOs, which means that they are in general-purpose mode and the code sets its level directly by writing to the GPIO output register instead of a peripheral controlling it.Output. The code from the MCU is driving the pin to high at 3.3V to light the LED by pushing current through it and its associated resistor. To turn it off, the MCU drives the pin to low at 0V. An input pin only reads a voltage. MODER = 01 for output.       
