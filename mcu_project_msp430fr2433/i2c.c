/*
 * i2c.c
 *
 *  Created on: Jul 20, 2021
 *      Author: Dexter
 */

/* adapted from the i2c examples in resource explorer/mspware */
/* mostly around the fact we have variable register sizes because the vl531x is
 * extremely not good
 */
#include "i2c.h"

static enum eI2C_MODE mode = I2C_IDLE_MODE;

/* note: we use 16 bit regs, the vl53l1x has a mix */
static uint16_t tx_reg_addr = 0;

//static uint8_t rx_buf[MAX_RX_BUFFER_SIZE] = {0};
static uint8_t* tx_buf;
static uint8_t* rx_buf;

static uint16_t rx_count = 0;
static uint16_t rx_index = 0;

//static uint8_t tx_buf[MAX_TX_BUFFER_SIZE] = {0};
static uint16_t tx_count = 0;
static uint8_t tx_reg_count = 0;
static uint16_t tx_index = 0;

static volatile bool got_nack = false;

static bool nostop_mode = false;
bool i2c_got_nack(void)
{
    return got_nack;
}

/* TODO: simplify this stuff, cleanup copy pasta, maybe even remove tx register mode for non register devices.. */

enum eI2C_MODE i2c_write_nostop(uint8_t dev_addr, uint8_t *data, uint16_t count)
{
   nostop_mode = true;
   mode = I2C_TX_DATA_MODE;
   tx_reg_count = 0;
   rx_count = 0;
   tx_count = count;
   rx_index = 0;
   tx_index = 0;

   tx_buf = data;

   /* Initialize slave address and interrupts */
   UCB0I2CSA = dev_addr;
   UCB0IFG &= ~(UCTXIFG + UCRXIFG);       // Clear any pending interrupts
   UCB0IE &= ~UCRXIE;                     // Disable RX interrupt
   UCB0IE |= UCTXIE;                      // Enable TX interrupt

   UCB0CTLW0 |= UCTR + UCTXSTT;           // I2C TX, start condition
   __bis_SR_register(LPM0_bits + GIE);    // Enter LPM0 w/ interrupts

   return mode;
}

enum eI2C_MODE i2c_write_cont(uint8_t dev_addr, uint8_t *data, uint16_t count)
{
   nostop_mode = false;
   mode = I2C_TX_DATA_MODE;
   tx_reg_count = 0;
   rx_count = 0;
   tx_count = count;
   rx_index = 0;
   tx_index = 0;
   tx_buf = data;
   /* Initialize slave address and interrupts */
   UCB0I2CSA = dev_addr;
   UCB0IFG &= ~(UCTXIFG + UCRXIFG);       // Clear any pending interrupts
   UCB0IE &= ~UCRXIE;                     // Disable RX interrupt
   UCB0IE |= UCTXIE;                      // Enable TX interrupt

   //UCB0CTLW0 |= UCTR + UCTXSTT;           // I2C TX, start condition
   /* load buf with start of data */
   UCB0TXBUF = data[tx_count++];
   __bis_SR_register(LPM0_bits + GIE);    // Enter LPM0 w/ interrupts

   return mode;
}

enum eI2C_MODE i2c_write(uint8_t dev_addr, uint8_t *data, uint16_t count)
{
    nostop_mode = false;
    mode = I2C_TX_DATA_MODE;
    tx_reg_count = 0;
    rx_count = 0;
    tx_count = count;
    rx_index = 0;
    tx_index = 0;

    tx_buf = data;

    /* Initialize slave address and interrupts */
    UCB0I2CSA = dev_addr;
    UCB0IFG &= ~(UCTXIFG + UCRXIFG);       // Clear any pending interrupts
    UCB0IE &= ~UCRXIE;                     // Disable RX interrupt
    UCB0IE |= UCTXIE;                      // Enable TX interrupt

    UCB0CTLW0 |= UCTR + UCTXSTT;           // I2C TX, start condition
    __bis_SR_register(LPM0_bits + GIE);    // Enter LPM0 w/ interrupts

    return mode;
}

enum eI2C_MODE i2c_write_reg1(uint8_t dev_addr, uint8_t reg, uint8_t *data, uint16_t count)
{
    nostop_mode = false;
    mode = I2C_TX_REG_ADDRESS_MODE;
    tx_reg_count = 1;
    tx_reg_addr = reg;
    rx_count = 0;
    tx_count = count;
    rx_index = 0;
    tx_index = 0;

    tx_buf = data;

    /* Initialize slave address and interrupts */
    UCB0I2CSA = dev_addr;
    UCB0IFG &= ~(UCTXIFG + UCRXIFG);       // Clear any pending interrupts
    UCB0IE &= ~UCRXIE;                       // Disable RX interrupt
    UCB0IE |= UCTXIE;                        // Enable TX interrupt

