#include "mcc_generated_files/mcc.h"

#include "vl53l1x.h"
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

#define SLEEP_TIMEOUT_MS (5000)

static volatile uint8_t sleep_countdown = 0;
static volatile bool clear_int = false;
static uint16_t last_dist_mm = 0;

static bool dist_setting_active = false;
static bool dist_sensor_initd = false;
static volatile bool do_wakeup = false;
static volatile bool do_enter_sleep = false;
static volatile bool ranging_active = false;
static volatile bool do_input_pin = false;

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
    
    /* reset timers in case there was already some data in counters */
    /* dont do a full initialize cuz that nukes our callbacks */
    //TMR0_Initialize();
    //TMR1_Initialize();
    
    dist_sensor_initd = vl53l1x_init();
    
    if (dist_sensor_initd)
    {
        vl53l1x_set_dist_mode(VL53L1X_DIST_MODE_SHORT);
        vl53l1x_set_timing_budget_ms(50);
    }
    
    TMR1_StartTimer();
}

static void enter_sleep(void)
{   
    if (dist_sensor_initd)
    {
        if (ranging_active)
        {
            vl53l1x_stop_ranging();
            ranging_active = false;
        }
        vl53l1x_clear_int();
    }
    clear_int = false;
    
    IO_STATUS_LED_SetHigh();
    
    TMR0_StopTimer();
    TMR1_StopTimer();    
    NOP();
    NOP();
    IO_OUT_VSHTDN_SetLow();
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
            do_enter_sleep = true;            
        }
    }
}

static void on_input_pin(void)
{
    if (sleep_countdown == 0)
    {
        do_wakeup = true;     
    }
    else
    {
        reset_sleep_timer();
        do_input_pin = true;
    }
}

void main(void)
{
    SYSTEM_Initialize();
      
    TMR0_SetInterruptHandler(on_systick_timer);
    TMR1_SetInterruptHandler(on_heartbeat_timer);    
    
    IOCCF4_SetInterruptHandler(on_input_pin);
    IOCCF5_SetInterruptHandler(on_input_pin);
    
    INTERRUPT_GlobalInterruptEnable();
    INTERRUPT_PeripheralInterruptEnable();       
        
    IOCCF3_SetInterruptHandler(on_sensor_data);
    
    enter_sleep();
    
    while (1)
    {
        NOP();
        if (clear_int)
        {
            last_dist_mm = vl53l1x_get_dist();
            vl53l1x_clear_int();            
            clear_int = false;
            reset_sleep_timer();
        }
        
        if (do_wakeup)
        {
            /* if we hit this, it will be true for the first timer tick */
            awake_setup();
            do_wakeup = false;
        }
        else if (do_enter_sleep)
        {
            do_enter_sleep = false;
            enter_sleep();
            
        }
        else if (do_input_pin)
        {
            if (!ranging_active)
            {
                vl53l1x_start_ranging();
                ranging_active = true;
            }
            
            /* TODO: long press for set, tap to range to */
            do_input_pin = false;
        }
        
    }   
}
