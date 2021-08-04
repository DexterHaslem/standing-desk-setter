/*
 * File:   vl53l1x.c
 * Author: Dexter
 *
 * Created on July 13, 2021, 4:12 AM
 */

#include "vl53l1x.h"

/* address 0x29 over the wire after r/w bit shift */
#define I2C_ADDR (0x52 >> 1)

/* dont set this too low or else it will fail errornously, it does take a bit */
#define INIT_TIMEOUT_MS (140)


#define PHASECAL_CONFIG__TIMEOUT_MACROP     (0x004B)
#define RANGE_CONFIG__VCSEL_PERIOD_A        (0x0060)
#define RANGE_CONFIG__VCSEL_PERIOD_B        (0x0063)
#define RANGE_CONFIG__VALID_PHASE_HIGH      (0x0069)
#define SD_CONFIG__WOI_SD0                  (0x0078)
#define SD_CONFIG__INITIAL_PHASE_SD0        (0x007A)
#define RANGE_CONFIG__TIMEOUT_MACROP_A_HI   (0x005E)
#define RANGE_CONFIG__TIMEOUT_MACROP_B_HI  	(0x0061)

#define VL53L1_SYSTEM__INTERMEASUREMENT_PERIOD              (0x006C)
#define VL53L1_RESULT__OSC_CALIBRATE_VAL                    (0x00DE)

/* NOTE: there is virtually no error checking, no nack checks on reg writes
 * live fast and die young
 */

static volatile uint16_t write_scratch;

/* defines borrowed from vl53l1x_class.cpp - see LICENSES */

/* this list of register values is sent once during init, from 
 register 0x2d to 0x87 */
