
#ifndef VL53L1X_H
#define	VL53L1X_H

/* for _XTAL_FREQ */
#include "mcc_generated_files/mcc.h"

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

bool vl53l1x_init(void);
uint16_t vl53l1x_get_id(void);

#endif	/* VL53L1X */

