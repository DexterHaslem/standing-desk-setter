/*
 * ssd1306.h
 *
 *  Created on: Jul 25, 2021
 *      Author: Dexter
 */

#ifndef SSD1306_H_
#define SSD1306_H_

#include "i2c.h"
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>


void ssd1306_init(void);
void ssd1306_present_full(void);

#endif /* SSD1306_H_ */