const uint8_t default_cfgs[] =  {
    0x00, /* 0x2d : set bit 2 and 5 to 1 for fast plus mode (1MHz I2C), else don't touch */
	0x01, /* 0x2e : bit 0 if I2C pulled up at 1.8V, else set bit 0 to 1 (pull up at AVDD) */
	0x01, /* 0x2f : bit 0 if GPIO pulled up at 1.8V, else set bit 0 to 1 (pull up at AVDD) */
	0x01, /* 0x30 : set bit 4 to 0 for active high interrupt and 1 for active low (bits 3:0 must be 0x1), use SetInterruptPolarity() */
	0x02, /* 0x31 : bit 1 = interrupt depending on the polarity, use CheckForDataReady() */
	0x00, /* 0x32 : not user-modifiable */
	0x02, /* 0x33 : not user-modifiable */
	0x08, /* 0x34 : not user-modifiable */
	0x00, /* 0x35 : not user-modifiable */
	0x08, /* 0x36 : not user-modifiable */
	0x10, /* 0x37 : not user-modifiable */
	0x01, /* 0x38 : not user-modifiable */
	0x01, /* 0x39 : not user-modifiable */
	0x00, /* 0x3a : not user-modifiable */
	0x00, /* 0x3b : not user-modifiable */
	0x00, /* 0x3c : not user-modifiable */
	0x00, /* 0x3d : not user-modifiable */
	0xff, /* 0x3e : not user-modifiable */
	0x00, /* 0x3f : not user-modifiable */
	0x0F, /* 0x40 : not user-modifiable */
	0x00, /* 0x41 : not user-modifiable */
	0x00, /* 0x42 : not user-modifiable */
	0x00, /* 0x43 : not user-modifiable */
	0x00, /* 0x44 : not user-modifiable */
	0x00, /* 0x45 : not user-modifiable */
	0x20, /* 0x46 : interrupt configuration 0->level low detection, 1-> level high, 2-> Out of window, 3->In window, 0x20-> New sample ready , TBC */
	0x0b, /* 0x47 : not user-modifiable */
	0x00, /* 0x48 : not user-modifiable */
	0x00, /* 0x49 : not user-modifiable */
	0x02, /* 0x4a : not user-modifiable */
	0x0a, /* 0x4b : not user-modifiable */
	0x21, /* 0x4c : not user-modifiable */
	0x00, /* 0x4d : not user-modifiable */
	0x00, /* 0x4e : not user-modifiable */
	0x05, /* 0x4f : not user-modifiable */
	0x00, /* 0x50 : not user-modifiable */
	0x00, /* 0x51 : not user-modifiable */
	0x00, /* 0x52 : not user-modifiable */
	0x00, /* 0x53 : not user-modifiable */
	0xc8, /* 0x54 : not user-modifiable */
	0x00, /* 0x55 : not user-modifiable */
	0x00, /* 0x56 : not user-modifiable */
	0x38, /* 0x57 : not user-modifiable */
	0xff, /* 0x58 : not user-modifiable */
	0x01, /* 0x59 : not user-modifiable */
	0x00, /* 0x5a : not user-modifiable */
	0x08, /* 0x5b : not user-modifiable */
	0x00, /* 0x5c : not user-modifiable */
	0x00, /* 0x5d : not user-modifiable */
	0x01, /* 0x5e : not user-modifiable */
	0xdb, /* 0x5f : not user-modifiable */
	0x0f, /* 0x60 : not user-modifiable */
	0x01, /* 0x61 : not user-modifiable */
	0xf1, /* 0x62 : not user-modifiable */
	0x0d, /* 0x63 : not user-modifiable */
	0x01, /* 0x64 : Sigma threshold MSB (mm in 14.2 format for MSB+LSB), use SetSigmaThreshold(), default value 90 mm  */
	0x68, /* 0x65 : Sigma threshold LSB */
	0x00, /* 0x66 : Min count Rate MSB (MCPS in 9.7 format for MSB+LSB), use SetSignalThreshold() */
	0x80, /* 0x67 : Min count Rate LSB */
	0x08, /* 0x68 : not user-modifiable */
	0xb8, /* 0x69 : not user-modifiable */
	0x00, /* 0x6a : not user-modifiable */
	0x00, /* 0x6b : not user-modifiable */
	0x00, /* 0x6c : Intermeasurement period MSB, 32 bits register, use SetIntermeasurementInMs() */
	0x00, /* 0x6d : Intermeasurement period */
	0x0f, /* 0x6e : Intermeasurement period */
	0x89, /* 0x6f : Intermeasurement period LSB */
	0x00, /* 0x70 : not user-modifiable */
	0x00, /* 0x71 : not user-modifiable */
	0x00, /* 0x72 : distance threshold high MSB (in mm, MSB+LSB), use SetD:tanceThreshold() */
	0x00, /* 0x73 : distance threshold high LSB */
	0x00, /* 0x74 : distance threshold low MSB ( in mm, MSB+LSB), use SetD:tanceThreshold() */
	0x00, /* 0x75 : distance threshold low LSB */
	0x00, /* 0x76 : not user-modifiable */
	0x01, /* 0x77 : not user-modifiable */
	0x0f, /* 0x78 : not user-modifiable */
	0x0d, /* 0x79 : not user-modifiable */
	0x0e, /* 0x7a : not user-modifiable */
	0x0e, /* 0x7b : not user-modifiable */
	0x00, /* 0x7c : not user-modifiable */
	0x00, /* 0x7d : not user-modifiable */
	0x02, /* 0x7e : not user-modifiable */
	0xc7, /* 0x7f : ROI center, use SetROI() */
	0xff, /* 0x80 : XY ROI (X=Width, Y=Height), use SetROI() */
	0x9B, /* 0x81 : not user-modifiable */
	0x00, /* 0x82 : not user-modifiable */
	0x00, /* 0x83 : not user-modifiable */
	0x00, /* 0x84 : not user-modifiable */
	0x01, /* 0x85 : not user-modifiable */
	0x00, /* 0x86 : clear interrupt, use ClearInterrupt() */
	0x00 /* 0x87 : start ranging, use StartRanging() or StopRanging(), If you want an automatic start after VL53L1X_init() call, put 0x40 in location 0x87 */
};

/* endian swap helpers */

static inline uint32_t swap32(uint32_t v)
{
    return (((v & 0x000000FF) << 24) |
            ((v & 0x0000FF00) <<  8) |
            ((v & 0x00FF0000) >>  8) |
            ((v & 0xFF000000) >> 24));
}

static inline uint16_t swap16(uint16_t v)
{
    return ((v & 0x00FF) << 8) | ((v & 0xFF00) >> 8);
}

/* i2c helpers since we almost always same amount of data  */
static void write1_reg2(uint16_t reg, uint8_t data)
{
    write_scratch = data;
    i2c_write_reg2(I2C_ADDR, reg, (uint8_t*)&write_scratch, 1);
}

static void write2_reg2(uint16_t reg, uint16_t data)
{
    write_scratch = swap16(data);
    i2c_write_reg2(I2C_ADDR, reg, (uint8_t*)&write_scratch, 2);
}

static uint8_t read1_reg2(uint16_t reg)
{
    uint8_t buf;
    i2c_read_reg2(I2C_ADDR, reg, &buf, 1);
    return buf;
}

static uint16_t read2_reg2(uint16_t reg)
{
    uint8_t buf[2];
    i2c_read_reg2(I2C_ADDR, reg, buf, 2);

    /* big endian returned, swap to le */
    return ((buf[0] << 8) | buf[1]);
}

