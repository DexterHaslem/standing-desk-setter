#include "i2c.h"

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
    RTCCTL = 0x0000; /* clear RTC, not cleared on BOR (or power remove at all, surprising! */
    WDTCTL = WDTPW | WDTHOLD; /* stop watchdog */
    init_clk_16mhz();
    init_gpio();
    i2c_init();

    //uint8_t addr = (0x52 >> 1);
    //uint8_t data[32] = { 0xAA };

    __bis_SR_register(LPM0_bits + GIE);
	return 0;
}
