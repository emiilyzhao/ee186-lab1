#include <stdint.h>

// base address
// can we use #define instead? #define RCC_BASE    0x40021000UL
#define RCC_BASE    0x40021000UL
#define GPIOB_BASE  0x48000400UL
#define GPIOC_BASE  0x48000800UL

// offset
#define RCC_AHB2ENR_OFFSET  0x4CUL
#define GPIO_MODER_OFFSET   0x00UL
#define GPIO_ODR_OFFSET     0x14UL

// Bit masks
#define RCC_AHB2ENR_GPIOBEN (1UL << 1) // 10
#define RCC_AHB2ENR_GPIOCEN (1UL << 2) // 100

// registers, volatile because MMIO
volatile uint32_t *rcc_ahb2enr = (volatile uint32_t *)(RCC_BASE + RCC_AHB2ENR_OFFSET);
volatile uint32_t *gpioc_moder = (volatile uint32_t *)(GPIOC_BASE + GPIO_MODER_OFFSET);
volatile uint32_t *gpioc_odr = (volatile uint32_t *)(GPIOC_BASE + GPIO_ODR_OFFSET);
volatile uint32_t *gpiob_moder = (volatile uint32_t *)(GPIOB_BASE + GPIO_MODER_OFFSET);
volatile uint32_t *gpiob_odr = (volatile uint32_t *)(GPIOB_BASE + GPIO_ODR_OFFSET);

// delay function
void delay(uint32_t counter) {
	volatile uint32_t i = counter;
	while(i > 0)
		i--;
}

int main(void)
{
	// Part 4: Blinking LED in Assembly
	__asm__ volatile (
			/* we need to
			 * 1. enable clock for port B
			 * 2. configure GPIO pin
			 * 3. turn on blue LED
			 */

			// enable clock (0x40021000UL base, 0x4CUL offset) for port B (1UL << 1 mask)
			"LDR r0, =0x4002104C		\n\t" // load address of RCC to r0 (with 4C offset for RCC)
			"LDR r1, [r0]				\n\t" // load value of RCC into r1
			"ORR r1, r1, #0x2			\n\t" // set bit for clock on with mask
			"STR r1, [r0]				\n\t" // write value stored in r1 into address at r0
			"LDR r1, [r0]				\n\t" // read back for clock to run

			// configure GPIOB pin mode (0x48000400UL base, 0x00UL offset)
			"LDR r0, =0x48000400		\n\t" // load address of GPIOB to r0 (with 00 offset for MODER)
			"LDR r1, [r0]				\n\t" // load value of GPIOB into r1
			"BIC r1, r1, #(3 << 14)		\n\t" // clear bits, 3 << 14
			"ORR r1, r1, #(1 << 14)		\n\t" // set mode to general-purpose output 01, 1 << 14
			"STR r1, [r0]				\n\t" // write value stored in r1 into address at r0

			// write GPIOB output (0x48000400UL base, 0x14UL offset)
			"LDR r0, =0x48000414		\n\t" // load address of GPIOB_ODR to r0 (with 0x14 offset for ODR)
			"LDR r1, [r0]				\n\t" // load value of GPIOB_ODR into r1
			"ORR r1, r1, #(1 << 7) 		\n\t" // set pin 7 for PB7 high to turn on blue LED
			"STR r1, [r0]				\n\t" // write value stored in r1 into address at r0

			// cobbler list to show compiler what I changed
			:
			:
			: "r0", "r1", "memory" // registers changed, wrote MMIO as addresses belong to peripheral register

	);

	while (1);

	/****** Part 3: Blinking LEDs
	// green LED on PC7, blue LED on PB7, red LED on PB14, shift bit positions by 7/8
	// always &= with ~ OR |= to avoid modifying all pins

	// enable clocks for both ports
	*rcc_ahb2enr |= RCC_AHB2ENR_GPIOCEN | RCC_AHB2ENR_GPIOBEN;
	(void)*rcc_ahb2enr;

    // set mode as GPIO (01 - general purpose output), two bits for configuration so *2
    // clear this first because multi-pin
    *gpioc_moder &= ~(3UL << (7 * 2)); // to clear: & forces bits to 0, ~ for NOT, change 1s to 0s
    *gpioc_moder |= (1UL << (7 * 2)); // to set: | sets bits to 01
    *gpiob_moder &= ~(3UL << (7 * 2)); // to clear: & forces bits to 0, ~ for NOT, change 1s to 0s
    *gpiob_moder |= (1UL << (7 * 2)); // to set: | sets bits to 01
    *gpiob_moder &= ~(3UL << (14 * 2));  // to clear: & forces bits to 0, ~ for NOT, change 1s to 0s
    *gpiob_moder |= (1UL << (14 * 2)); // to set: | sets bits to 01

    while (1) {
    	// drive PC7 to high to turn green LED on
    	*gpioc_odr |= (1UL << (7));
    	delay(100000);
    	// clear PC7 to low to turn green LED off
    	*gpioc_odr &= ~(1UL << (7));

    	// drive PB7 to high to turn blue LED on
    	*gpiob_odr |= (1UL << (7));
    	delay(100000);
    	// clear PB7 to low to turn blue LED off
    	*gpiob_odr &= ~(1UL << (7));

    	// drive PB7 to high to turn red LED on
    	*gpiob_odr |= (1UL << (14));
    	delay(100000);
    	// clear PB7 to low to turn red LED off
    	*gpiob_odr &= ~(1UL << (14));
    }
    */
}
