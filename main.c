#include "mcc_generated_files/mcc.h"

#include "vl53l1x.h"
#include <stdint.h>
#include <stdio.h>
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
   
    __delay_ms(5);
    
    bool sensor_init = vl53l1x_init();
    if (!sensor_init)
    {
        printf("failed to init sensor\r\n");
    }
    else
    {
        printf("sensor initialized\r\n");
        uint16_t device_id = vl53l1x_get_id();
        printf("device id: 0x%04X\n", device_id);
    }
    
    SLEEP();
    
    vl53l1x_start_ranging();
    
    while (1)
    {
        
        if (sensor_init)
        { 
            while (vl53l1x_get_data_ready() == 0)
                __delay_ms(1);
            
            uint16_t dist = vl53l1x_get_dist();
            vl53l1x_clear_int();
            
            //printf("\rdist = %hu mm       \r", dist);
        }
        
        __delay_ms(25);
    }
    
    vl53l1x_stop_ranging();
}
