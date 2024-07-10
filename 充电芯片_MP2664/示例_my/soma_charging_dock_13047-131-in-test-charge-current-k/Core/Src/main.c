/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "i2c.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "app_rgbled.h"
#include "app_mp2664gg.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
 
 

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */



void delay_nop(uint32_t nCount) 
{ 
   for(; nCount != 0; nCount--)  
   { 
      __NOP(); 
   } 
}

void Send_Data_To_UART1 (uint8_t c)
{
   USART1->TDR = c;
   while( (USART1-> ISR & (1<<7) ) ==0 );  //USART_ISR_TC  //USART_ISR_TXE_TXFNF
}

void iputs1(char *msg)
{
   while(*msg)
   Send_Data_To_UART1(*msg++);   
}


void iputbytes1(uint8_t *msg, uint32_t ilen)
{
   while(ilen--)
   Send_Data_To_UART1(*msg++);
}

void uartsendcmd(uint8_t *scmd, uint32_t ilen)
{
   //switch to tx 
   delay_nop(WAIT2TE_SLAVE); //delay_nop(WAIT2TE_MASTER);  
   switch2send(USART1); 

   //iputbytes1((uint8_t *)scmd,ilen);     
   iputbytes1(scmd,ilen);     

   //switch to rx
   delay_nop(WAIT2RE_SLAVE);  //delay_nop(WAIT2RE0_MASTER);    
   switch2read(USART1);   
}   

void uartsendstr(uint8_t *str1)
{
   //switch to tx 
   delay_nop(WAIT2TE_SLAVE); //delay_nop(WAIT2TE_MASTER);  
   switch2send(USART1); 

   iputs1((char*)str1);   

   //switch to rx
   delay_nop(WAIT2RE_SLAVE);  //delay_nop(WAIT2RE0_MASTER);    
   switch2read(USART1);   
}   


//-----------------uartx-------------------------------------

void Send_Data_To_UARTx(USART_TypeDef *USARTx, uint8_t c)
{
   USARTx->TDR = c;
   while( (USARTx-> ISR & (1<<7) ) ==0 );  //USART_ISR_TC  //USART_ISR_TXE_TXFNF
}

void iputsx(USART_TypeDef *USARTx , char *msg)
{
   while(*msg)
   Send_Data_To_UARTx(USARTx ,*msg++);   
}

void iputbytesx(USART_TypeDef *USARTx, uint8_t *msg, uint32_t ilen)
{
   while(ilen--)
   Send_Data_To_UARTx(USARTx ,*msg++);
}

void uartsendcmdx(USART_TypeDef *USARTx, uint8_t *scmd, uint32_t ilen)
{
   //switch to tx 
   delay_nop(WAIT2TE_SLAVE);    
   switch2send(USARTx); 
 
   iputbytesx(USARTx, scmd,ilen);     

   //switch to rx
   delay_nop(WAIT2RE_SLAVE);      
   switch2read(USARTx);   
}   

void uartsendstrx(USART_TypeDef *USARTx, uint8_t *str1)
{
   //switch to tx 
   delay_nop(WAIT2TE_SLAVE);   
   switch2send(USARTx); 

   iputsx(USARTx, (char*)str1);   

   //switch to rx
   delay_nop(WAIT2RE_SLAVE);   
   switch2read(USARTx);   
}   

//-----------------uartx----------end---------------------------


void spi_transmitbytes(uint8_t *sdata, uint16_t inum)
{
   for(uint16_t i =0; i< inum; i++)
   {
      LL_SPI_TransmitData8(SPI1, sdata[i]);
      while(!LL_SPI_IsActiveFlag_TXE(SPI1));
   }
}

void pwmled_task(void)
{
     static uint32_t iduty=0;

     iduty++;
     if(iduty>10)
        iduty=0;
 
     LL_TIM_OC_SetCompareCH1(TIM3, iduty);     
     LL_TIM_OC_SetCompareCH4(TIM1, iduty);     
     LL_TIM_OC_SetCompareCH1(TIM15, iduty);     
        
}
  

