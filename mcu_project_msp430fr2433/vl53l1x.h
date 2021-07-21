
#ifndef VL53L1X_H
#define	VL53L1X_H

#include "i2c.h"
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* use an enum for long/short mode to make ever so slightly less stupid */
enum eVL53L1X_DIST_MODE //: uint8_t 
{
    VL53L1X_DIST_MODE_UNKNOWN,
    VL53L1X_DIST_MODE_SHORT,
    VL53L1X_DIST_MODE_LONG,
};
void delay_timer0_1ms(void);

bool vl53l1x_init(void);
uint16_t vl53l1x_get_id(void);
uint8_t vl53l1x_get_data_ready(void);

void vl53l1x_stop_ranging(void);
void vl53l1x_start_ranging(void);

uint16_t vl53l1x_get_dist(void);

void vl53l1x_clear_int(void);

void vl53l1x_set_dist_mode(enum eVL53L1X_DIST_MODE dm);
enum eVL53L1X_DIST_MODE vl53l1x_get_dist_mode(void);

void vl53l1x_set_timing_budget_ms(uint16_t tb);
uint16_t vl53l1x_get_timing_budget_ms(void);

#endif	/* VL53L1X */

