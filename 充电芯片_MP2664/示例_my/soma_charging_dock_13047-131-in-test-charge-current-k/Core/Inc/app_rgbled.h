 
#ifndef __APP_RGBLED_H__
#define __APP_RGBLED_H__

#include "main.h"


void led2spi_onedata(uint8_t idatain,  uint8_t *sout);
void led2spi_data(uint8_t *sledin,  uint8_t *sout, uint16_t icount);
void switch_color(void);

void set_color_rgb(uint8_t ired,  uint8_t igreen, uint8_t iblue);
void set_color_red(void);
void set_color_green(void);
void set_color_blue(void);
void set_color_white(void);
void set_color_black(void);  

void rgbled_task(void);

#endif /* __APP_RGBLED_H__ */

