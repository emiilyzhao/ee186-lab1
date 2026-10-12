# ee186_lab1

## 2. Flashing and Debugging Code

When the project is built, **.o** (one per source file), **.elf**, **.map**, and **.list** files are generated from the source files.
Each **.c** file is compiled into an **.o** file and linked with the linker script **.ld**, which says where flash and SRAM are, to produce executable and linkable format of the file **.elf**.

These are written into the MCU's internal flash memory, which starts at address *0x08000000*. This represents the region of memoery dedicated to code.
We write the vector table (SP, handlers), program code (**.text**), constants (**.rodata**), and intiial values of initialized global variables already written in code (**.data**).

STM32CubeIDE communicates with the board over a connection facilited by USB.
The ST-LINK debugger/programmer is internally built into the board (chip labeled "STM32F").
On my computer, STM32CubeIDE runs a gdb server, and the gdb debugger --------

The binary is transferred with serial wire debug **SWD**, a 2-wire debug protocol by ARM. This contains a **SWDIO** data wire and **SWCLK** clock wire. 
ST-LINK uses SWD to access the core's debug port, which pauses the CPU and allows for read/write memory and flashing the program into memory.

Overall, the build compiles and links files to produce the **lab1.elf** file. 
Meanwhile, CubeIDE on my computer starts the GDB server that connects to ST-LINK over the USB connection between the board and my computer.
ST-LINK connects to the baord over SWD and stops the core's processing to start the write process.
Any parts of flash memory that need to be wiped are erased, and then the program is written into the code sector of memory, starting at *0x08000000*.
The MCU is reset. In debug mode, we are now waiting to run the code or step through it.
