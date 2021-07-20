#include "mcc_generated_files/mcc.h"

#include "vl53l1x.h"
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

#define SLEEP_TIMEOUT_MS (5000)

static volatile uint8_t sleep_countdown = 0;
static volatile bool clear_int = false;

static bool dist_setting_active = false;
static bool dist_sensor_initd = false;

static void reset_sleep_timer(void)
{
    sleep_countdown = (SLEEP_TIMEOUT_MS / 500);
}

static void awake_setup()
{
    /* start timer to go back to sleep */
    IO_STATUS_LED_SetLow();
    reset_sleep_timer();
    IO_OUT_VSHTDN_SetHigh();
}

static void enter_sleep(void)
{   
    IO_STATUS_LED_SetHigh();    
    IO_OUT_VSHTDN_SetLow();
    NOP();
    NOP();
    SLEEP();
}

static void on_sensor_data(void)
{
    clear_int = true;    
}

static void on_systick_timer(void)
{
    /* this is main operation scanning timer at ~25ms */
    //LATBbits.LATB4 = !PORTBbits.RB4;
    //IO_STATUS_LED_Toggle();
    //SLEEP();
    NOP();
}

static void on_heartbeat_timer(void)
{
    NOP();
    
    /* longer term twice a second timer to see if we can go back to sleep */
    if (!dist_setting_active)
    {
        if (--sleep_countdown < 1)
        {
            enter_sleep();
        }
    }
}

static void on_input_pin(void)
{
    if (sleep_countdown == 0)
    {
        awake_setup();       
    }
    else
    {
        reset_sleep_timer();
    }
}

void main(void)
{
    SYSTEM_Initialize();
    
    TMR0_SetInterruptHandler(on_systick_timer);
    TMR1_SetInterruptHandler(on_heartbeat_timer);    
    //TMR0_StartTimer();
    //TMR1_Starttimer();
    
    IOCCF3_SetInterruptHandler(on_sensor_data);
    IOCCF4_SetInterruptHandler(on_input_pin);
    IOCCF5_SetInterruptHandler(on_input_pin);
    
    INTERRUPT_GlobalInterruptEnable();
    INTERRUPT_PeripheralInterruptEnable();
    
    IO_OUT_VSHTDN_SetLow();
    __delay_ms(25);
    
    dist_sensor_initd = vl53l1x_init();
    
    if (dist_sensor_initd)
    {
        vl53l1x_set_dist_mode(VL53L1X_DIST_MODE_SHORT);
        vl53l1x_set_timing_budget_ms(15);
        vl53l1x_start_ranging();
    }
    
    //enter_sleep();
    
    while (1)
    {
        NOP();
        if (clear_int)
        {
            vl53l1x_clear_int();
            clear_int = false;
        }
#if 0
        if (sensor_init)
        { 
            while (vl53l1x_get_data_ready() == 0)
                NOP();
                //__delay_ms(1);
            
            uint16_t dist = vl53l1x_get_dist();
            vl53l1x_clear_int();
            
            printf("\rdist = %hu mm       \r", dist);
        }
#endif 
        //__delay_ms(50);
    }   
}
