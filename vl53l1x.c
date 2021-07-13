/*
 * File:   vl53l1x.c
 * Author: Dexter
 *
 * Created on July 13, 2021, 4:12 AM
 */

#include "vl53l1x.h"
#include "mcc_generated_files/i2c1_master.h"
#include "mcc_generated_files/examples/i2c1_master_example.h"

#define I2C_ADDR (0x29)

#define INIT_TIMEOUT_MS (150)


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

static i2c1_operations_t data16_read(void *ptr)
{
    I2C1_SetBuffer(ptr, 2);
    I2C1_SetDataCompleteCallback(NULL,NULL);
    return I2C1_RESTART_READ;
}

static i2c1_operations_t data8_read(void *ptr)
{
    I2C1_SetBuffer(ptr, 1);
    I2C1_SetDataCompleteCallback(NULL,NULL);
    return I2C1_RESTART_READ;
}

static uint8_t read1_reg2(uint16_t reg)
{
    /* have to swap endian for over the wire */
    uint16_t reg_swapped = (reg>>8) | (reg<<8);
    
    uint8_t ret = 0x00; 

    while(!I2C1_Open(I2C_ADDR)); 
    I2C1_SetDataCompleteCallback(data8_read, &ret);
    I2C1_SetBuffer(&reg_swapped, 2);
    I2C1_SetAddressNackCallback(NULL,NULL); 
    I2C1_MasterWrite();
    while(I2C1_BUSY == I2C1_Close()); 
    return ret;
}

static uint16_t read2_reg2(uint16_t reg)
{
    /* have to swap endian for over the wire */
    uint16_t reg_swapped = (reg>>8) | (reg<<8);
    
    uint16_t ret = 0x00; 

    while(!I2C1_Open(I2C_ADDR)); 
    I2C1_SetDataCompleteCallback(data16_read, &ret);
    I2C1_SetBuffer(&reg_swapped, 2);
    I2C1_SetAddressNackCallback(NULL,NULL); 
    I2C1_MasterWrite();
    while(I2C1_BUSY == I2C1_Close()); 
    return (ret << 8 | ret >> 8);
}

static i2c1_operations_t data8_write(void *ptr)
{
    I2C1_SetBuffer(ptr, 1);
    I2C1_SetDataCompleteCallback(NULL,NULL);
    return I2C1_CONTINUE;
}

static i2c1_operations_t data16_write(void *ptr)
{
    I2C1_SetBuffer(ptr, 2);
    I2C1_SetDataCompleteCallback(NULL,NULL);
    return I2C1_CONTINUE;
}

static void write2_reg2(uint16_t reg, uint16_t val)
{
    /* have to swap endian for over the wire */
    uint16_t reg_swapped = (reg>>8) | (reg<<8);
    
    while(!I2C1_Open(I2C_ADDR)); 
    I2C1_SetDataCompleteCallback(data16_write, &val);
    I2C1_SetBuffer(&reg_swapped, 2);
    I2C1_SetAddressNackCallback(NULL,NULL); 
    I2C1_MasterWrite();
    while(I2C1_BUSY == I2C1_Close()); 
}

static void write1_reg2(uint16_t reg, uint8_t val)
{
    /* have to swap endian for over the wire */
    uint16_t reg_swapped = (reg>>8) | (reg<<8);
    
    while(!I2C1_Open(I2C_ADDR)); 
    I2C1_SetDataCompleteCallback(data8_write, &val);
    I2C1_SetBuffer(&reg_swapped, 2);
    I2C1_SetAddressNackCallback(NULL,NULL); 
    I2C1_MasterWrite();
    while(I2C1_BUSY == I2C1_Close()); 
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
    
    /* bit of a doozy to initialize the sensor */
    for (uint8_t reg_addr = 0x2d; reg_addr <= 0x87; ++reg_addr)
    {
        //I2C1_Write1ByteRegister(I2C_ADDR, reg_addr, default_cfgs[reg_addr - 0x2d]);
        write1_reg2(reg_addr, default_cfgs[reg_addr - 0x2d]);
    }
    
    vl53l1x_start_ranging();
    
    while (!data_ready)
    {
        data_ready = vl53l1x_get_data_ready();
        __delay_ms(1);
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