uint8_t i2c_writereg(uint16_t idevaddr, uint8_t ireg,uint8_t data)
{
   if((HAL_I2C_Mem_Write(&hi2c1,idevaddr,ireg,I2C_MEMADD_SIZE_8BIT,&data,1,1000)) == HAL_OK)
   {
      return 0;
   }
   else
   {
      return 1;
   }
}

uint8_t i2c_readreg(uint16_t idevaddr,uint8_t ireg)
{
   uint8_t res;
   HAL_I2C_Mem_Read(&hi2c1,idevaddr,ireg,I2C_MEMADD_SIZE_8BIT,&res,1,1000);
   return res;
}

uint8_t i2c_writebytes(uint16_t idevaddr, uint8_t ireg, uint8_t *sbuf,uint16_t ilen)
{
   if((HAL_I2C_Mem_Write(&hi2c1,idevaddr,ireg,I2C_MEMADD_SIZE_8BIT,sbuf,ilen,1000)) == HAL_OK)
   {
      return 0;
   }
   else
   {
      return 1;
   }
}

uint8_t i2c_readbytes(uint16_t idevaddr,uint8_t ireg,uint8_t *sbuf,uint16_t ilen)
{
   if((HAL_I2C_Mem_Read(&hi2c1,idevaddr,ireg,I2C_MEMADD_SIZE_8BIT,sbuf,ilen,1000)) == HAL_OK)
   {
      return 0;
   }
   else
   {
      return 1;
   }
}

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART1_UART_Init();
  MX_SPI1_Init();
  MX_TIM3_Init();
  MX_TIM1_Init();
  MX_TIM15_Init();
  MX_I2C1_Init();
  MX_USART2_UART_Init();
  MX_USART3_UART_Init();
  MX_USART4_UART_Init();
  MX_USART5_UART_Init();
  MX_USART6_UART_Init();
  /* USER CODE BEGIN 2 */

   LL_GPIO_ResetOutputPin( DATA_EN1_GPIO_Port, DATA_EN1_Pin);
   LL_GPIO_ResetOutputPin( DATA_EN2_GPIO_Port, DATA_EN2_Pin);
   LL_GPIO_ResetOutputPin( DATA_EN3_GPIO_Port, DATA_EN3_Pin);
   LL_GPIO_ResetOutputPin( DATA_EN4_GPIO_Port, DATA_EN4_Pin);
   LL_GPIO_ResetOutputPin( DATA_EN5_GPIO_Port, DATA_EN5_Pin);
   LL_GPIO_ResetOutputPin( DATA_EN6_GPIO_Port, DATA_EN6_Pin);
   
   uartsendstrx(USART1,(uint8_t*)"start1...");   
   uartsendstrx(USART2,(uint8_t*)"start2...");
   uartsendstrx(USART3,(uint8_t*)"start3...");
   uartsendstrx(USART4,(uint8_t*)"start4...");
   uartsendstrx(USART5,(uint8_t*)"start5...");
   uartsendstrx(USART6,(uint8_t*)"start6...");
   
   LL_SPI_Enable(SPI1);   
   
   //TIM3 CH1 PA6
   LL_TIM_CC_EnableChannel(TIM3, LL_TIM_CHANNEL_CH1);
   LL_TIM_EnableCounter(TIM3);
     
   //TIM1 CH4 PA11
   LL_TIM_CC_EnableChannel(TIM1, LL_TIM_CHANNEL_CH4);
   LL_TIM_EnableCounter(TIM1);
   
   //TIM15 CH1 PB14
   LL_TIM_CC_EnableChannel(TIM15, LL_TIM_CHANNEL_CH1);
   LL_TIM_EnableCounter(TIM15);
   
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  
  //SEL2-0: 000 , sel I2C_SCL_1
   LL_GPIO_ResetOutputPin( SEL0_GPIO_Port, SEL0_Pin);    
   LL_GPIO_ResetOutputPin( SEL1_GPIO_Port, SEL1_Pin);    
   LL_GPIO_ResetOutputPin( SEL2_GPIO_Port, SEL2_Pin);   
   

   while (1)
   {  
      static uint32_t imaincount=0;
      uint8_t itemp=0;  
      
      imaincount++;      

      if(imaincount>2500000)
      {
         imaincount=0;     

         
         //--------test charge current----------------------------------         
 
         opencharge(BATT_CHANNEL0);
         set_fastcharge_current(BATT_CHANNEL0, 0x10);
         set_batt_regulation_voltage(BATT_CHANNEL0, 0x28<<2 );
         
         itemp=i2c_readreg( ICADDR_MP2664GG,MP2664GG_POWERON_CONFIG_REG);
         uartsendcmd( (uint8_t*)&itemp,1);
         itemp=i2c_readreg( ICADDR_MP2664GG,MP2664GG_CHARGECURRENT_CONTROL_REG);
         uartsendcmd( (uint8_t*)&itemp,1);
         itemp=i2c_readreg( ICADDR_MP2664GG,MP2664GG_CHARGEVOLTAGE_CONTROL_REG);
         uartsendcmd( (uint8_t*)&itemp,1);
         
         //read the status
         //closecharge(BATT_CHANNEL0);
         itemp=get_system_status(BATT_CHANNEL0); 
         uartsendcmd( (uint8_t*)&itemp,1);         
         itemp=get_fault(BATT_CHANNEL0);
         uartsendcmd( (uint8_t*)&itemp,1);  
         
         //--------test charge current-----end-----------------------------         
         
         ////uart
         //LL_GPIO_ResetOutputPin( DATA_EN1_GPIO_Port, DATA_EN1_Pin);      
         //uartsendstrx(USART1,(uint8_t*)"123");

         //LL_GPIO_ResetOutputPin( DATA_EN2_GPIO_Port, DATA_EN2_Pin);
         //LL_GPIO_ResetOutputPin( DATA_EN3_GPIO_Port, DATA_EN3_Pin);
         //LL_GPIO_ResetOutputPin( DATA_EN4_GPIO_Port, DATA_EN4_Pin);
         //LL_GPIO_ResetOutputPin( DATA_EN5_GPIO_Port, DATA_EN5_Pin);
         //LL_GPIO_ResetOutputPin( DATA_EN6_GPIO_Port, DATA_EN6_Pin);         
         //uartsendstrx(USART2,(uint8_t*)"22222bccdef");
         //uartsendstrx(USART3,(uint8_t*)"33333bccdef");
         //uartsendstrx(USART4,(uint8_t*)"44444bccdef");
         //uartsendstrx(USART5,(uint8_t*)"5555bccdef");
         //uartsendstrx(USART6,(uint8_t*)"6666bccdef");         
         
         
         //rgbled                    
         set_color_rgb(8, 15, 5);  
         rgbled_task();	
         
         //pwm led
         pwmled_task();                  
      }      
      
      //LL_mDelay(1000);	
      
      
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
   }

  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  LL_FLASH_SetLatency(LL_FLASH_LATENCY_1);
  while(LL_FLASH_GetLatency() != LL_FLASH_LATENCY_1)
  {
  }
  LL_RCC_HSI_Enable();

   /* Wait till HSI is ready */
  while(LL_RCC_HSI_IsReady() != 1)
  {

  }
  LL_RCC_HSI_SetCalibTrimming(16);
  LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_HSI, LL_RCC_PLL_MUL_6, LL_RCC_PREDIV_DIV_1);
  LL_RCC_PLL_Enable();

   /* Wait till PLL is ready */
  while(LL_RCC_PLL_IsReady() != 1)
  {

  }
  LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
  LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_1);
  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL);

   /* Wait till System clock is ready */
  while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL)
  {

  }
  LL_SetSystemCoreClock(48000000);

   /* Update the time base */
  if (HAL_InitTick (TICK_INT_PRIORITY) != HAL_OK)
  {
    Error_Handler();
  }
  LL_RCC_SetUSARTClockSource(LL_RCC_USART1_CLKSOURCE_PCLK1);
  LL_RCC_SetI2CClockSource(LL_RCC_I2C1_CLKSOURCE_HSI);
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