    UCB0CTLW0 |= UCTR + UCTXSTT;             // I2C TX, start condition
    __bis_SR_register(LPM0_bits + GIE);              // Enter LPM0 w/ interrupts

    return mode;
}

enum eI2C_MODE i2c_write_reg2(uint8_t dev_addr, uint16_t reg, uint8_t *data, uint16_t count)
{
    nostop_mode = false;
    mode = I2C_TX_REG_ADDRESS_MODE;
    tx_reg_addr = reg;
    tx_reg_count = 2;
    rx_count = 0;
    tx_count = count;
    rx_index = 0;
    tx_index = 0;

    tx_buf = data;

    /* Initialize slave address and interrupts */
    UCB0I2CSA = dev_addr;
    UCB0IFG &= ~(UCTXIFG + UCRXIFG);         // Clear any pending interrupts
    UCB0IE &= ~UCRXIE;                       // Disable RX interrupt
    UCB0IE |= UCTXIE;                        // Enable TX interrupt

    UCB0CTLW0 |= UCTR + UCTXSTT;             // I2C TX, start condition
    __bis_SR_register(LPM0_bits + GIE);      // Enter LPM0 w/ interrupts

    return mode;
}

enum eI2C_MODE i2c_read(uint8_t dev_addr, uint8_t* dest, uint16_t count)
{
    mode = I2C_RX_DATA_MODE;
    tx_reg_count = 0;
    tx_count = 0;
    rx_count = count;
    rx_index = 0;
    tx_index = 0;
    rx_buf = dest;

    /* since we have no register to send, we can immediately go to rx mode */
    UCB0IFG &= ~(UCTXIFG + UCRXIFG);         // Clear any pending interrupts
    UCB0IE |= UCRXIE;              // Enable RX interrupt
    UCB0IE &= ~UCTXIE;             // Disable TX interrupt
    UCB0CTLW0 &= ~UCTR;            // Switch to receiver
    //UCB0CTLW0 |= UCTXSTT;          // Send start
    //UCB0CTLW0 |= UCTR + UCTXSTT;             // I2C TX, start condition
    __bis_SR_register(LPM0_bits + GIE);      // Enter LPM0 w/ interrupts

    return mode;
}

enum eI2C_MODE i2c_read_reg1(uint8_t dev_addr, uint8_t reg, uint8_t* dest, uint16_t count)
{
    mode = I2C_TX_REG_ADDRESS_MODE;
    tx_reg_addr = reg;
    tx_reg_count = 1;
    tx_count = 0;
    rx_count = count;
    rx_index = 0;
    tx_index = 0;
    rx_buf = dest;

    /* Initialize slave address and interrupts */
    UCB0I2CSA = dev_addr;
    UCB0IFG &= ~(UCTXIFG + UCRXIFG);         // Clear any pending interrupts
    UCB0IE &= ~UCRXIE;                       // Disable RX interrupt
    UCB0IE |= UCTXIE;                        // Enable TX interrupt

    UCB0CTLW0 |= UCTR + UCTXSTT;             // I2C TX, start condition
    __bis_SR_register(LPM0_bits + GIE);      // Enter LPM0 w/ interrupts

    return mode;
}

enum eI2C_MODE i2c_read_reg2(uint8_t dev_addr, uint16_t reg, uint8_t* dest, uint16_t count)
{
    mode = I2C_TX_REG_ADDRESS_MODE;
    tx_reg_addr = reg;
    tx_reg_count = 2;
    tx_count = 0;
    rx_count = count;
    rx_index = 0;
    tx_index = 0;
    rx_buf = dest;

    /* Initialize slave address and interrupts */
    UCB0I2CSA = dev_addr;
    UCB0IFG &= ~(UCTXIFG + UCRXIFG);       // Clear any pending interrupts
    UCB0IE &= ~UCRXIE;                       // Disable RX interrupt
    UCB0IE |= UCTXIE;                        // Enable TX interrupt

    UCB0CTLW0 |= UCTR + UCTXSTT;             // I2C TX, start condition
    __bis_SR_register(LPM0_bits + GIE);              // Enter LPM0 w/ interrupts

    return mode;
}

void i2c_init(void)
{
    UCB0CTLW0 = UCSWRST;                      // Enable SW reset
    UCB0CTLW0 |= UCMODE_3 | UCMST | UCSSEL__SMCLK | UCSYNC; // I2C master mode, SMCLK, sync mode
    // SMCLK = 16 MHz
    //UCB0BRW = 160;                            // fSCL = SMCLK/160 = ~100kHz
    UCB0BRW = 40;                            // fSCL = SMCLK/40 = ~400kHz
    UCB0CTLW0 &= ~UCSWRST;                    // Clear SW reset, resume operation
    // enable nack interrupts
    UCB0IE |= UCNACKIE;
}

