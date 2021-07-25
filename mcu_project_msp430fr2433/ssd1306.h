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
void ssd1306_pixel(uint8_t x, uint8_t y);
void ssd1306_char(uint8_t x, uint8_t y, uint8_t ch);
void ssd1306_str(uint8_t x, uint8_t y, char *str);

void ssd1306_sleep(void);
void ssd1306_awake(void);

#endif /* SSD1306_H_ */
