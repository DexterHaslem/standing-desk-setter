/*
 * ssd1306.c
 *
 *  Created on: Jul 25, 2021
 *      Author: Dexter
 */
#include "ssd1306.h"

/* SSD1306 driver mostly adapted from common libraries like Adafruit_SSD1306, the sparkfun stuff, etc */

#define DISP_ADDR (0x3C) // (0x78 >> 1)

#define WIDTH   (128)
#define HEIGHT  (64)

static uint8_t display_buffer[WIDTH * ((HEIGHT + 7) / 8)];/* oops.. WIDTH * HEIGHT];, bit stride */

#define SSD1306_MEMORYMODE          (0x20)
#define SSD1306_COLUMNADDR          (0x21)
#define SSD1306_PAGEADDR            (0x22)
#define SSD1306_SETCONTRAST         (0x81)
#define SSD1306_CHARGEPUMP          (0x8D)
#define SSD1306_SEGREMAP            (0xA0)
#define SSD1306_DISPLAYALLON_RESUME (0xA4)
#define SSD1306_DISPLAYALLON        (0xA5)
#define SSD1306_NORMALDISPLAY       (0xA6)
#define SSD1306_INVERTDISPLAY       (0xA7)
#define SSD1306_SETMULTIPLEX        (0xA8)
#define SSD1306_DISPLAYOFF          (0xAE)
#define SSD1306_DISPLAYON           (0xAF)
#define SSD1306_COMSCANINC          (0xC0)
#define SSD1306_COMSCANDEC          (0xC8)
#define SSD1306_SETDISPLAYOFFSET    (0xD3)
#define SSD1306_SETDISPLAYCLOCKDIV  (0xD5)
#define SSD1306_SETPRECHARGE        (0xD9)
#define SSD1306_SETCOMPINS          (0xDA)
#define SSD1306_SETVCOMDETECT       (0xDB)
#define SSD1306_SETLOWCOLUMN        (0x00)
#define SSD1306_SETHIGHCOLUMN       (0x10)
#define SSD1306_SETSTARTLINE        (0x40)
#define SSD1306_EXTERNALVCC         (0x01)
#define SSD1306_SWITCHCAPVCC        (0x02)

#define SSD1306_RIGHT_HORIZONTAL_SCROLL 0x26              ///< Init rt scroll
#define SSD1306_LEFT_HORIZONTAL_SCROLL 0x27               ///< Init left scroll
#define SSD1306_VERTICAL_AND_RIGHT_HORIZONTAL_SCROLL 0x29 ///< Init diag scroll
#define SSD1306_VERTICAL_AND_LEFT_HORIZONTAL_SCROLL 0x2A  ///< Init diag scroll
#define SSD1306_DEACTIVATE_SCROLL 0x2E                    ///< Stop scroll
#define SSD1306_ACTIVATE_SCROLL 0x2F                      ///< Start scroll
#define SSD1306_SET_VERTICAL_SCROLL_AREA 0xA3             ///< Set scroll range


static void cmd(uint8_t c)
{
    static uint8_t scratch;
    scratch = c;
    /* just treat command as reg 0 */
    i2c_write_reg1(DISP_ADDR, 0x00, &scratch, 1);
}

static void cmd1(uint8_t c, uint8_t a)
{
    static uint8_t scratch[2];
    scratch[0] = c;
    scratch[1] = a;
    /* just treat command as reg 0 */
    i2c_write_reg1(DISP_ADDR, 0x00, (uint8_t*) &scratch, 2);
}


static void cmd2(uint8_t c, uint8_t a1, uint8_t a2)
{
    static uint8_t scratch[3];
    scratch[0] = c;
    scratch[1] = a1;
    scratch[2] = a2;
    /* just treat command as reg 0 */
    i2c_write_reg1(DISP_ADDR, 0x00, (uint8_t*)&scratch, 3);

}

void ssd1306_init(void)
{
    cmd(SSD1306_DISPLAYOFF);
    cmd(SSD1306_CHARGEPUMP);
    cmd1(SSD1306_SETPRECHARGE, 0x33);
    cmd1(SSD1306_SETCONTRAST, 0x8A);

    cmd1(SSD1306_SETDISPLAYCLOCKDIV, 0x80); /* display ratio to recommended 0x80 */
    cmd1(SSD1306_SETMULTIPLEX, HEIGHT - 1);
    cmd1(SSD1306_SETDISPLAYOFFSET, 0x00); /* no offset */
    cmd(SSD1306_SETSTARTLINE | 0x00);
    cmd1(SSD1306_MEMORYMODE, 0x00); /* ks0108, memory mode 0 horizontal addressing */
    cmd(SSD1306_SEGREMAP | 0x01); /* segement remap */
    cmd(SSD1306_COMSCANDEC);

    cmd1(SSD1306_SETCOMPINS, 0x12);
    //cmd1(SSD1306_SETCOMPINS, 0xDA);
    cmd1(SSD1306_SETVCOMDETECT, 0x40);

    cmd(SSD1306_DISPLAYALLON_RESUME);
    cmd(SSD1306_NORMALDISPLAY);
    cmd(SSD1306_DEACTIVATE_SCROLL);
    cmd(SSD1306_DISPLAYON);

    /* start at 0,0 incase we were left somewhere else */
    cmd2(SSD1306_PAGEADDR, 0x00, 0xFF);
    cmd2(SSD1306_COLUMNADDR, 0x00, WIDTH - 1);

    //memset(display_buffer, 0xff, sizeof(display_buffer));
}

void ssd1306_pixel(uint16_t x, uint16_t y)
{
    display_buffer[x + (y/8)*WIDTH] |= (1 << (y&7));
}

/* presents display buffer to device. this will send all width*height data to device for full redraw */
void ssd1306_present_full(void)
{
    /* start at 0,0 incase we were left somewhere else */
    cmd2(SSD1306_PAGEADDR, 0x00, 0xFF);
    cmd2(SSD1306_COLUMNADDR, 0x00, WIDTH - 1);

    /* barf entire display content over single tx */
    /* note: dont use cmd for this, its not 0x00, just raw 0x40 first */
    uint8_t start_data[1] = {0x40};
    i2c_write_nostop(DISP_ADDR, &start_data[0], 1);
    i2c_write_cont(DISP_ADDR, display_buffer, sizeof(display_buffer));
}
