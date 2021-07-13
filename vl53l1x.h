
#ifndef VL53L1X_H
#define	VL53L1X_H

/* for _XTAL_FREQ */
#include "mcc_generated_files/mcc.h"

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

bool vl53l1x_init(void);
uint16_t vl53l1x_get_id(void);
uint8_t vl53l1x_get_data_ready(void);
void vl53l1x_stop_ranging(void);
void vl53l1x_start_ranging(void);
uint16_t vl53l1x_get_dist(void);
void vl53l1x_clear_int(void);

#endif	/* VL53L1X */

