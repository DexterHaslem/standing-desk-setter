#include <msp430.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include "i2c.h"
#include "vl53l1x.h"
#include "ssd1306.h"
#include "logic.h"


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
__interrupt void p2_interrupt(void)
{
    PM5CTL0 &= ~LOCKLPM5;
    /* clear flag we done */
    P2IFG = 0x00;
    /* PMM auto starts */
    /* wake from any sleep */
    LPM4_EXIT;
}

static void vl513lx_set_shutdown(bool sd)
{
    /* shutdown has active high marker.. but you pull it low to shutdown */
    /* P2.1 */
    if (!sd)
    {
        P2OUT |= BIT1;
    }
    else
    {
        P2OUT &= ~(BIT1);
    }
}

static void init_gpio(void)
{
    P1DIR = 0xFF;
    P2DIR = 0xFF;
    P1OUT = 0;
    P2OUT = 0;

    P1SEL0 |= BIT2 | BIT3; /* P1.3, P1.2 to I2C SCL/SDA */
    P1SEL1 &= ~(BIT2 | BIT3);

    /* P2.1 to shutdown vl53l1x, output, already set output. drive high */
    vl513lx_set_shutdown(false);

    /* set the two launchpad buttons on p2.3, p2.7 as inputs (0). they have no alt mode */
    /* P2.0 to interrupt pin on vl53l1x, input */
    const uint8_t btns = (BIT3 | BIT7);
    const uint8_t p2in_ints = (BIT0 | btns);
    P2DIR &= ~p2in_ints;
    /* additionally, on the launchpad, the switches go to ground with no ext pull, enable weak pullup */
    P2OUT |= btns;
    P2REN = btns;
    P2IFG = 0x00;

    /* set button interrupt on high->low transistion (weak pull up'd)*/
    P2IES |= btns;

    /* enable interrupt on buttons and vl53l1x interrupt */
    P2IE |= p2in_ints;

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

static void enter_deep_sleep(void)
{
    /* uncomment for full LPM4.5. note registers + ram lost
    PMMCTL0_H = PMMPW_H; // open PMM
    PMMCTL0_L |= PMMREGOFF; // set Flag to enter LPM4.5 with LPM4 request
    */
    ssd1306_sleep();
    vl513lx_set_shutdown(true);
    P1OUT &= ~(BIT0);
    __bis_SR_register(LPM4_bits + GIE);
}

static void exit_deep_sleep(void)
{
    P1OUT |= BIT0;

    /* do this before, it will not ack when shutdown, incase never woke */
    vl513lx_set_shutdown(false);
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

    i2c_init();
    //ssd1306_init();
    vl53l1x_init();

    vl53l1x_set_dist_mode(VL53L1X_DIST_MODE_SHORT);
    vl53l1x_set_timing_budget_ms(500);

    vl53l1x_start_ranging();

    while (1)
    {
        /* enter lpm0 with ints, wake on ranging data ready */
        __bis_SR_register(LPM0_bits + GIE);

        /* we woke up, vl53l1x has data */
        vl53l1x_clear_int();

        P1OUT ^= BIT0;
        //update();
#if 0
        enter_deep_sleep();

        /* if we hit this point, we were just awakened by a button. ghetto debounce as hw has none. tiny pb is super noises at edge*/

        P2IFG = 0x00;
        P2IE &= ~(BIT3 | BIT7);

        for (uint8_t i = 125; i > 0; --i)
            delay_timer0_1ms();

        exit_deep_sleep();
        P2IE |= (BIT3 | BIT7);

        //ssd1306_str(1,  1, "i woke up");
        //ssd1306_present_full();
        __no_operation();
        __no_operation();
#endif
    }

    /* should never hit this */
    __no_operation();
	return 0;
}
