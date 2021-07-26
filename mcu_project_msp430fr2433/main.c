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
        __no_operation();
    }
    TA0CCTL0 = 0;
   __no_operation();
}

#pragma vector = PORT2_VECTOR
__interrupt void btn_in_isr(void)
{
    PM5CTL0 &= ~LOCKLPM5;
    /* clear flag we done */
    P2IFG = 0x00;
    /* PMM auto starts */
    LPM4_EXIT;
}

static void init_gpio(void)
{
    P1DIR = 0xFF;
    P2DIR = 0xFF;
    P1OUT = 0;
    P2OUT = 0;

    P1SEL0 |= BIT2 | BIT3; /* P1.3, P1.2 to I2C SCL/SDA */
    P1SEL1 &= ~(BIT2 | BIT3);

    /* set the two launchpad buttons on p2.3, p2.7 as inputs (0). they have no alt mode */
    const uint8_t btns = (BIT3 | BIT7);

    P2DIR &= ~btns;
    /* additionally, on the launchpad, the switches go to ground with no ext pull, enable weak pullup */
    P2OUT |= btns;
    P2REN = btns;
    P2IFG = 0x00;
    /* set button interrupt on high->low transistion (weak pull up'd)*/
    P2IES = btns;
    /* enable interrupt on buttons */
    P2IE |= btns;

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

static bool devices_initd = false;

static void enter_deep_sleep(void)
{
    /* uncomment for full LPM4.5. note registers + ram lost
    PMMCTL0_H = PMMPW_H; // open PMM
    PMMCTL0_L |= PMMREGOFF; // set Flag to enter LPM4.5 with LPM4 request
    */
    if (devices_initd)
    {
        ssd1306_sleep();

        /* TODO: vl513l1x sleep pin down */
    }
    __bis_SR_register(LPM4_bits + GIE);
}

static void exit_deep_sleep(void)
{
    P1OUT ^= BIT0;

    i2c_init();
    /* init devices incase this was first we woke up */
    if (!devices_initd)
    {
        ssd1306_init();
        vl53l1x_init();
        devices_initd = true;
    }
    ssd1306_awake();
    /* start timer for going back to sleep if nothing happens */
}

int main(void)
{
    RTCCTL = 0x0000; /* clear RTC, not cleared on BOR (or power remove at all, surprising!) */
    WDTCTL = WDTPW | WDTHOLD; /* stop watchdog */
    TA0CTL |= TASSEL__SMCLK | MC__UP;  /* turn on timer a0, no interrupts, smclock up to mode , used for compare timers */

    init_clk_16mhz();
    init_gpio();

    while (1)
    {
        enter_deep_sleep();

        P2IFG = 0x00;
        P2IE &= ~(BIT1 | BIT7);

        /* if we hit this point, we were just awakened by a button. ghetto debounce as hw has none. tiny pb is super noises at edge*/
        for (uint8_t i = 125; i > 0; --i)
            delay_timer0_1ms();

        exit_deep_sleep();
        P2IE |= (BIT1 | BIT7);

        //ssd1306_str(1,  1, "i woke up");
        //ssd1306_present_full();
        __no_operation();
        __no_operation();
    }

    /* should never hit this */
    __no_operation();
	return 0;
}
