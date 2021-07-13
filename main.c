#include "mcc_generated_files/mcc.h"

#include <stdint.h>
#include <stdbool.h>

static void on_systick_timer(void)
{
    /* this is main operation scanning timer at ~25ms */
    //LATBbits.LATB4 = !PORTBbits.RB4;
    //IO_STATUS_LED_Toggle();
    SLEEP();
}

static void on_heartbeat_timer(void)
{
    /* longer term twice a second timer to see if we can go back to sleep */
    SLEEP();
}

void main(void)
{
    SYSTEM_Initialize();
    
    TMR0_SetInterruptHandler(on_systick_timer);
    TMR1_SetInterruptHandler(on_heartbeat_timer);
    //TMR0_StartTimer();
    //TMR1_Starttimer();
    
    INTERRUPT_GlobalInterruptEnable();
    INTERRUPT_PeripheralInterruptEnable();
    
    //SLEEP();
    
    while (1)
    {
        
    }
}