uint8_t vl53l1x_get_int_polarity(void)
{
    uint8_t tmp = read1_reg2(0x0030);
    tmp = tmp & 0x10;
    return !(tmp >> 4);
}

void vl53l1x_clear_int(void)
{
    write1_reg2(0x0086, 0x01);
}

void vl53l1x_start_ranging(void)
{
    write1_reg2(0x0087, 0x40);
}

void vl53l1x_stop_ranging(void)
{
    write1_reg2(0x0087, 0x00);
}

uint8_t vl53l1x_get_data_ready(void)
{
    /* 0x0031 */
    uint8_t pol = vl53l1x_get_int_polarity();   
    uint8_t tmp = read1_reg2(0x0031);
    uint8_t data_ready = (tmp & 1) == pol;
    return data_ready;
}

bool vl53l1x_init(void)
{
    uint8_t data_ready = 0;
    uint8_t timeout = 0;
    
    /* bit of a doozy to initialize the sensor with static init table */
    for (uint8_t reg_addr = 0x2d; reg_addr <= 0x87; ++reg_addr)
    {
        write1_reg2(reg_addr, default_cfgs[reg_addr - 0x2d]);
    }
    
    vl53l1x_start_ranging();

    /* this can take full intermeasurement period if its a fresh boot. default is ~103ms */
    while (!data_ready)
    {
        data_ready = vl53l1x_get_data_ready();
        /* TODO: sleep here */
        delay_timer0_1ms();
        if (timeout++ >= INIT_TIMEOUT_MS)
            return false;
    }
    
    vl53l1x_clear_int();
    vl53l1x_stop_ranging();
    
    write1_reg2(0x0008, 0x09); /* two bounds vhv */
    write1_reg2(0x000b, 0x00); /* start vhv from prev temp */
    return true;
}

uint16_t vl53l1x_get_id(void)
{
    uint16_t ret = read2_reg2(0x010f);
    return ret;
}

//VL53L1_RESULT__FINAL_CROSSTALK_CORRECTED_RANGE_MM_SD0
uint16_t vl53l1x_get_dist(void)
{
    uint16_t dist = read2_reg2(0x0096);
    return dist;
}

/* if you thought this sensor sucked to work with, wait till you see this 
 range mode and timing budget crap!
 HOPE YOU LIKE RANDOM MAGICVALS IN SEVERAL REGISTERS */

/* TODO: datasheet has some mid range mode in spec tables.. what */

/* Predefined values = 15, 20, 33, 50, 100(default), 200, 500. */

/*sets distance mode: 1 = short, 2 = long. default is long mode
 short mode 1.3m range, long up to 4 meters w/ a 200ms timing budget.
 
NOTE: nukes timing budget! */
void vl53l1x_set_dist_mode(enum eVL53L1X_DIST_MODE dm)
{
    if (dm == VL53L1X_DIST_MODE_UNKNOWN)
        return;
    
    bool sr = dm == VL53L1X_DIST_MODE_SHORT;
    write1_reg2(PHASECAL_CONFIG__TIMEOUT_MACROP, sr ? 0x14 : 0x0a);
    write1_reg2(RANGE_CONFIG__VCSEL_PERIOD_A, sr ? 0x07 : 0x0f);
    write1_reg2(RANGE_CONFIG__VCSEL_PERIOD_B, sr ? 0x05 : 0x0d);
    write1_reg2(RANGE_CONFIG__VALID_PHASE_HIGH, sr ? 0x38 : 0xb8);
    write2_reg2(SD_CONFIG__WOI_SD0, sr ? 0x0705 : 0x0f0d);
    write2_reg2(SD_CONFIG__INITIAL_PHASE_SD0, sr ? 0x0606 : 0x0e0e);
}

enum eVL53L1X_DIST_MODE vl53l1x_get_dist_mode(void)
{
    enum eVL53L1X_DIST_MODE ret = VL53L1X_DIST_MODE_UNKNOWN;
    uint8_t v = read1_reg2(PHASECAL_CONFIG__TIMEOUT_MACROP);
    /* you think it would kill them to use a register with actual values */
    if (v == 0x14)
    {
        ret = VL53L1X_DIST_MODE_SHORT;
    }
    else if (v == 0x0a)
    {
        ret = VL53L1X_DIST_MODE_LONG;
    }
    
    return ret;
}