#pragma vector = USCI_B0_VECTOR
__interrupt void i2c_isr(void)
{
  //Must read from UCB0RXBUF
  uint8_t rx_val = 0;
  switch(__even_in_range(UCB0IV, USCI_I2C_UCBIT9IFG))
  {
    case USCI_NONE:          break;         // Vector 0: No interrupts
    case USCI_I2C_UCALIFG:                  // Vector 2: ALIFG
        /* arbitration lost, dont get stuck */
        mode = I2C_IDLE_MODE;
        UCB0IE &= ~UCTXIE;                       // disable TX interrupt
        UCB0IE &= ~UCRXIE;                       // disable RX interrupt
        __bic_SR_register_on_exit(CPUOFF);      // Exit LPM0
        break;
    case USCI_I2C_UCNACKIFG:                // Vector 4: NACKIFG
        got_nack = true;
        UCB0CTLW0 |= UCTXSTP;     // Send stop condition
        mode = I2C_IDLE_MODE;
        UCB0IE &= ~UCTXIE;                       // disable TX interrupt
        UCB0IE &= ~UCRXIE;                       // disable RX interrupt
        __bic_SR_register_on_exit(CPUOFF);      // Exit LPM0
        break;
    case USCI_I2C_UCSTTIFG:  break;         // Vector 6: STTIFG
    case USCI_I2C_UCSTPIFG:  break;         // Vector 8: STPIFG
    case USCI_I2C_UCRXIFG3:  break;         // Vector 10: RXIFG3
    case USCI_I2C_UCTXIFG3:  break;         // Vector 12: TXIFG3
    case USCI_I2C_UCRXIFG2:  break;         // Vector 14: RXIFG2
    case USCI_I2C_UCTXIFG2:  break;         // Vector 16: TXIFG2
    case USCI_I2C_UCRXIFG1:  break;         // Vector 18: RXIFG1
    case USCI_I2C_UCTXIFG1:  break;         // Vector 20: TXIFG1
    case USCI_I2C_UCRXIFG0:                 // Vector 22: RXIFG0
        rx_val = UCB0RXBUF;
        if (rx_count)
        {
          rx_buf[rx_index++] = rx_val;
          rx_count--;
        }

        if (rx_count == 1)
        {
          UCB0CTLW0 |= UCTXSTP;
        }
        else if (rx_count == 0)
        {
          UCB0IE &= ~UCRXIE;
          mode = I2C_IDLE_MODE;
          __bic_SR_register_on_exit(CPUOFF);      // Exit LPM0
        }
        break;
    case USCI_I2C_UCTXIFG0:                 // Vector 24: TXIFG0
        switch (mode)
        {
          case I2C_TX_REG_ADDRESS_MODE:
              if (tx_reg_count > 1)
              {
                  /* two byte address, keep going in address mode next time */
                  UCB0TXBUF = (tx_reg_addr & 0xFF00) >> 8;
                  tx_reg_count--;
              }
              else
              {
                  UCB0TXBUF = tx_reg_addr & 0x00FF;
                  if (rx_count)
                      mode = I2C_SWITCH_TO_RX_MODE;   // Need to start receiving now
                  else
                      mode = I2C_TX_DATA_MODE;        // Continue to transmision with the data in Transmit Buffer
              }
              break;
          case I2C_SWITCH_TO_RX_MODE:
              UCB0IE |= UCRXIE;              // Enable RX interrupt
              UCB0IE &= ~UCTXIE;             // Disable TX interrupt
              UCB0CTLW0 &= ~UCTR;            // Switch to receiver
              mode = I2C_RX_DATA_MODE;       // State state is to receive data
              UCB0CTLW0 |= UCTXSTT;          // Send repeated start
              if (rx_count == 1)
              {
                  //Must send stop since this is the N-1 byte
                  while((UCB0CTLW0 & UCTXSTT));
                  UCB0CTLW0 |= UCTXSTP;      // Send stop condition
              }
              break;

          case I2C_TX_DATA_MODE:
              if (tx_count)
              {
                  UCB0TXBUF = tx_buf[tx_index++];
                  tx_count--;
              }
              else
              {
                  //Done with transmission
                  if (!nostop_mode)
                  {
                      UCB0CTLW0 |= UCTXSTP;     // Send stop condition
                  }
                  mode = I2C_IDLE_MODE;
                  UCB0IE &= ~UCTXIE;                       // disable TX interrupt
                  __bic_SR_register_on_exit(CPUOFF);      // Exit LPM0
              }
              break;

          default:
              __no_operation();
              break;
        }
        break;
    default: break;
  }
}
