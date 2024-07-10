
#include "app_rgbled.h"

//ws2812b
//#define CODE_0		         0xC0
//#define CODE_1		         0xF8

//led rgb
//#define CODE_0		         0xC0
//#define CODE_1		         0xF0

#define CODE_0		            0x3f
#define CODE_1		            0x0f

//test
//#define CODE_0		         0xC0
//#define CODE_1		         0xFC

#define RGBLED_COUNT		   24  
#define RGBLED_BYTE_COUNT	(RGBLED_COUNT*3)

uint8_t ispidata[RGBLED_BYTE_COUNT * 8] = {0};	

//uint8_t ileddata[RGBLED_BYTE_COUNT] = 
uint8_t ileddata[24*3] = 
{
   0XFF, 0X00, 0X00,	 
   0X00, 0XFF, 0X00,	 
   0X00, 0X00, 0XFF,	 
   0X00, 0XFF, 0XFF,	 
   0XFF, 0X00, 0XFF,	 
   0XFF, 0XFF, 0X00,	 
   0XFF, 0XFF, 0XFF,	 
   0X00, 0X00, 0X00,	 
  
   0XFF, 0X00, 0X00,	 
   0X00, 0XFF, 0X00,	 
   0X00, 0X00, 0XFF,	 
   0X00, 0XFF, 0XFF,	 
   0XFF, 0X00, 0XFF,	 
   0XFF, 0XFF, 0X00,	 
   0XFF, 0XFF, 0XFF,	 
   0X00, 0X00, 0X00,	 
  
   0XFF, 0X00, 0X00,	 
   0X00, 0XFF, 0X00,	 
   0X00, 0X00, 0XFF,	 
   0X00, 0XFF, 0XFF,	 
   0XFF, 0X00, 0XFF,	 
   0XFF, 0XFF, 0X00,	 
   0XFF, 0XFF, 0XFF,	 
   0X00, 0X00, 0X00 
  
};


void led2spi_onedata(uint8_t idatain,  uint8_t *sout)
{
   for(uint8_t i = 0; i <8; i++) 
   {
      if(idatain & 0x80)       
         *sout = CODE_1;          
      else           
         *sout = CODE_0;
       
      idatain <<= 1;
 
      sout++; 
   }
}

void led2spi_data(uint8_t *sledin,  uint8_t *sout, uint16_t icount)
{
   
	for(uint8_t i = 0; i < icount; i++)
	{      
      led2spi_onedata(ileddata[i], &sout[i*8]);
	}
   
}
 
void switch_color(void)
{
   static uint8_t icolorcount=0;  
   
   //switch color
   if(icolorcount==0)
      set_color_red();
   else if (icolorcount==1)            
      set_color_green();
   else if (icolorcount==2)  
      set_color_blue();
   else if (icolorcount==3) 
      set_color_white();
   else if (icolorcount==4) 
      set_color_black();       
   
   icolorcount++;         
   if(icolorcount >4)
      icolorcount=0;   
   
}

void set_color_rgb(uint8_t ired,  uint8_t igreen, uint8_t iblue)
{
   uint8_t i;   
   for (i = 0; i < RGBLED_COUNT; i++)
   {
      ileddata[i*3] = igreen;
      ileddata[i*3+1] = ired;
      ileddata[i*3+2] = iblue;
   } 
}

void set_color_red(void)
{
   uint8_t i;   
   for (i = 0; i < RGBLED_COUNT; i++)
   {
      ileddata[i*3] = 0;
      ileddata[i*3+1] = 0xff;
      ileddata[i*3+2] = 0;
   } 
}

void set_color_green(void)
{
   uint8_t i;   
   for (i = 0; i < RGBLED_COUNT; i++)
   {
      ileddata[i*3] = 0xff;
      ileddata[i*3+1] = 0;
      ileddata[i*3+2] = 0;
   } 
}
void set_color_blue(void)
{
   uint8_t i;   
   for (i = 0; i < RGBLED_COUNT; i++)
   {
      ileddata[i*3] = 0;
      ileddata[i*3+1] = 0;
      ileddata[i*3+2] = 0xff;
   } 
}

void set_color_white(void)
{
   uint8_t i;   
   for (i = 0; i < RGBLED_COUNT; i++)
   {
      ileddata[i*3] = 0xff;
      ileddata[i*3+1] = 0xff;
      ileddata[i*3+2] = 0xff;
   } 
}

void set_color_black(void)
{
   uint8_t i;   
   for (i = 0; i < RGBLED_COUNT; i++)
   {
      ileddata[i*3] = 0;
      ileddata[i*3+1] = 0;
      ileddata[i*3+2] = 0;
   } 
}


void rgbled_task(void)
{ 
   LL_SPI_Enable(SPI1);  

   led2spi_data(ileddata, ispidata, RGBLED_BYTE_COUNT);

   //reset data
   //for(uint16_t i=0; i<65; i++)
   //{
   //   LL_SPI_TransmitData8(SPI1, 0xff);
   //   while(!LL_SPI_IsActiveFlag_TXE(SPI1));   
   //}   
   LL_SPI_TransmitData8(SPI1, 0xff);
   while(!LL_SPI_IsActiveFlag_TXE(SPI1));    
   LL_mDelay(1);	
   
   //rgb data
   spi_transmitbytes(ispidata, RGBLED_BYTE_COUNT*8 );
   
   //reset data
   LL_SPI_TransmitData8(SPI1, 0xff);
   while(!LL_SPI_IsActiveFlag_TXE(SPI1));     
   LL_mDelay(1);	
}
 
 