uint16_t vl53l1x_get_timing_budget_ms(void)
{
    uint16_t v = read2_reg2(RANGE_CONFIG__TIMEOUT_MACROP_A_HI);
    switch(v)
    {
        case 0x001d:
            return 15;
        case 0x0051:
        case 0x001e:
            return 20;
        case 0x00d6:
        case 0x0060:
            return 33;
        case 0x01ae:
        case 0x00ad:
            return 50;
        case 0x02e1:
        case 0x01cc:
            return 100;
        case 0x03e1:
        case 0x02d9:
            return 200;
        case 0x0591:
        case 0x048f:
            return 500;
    }
    return 0;
}

/* TODO: enum for the magic values */
void vl53l1x_set_timing_budget_ms(uint16_t tb)
{
    /* once again for some mysterious reason, magic numbers in registers depending 
        on pre values, and even worse, depending on distance mode too.
        in an alternate universe this would be an unconditional single register write. 
        * this is currently the worst i2c register use i've seen in my career  */
    
    enum eVL53L1X_DIST_MODE dm = vl53l1x_get_dist_mode();
    if (dm == VL53L1X_DIST_MODE_UNKNOWN)
        return;
    
    bool lr = dm == VL53L1X_DIST_MODE_LONG;
    switch (tb)
    {
        case 15:
            /* 15 is only available in short distance mode .. */
            if (!lr)
            {
                write2_reg2(RANGE_CONFIG__TIMEOUT_MACROP_A_HI, 0x001d);
                write2_reg2(RANGE_CONFIG__TIMEOUT_MACROP_B_HI, 0x0027);
            }
            break;
        case 20:
            write2_reg2(RANGE_CONFIG__TIMEOUT_MACROP_A_HI, lr ? 0x001e : 0x0051);
            write2_reg2(RANGE_CONFIG__TIMEOUT_MACROP_B_HI, lr ? 0x0022 : 0x006e);
            break;
        case 33:
            write2_reg2(RANGE_CONFIG__TIMEOUT_MACROP_A_HI, lr ? 0x0060 : 0x00d6);
            write2_reg2(RANGE_CONFIG__TIMEOUT_MACROP_B_HI, lr ? 0x006e : 0x006e);
            break;
        case 50:
            write2_reg2(RANGE_CONFIG__TIMEOUT_MACROP_A_HI, lr ? 0x00ad : 0x01ae);
            write2_reg2(RANGE_CONFIG__TIMEOUT_MACROP_B_HI, lr ? 0x00c6 : 0x01e8);
            break;
        case 100:
            write2_reg2(RANGE_CONFIG__TIMEOUT_MACROP_A_HI, lr ? 0x01cc : 0x02e1);
            write2_reg2(RANGE_CONFIG__TIMEOUT_MACROP_B_HI, lr ? 0x01ea : 0x0388);
            break;
        case 200:
            write2_reg2(RANGE_CONFIG__TIMEOUT_MACROP_A_HI, lr ? 0x02d9 : 0x03e1);
            write2_reg2(RANGE_CONFIG__TIMEOUT_MACROP_B_HI, lr ? 0x02f8 : 0x0496);
            break;        
        case 500:
            write2_reg2(RANGE_CONFIG__TIMEOUT_MACROP_A_HI, lr ? 0x048f : 0x0591);
            write2_reg2(RANGE_CONFIG__TIMEOUT_MACROP_B_HI, lr ? 0x04a4 : 0x05c1);
            break;
    }
}



void vl53l1x_set_intermeasurement_ms(uint16_t ms)
{
    uint16_t pll = read2_reg2(VL53L1_RESULT__OSC_CALIBRATE_VAL);
    pll = pll & 0x3ff;

    /* need to write a 32 bit value in big endian order */
    uint32_t im_bytes = (uint32_t)(pll * ms * 1.075);
    im_bytes = swap32(im_bytes);
    i2c_write_reg2(I2C_ADDR, VL53L1_SYSTEM__INTERMEASUREMENT_PERIOD, (uint8_t*)&im_bytes, 4);
}

uint16_t vl53l1x_get_intermeasurement_ms(void)
{
    /* to read intermeasurement, we need to do similar to set, read 32bit, flip around ,
     * and PLL math */
    uint32_t ims;
    i2c_read_reg2(I2C_ADDR, VL53L1_SYSTEM__INTERMEASUREMENT_PERIOD, (uint8_t*)&ims, 4);

    /* big endian -> msp30 native little endian */
    ims = swap32(ims);

    uint16_t pll = read2_reg2(VL53L1_RESULT__OSC_CALIBRATE_VAL);
    pll = pll & 0x3ff;
    /* take note the conversion is slightly different vs set */
    ims = (uint16_t)(ims / (pll * 1.065));
    return (uint16_t)ims;
}
