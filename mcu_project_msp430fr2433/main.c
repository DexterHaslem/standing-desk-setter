#include <msp430.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include "i2c.h"
#include "vl53l1x.h"
#include "ssd1306.h"

void delay_timer0_1ms(void)
{
    /* this is not super accurate because of sw polling overhead,
     * but good enough for dirty waits when running
     */
    TA0CCTL0 = 0;
#if 0


#endif
    /* ~1ms on 16 MHz */
    TA0CCR0 = 16100;

    while ((TA0CCTL0 & BIT0) == 0)
    {
       // wait overflow
    }
    TA0CCTL0 = 0;
   __no_operation();
}

static void init_gpio(void)
{
    P1DIR = 0xFF;
    P2DIR = 0xFF;
    P1OUT = 0;
    P2OUT = 0;

    P1SEL0 |= BIT2 | BIT3; /* P1.3, P1.2 to I2C SCL/SDA */
    P1SEL1 &= ~(BIT2 | BIT3);

    // Disable the GPIO power-on default high-impedance mode to activate
    // previously configured port settings
    PM5CTL0 &= ~LOCKLPM5;
}

static void init_clk_16mhz(void)
{
    // Configure one FRAM waitstate as required by the device datasheet for MCLK
    // operation beyond 8MHz _before_ configuring the clock system.
    FRCTL0 = FRCTLPW | NWAITS_1;

    // Clock System Setup
    __bis_SR_register(SCG0);                           // disable FLL
    CSCTL3 |= SELREF__REFOCLK;                         // Set REFO as FLL reference source
    CSCTL0 = 0;                                        // clear DCO and MOD registers
    CSCTL1 &= ~(DCORSEL_7);                            // Clear DCO frequency select bits first
    CSCTL1 |= DCORSEL_5;                               // Set DCO = 16MHz
    CSCTL2 = FLLD_0 + 487;                             // DCOCLKDIV = 16MHz
    __delay_cycles(3);
    __bic_SR_register(SCG0);                           // enable FLL
    while(CSCTL7 & (FLLUNLOCK0 | FLLUNLOCK1));         // FLL locked
}

int main(void)
{
    RTCCTL = 0x0000; /* clear RTC, not cleared on BOR (or power remove at all, surprising!) */
    WDTCTL = WDTPW | WDTHOLD; /* stop watchdog */
    TA0CTL |= TASSEL__SMCLK | MC__UP;  /* turn on timer a0, no interrupts, smclock up to mode , used for compare timers */

    init_clk_16mhz();
    init_gpio();
    i2c_init();

    ssd1306_init();

    ssd1306_pixel(0, 0);
    ssd1306_pixel(127, 0);
    ssd1306_pixel(0, 63);
    ssd1306_pixel(127, 63);

    ssd1306_str(30, 25, "hello world super long line to see if it wraps hehe");

    ssd1306_present_full();

#if 1
    bool sensor_initd = vl53l1x_init();
    if (sensor_initd)
    {
        //vl53l1x_start_ranging();
        /* this turns on launchpad led */
        P1OUT ^= BIT0;
    }
#endif
    __bis_SR_register(LPM0_bits + GIE);
	return 0;
}
