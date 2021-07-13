#include "mcc_generated_files/mcc.h"

#include <stdint.h>
#include <stdbool.h>

#define SLEEP_TIMEOUT_MS (5000)

static volatile uint8_t sleep_countdown = 0;

static void on_systick_timer(void)
{
    /* this is main operation scanning timer at ~25ms */
    //LATBbits.LATB4 = !PORTBbits.RB4;
    //IO_STATUS_LED_Toggle();
    //SLEEP();
}

static void on_heartbeat_timer(void)
{
    /* longer term twice a second timer to see if we can go back to sleep */
    if (--sleep_countdown < 1)
    {
        IO_STATUS_LED_SetHigh();
        SLEEP();
    }
        
}

static void on_input_pin(void)
{
    /* start timer to go back to sleep */
    IO_STATUS_LED_SetLow();
    sleep_countdown = (SLEEP_TIMEOUT_MS / 500);
}

void main(void)
{
    SYSTEM_Initialize();
    
    TMR0_SetInterruptHandler(on_systick_timer);
    TMR1_SetInterruptHandler(on_heartbeat_timer);
    //TMR0_StartTimer();
    //TMR1_Starttimer();
    
    IOCAF4_SetInterruptHandler(on_input_pin);
    IOCAF5_SetInterruptHandler(on_input_pin);
    
    INTERRUPT_GlobalInterruptEnable();
    INTERRUPT_PeripheralInterruptEnable();
    
    SLEEP();
    
    while (1)
    {
        
    }
}
