#include "mcc_generated_files/mcc.h"

static void on_systick(void)
{
    LATBbits.LATB4 = !PORTBbits.RB4;
    SLEEP();
}

void main(void)
{
    SYSTEM_Initialize();
    
    TMR6_SetInterruptHandler(on_systick);
    TMR6_StartTimer();
    
    INTERRUPT_GlobalInterruptEnable();
    INTERRUPT_PeripheralInterruptEnable();
    
    //SLEEP();
    
    while (1)
    {
        
    }
}
