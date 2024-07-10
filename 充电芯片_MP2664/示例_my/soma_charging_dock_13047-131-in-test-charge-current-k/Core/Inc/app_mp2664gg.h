 
#ifndef __APP_MP2664GG_H__
#define __APP_MP2664GG_H__

#include "main.h"


#define ICADDR_MP2664GG  (0x09 << 1) 

   
#define MP2664GG_INPUTSOURCE_CONTROL_REG        0x00  
#define MP2664GG_POWERON_CONFIG_REG             0x01  
#define MP2664GG_CHARGECURRENT_CONTROL_REG      0x02 
#define MP2664GG_PRECHARGECURRENT_REG           0x03  
#define MP2664GG_CHARGEVOLTAGE_CONTROL_REG      0x04  
#define MP2664GG_CHARGETIME_CONTROL_REG         0x05  
#define MP2664GG_MISCOPERATION_CONTROL_REG       0x06  
#define MP2664GG_SYSTEMSTATUS_REG               0x07
#define MP2664GG_FAULT_REG                      0x08

#define BATT_CHANNEL0                      0x00
#define BATT_CHANNEL1                      0x01
#define BATT_CHANNEL2                      0x02
#define BATT_CHANNEL3                      0x03
#define BATT_CHANNEL4                      0x04
#define BATT_CHANNEL5                      0x05


void selbattchannel(uint8_t isel);

//00H
void set_hiz(uint8_t isel, uint8_t ienable);
void set_input_min_voltage(uint8_t isel, uint8_t ival);
void set_input_current_limit(uint8_t isel, uint8_t ival);

//01H
void set_register_reset(uint8_t isel, uint8_t ival);
void opencharge(uint8_t isel);
void closecharge(uint8_t isel);
void set_batt_UVLO(uint8_t isel, uint8_t ilevel);

//02H
void set_fastcharge_current(uint8_t isel , uint8_t ilevel);

//03H
void set_precharge_current(uint8_t isel, uint8_t ilevel);

//04H
void set_batt_regulation_voltage(uint8_t isel, uint8_t ilevel);
void set_fastcharge_threshold(uint8_t isel, uint8_t ilevel);
void set_recharge_threshold(uint8_t isel, uint8_t ilevel);

//05H
void set_terminate(uint8_t isel, uint8_t ienable);
void set_safety_timer(uint8_t isel, uint8_t ienable);
void set_fastcharge_timer(uint8_t isel, uint8_t ilevel);
void set_terminate_timer(uint8_t isel, uint8_t ienable);

//06H
void set_ntc(uint8_t isel, uint8_t ienable);
void set_thermal_regulation_threshold(uint8_t isel, uint8_t ilevel);

//07H
uint8_t get_system_status(uint8_t isel);

//08H
uint8_t get_fault(uint8_t isel);

#endif /* __APP_MP2664GG_H__ */

