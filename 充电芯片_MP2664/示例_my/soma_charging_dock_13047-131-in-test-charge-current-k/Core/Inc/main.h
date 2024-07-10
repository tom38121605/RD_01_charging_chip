/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx_hal.h"

#include "stm32f0xx_ll_crs.h"
#include "stm32f0xx_ll_rcc.h"
#include "stm32f0xx_ll_bus.h"
#include "stm32f0xx_ll_system.h"
#include "stm32f0xx_ll_exti.h"
#include "stm32f0xx_ll_cortex.h"
#include "stm32f0xx_ll_utils.h"
#include "stm32f0xx_ll_pwr.h"
#include "stm32f0xx_ll_dma.h"
#include "stm32f0xx_ll_spi.h"
#include "stm32f0xx_ll_tim.h"
#include "stm32f0xx_ll_usart.h"
#include "stm32f0xx_ll_gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#define switch2read(x)  x->CR1 &=~(1<<3);  x->CR1 |=  (1<<2);   //TE=0, RE=1
#define switch2send(x)  x->CR1 &=~(1<<2);  x->CR1 |=  (1<<3);   //TE=1, RE=0

#define WAIT2TE_MASTER 300  
#define WAIT2RE0_MASTER 320  
#define WAIT2RE_MASTER 320   
 
#define WAIT2TE_SLAVE   10
#define WAIT2RE_SLAVE   320 

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */
void delay_nop(uint32_t nCount);
void Send_Data_To_UART1 (uint8_t c);
void iputs1(char *msg);
void iputbytes1(uint8_t *msg, uint32_t ilen);
 
void uartsendcmd(uint8_t *scmd, uint32_t ilen);   
void uartsendstr(uint8_t *str1);


void Send_Data_To_UARTx(USART_TypeDef *USARTx, uint8_t c);
void iputsx(USART_TypeDef *USARTx , char *msg);
void iputbytesx(USART_TypeDef *USARTx, uint8_t *msg, uint32_t ilen);
void uartsendcmdx(USART_TypeDef *USARTx, uint8_t *scmd, uint32_t ilen);
void uartsendstrx(USART_TypeDef *USARTx, uint8_t *str1);

   
void spi_transmitbytes(uint8_t *sdata, uint16_t inum);

void pwmled_task(void);

uint8_t i2c_writereg(uint16_t idevaddr, uint8_t ireg,uint8_t data);
uint8_t i2c_readreg(uint16_t idevaddr,uint8_t ireg);
uint8_t i2c_writebytes(uint16_t idevaddr, uint8_t ireg, uint8_t *sbuf,uint16_t ilen);
uint8_t i2c_readbytes(uint16_t idevaddr,uint8_t ireg,uint8_t *sbuf,uint16_t ilen);

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define DATA_EN1_Pin LL_GPIO_PIN_13
#define DATA_EN1_GPIO_Port GPIOC
#define DATA_EN2_Pin LL_GPIO_PIN_14
#define DATA_EN2_GPIO_Port GPIOC
#define DATA_EN3_Pin LL_GPIO_PIN_15
#define DATA_EN3_GPIO_Port GPIOC
#define I2C1_SDA_Pin LL_GPIO_PIN_0
#define I2C1_SDA_GPIO_Port GPIOF
#define I2C1_SCL_Pin LL_GPIO_PIN_1
#define I2C1_SCL_GPIO_Port GPIOF
#define DATA4_Pin LL_GPIO_PIN_0
#define DATA4_GPIO_Port GPIOA
#define DATA2_Pin LL_GPIO_PIN_2
#define DATA2_GPIO_Port GPIOA
#define DATA6_Pin LL_GPIO_PIN_4
#define DATA6_GPIO_Port GPIOA
#define SPI1_CLK_Pin LL_GPIO_PIN_5
#define SPI1_CLK_GPIO_Port GPIOA
#define LED_B_Pin LL_GPIO_PIN_6
#define LED_B_GPIO_Port GPIOA
#define SPI1_MO_Pin LL_GPIO_PIN_7
#define SPI1_MO_GPIO_Port GPIOA
#define SEL0_Pin LL_GPIO_PIN_0
#define SEL0_GPIO_Port GPIOB
#define SEL1_Pin LL_GPIO_PIN_1
#define SEL1_GPIO_Port GPIOB
#define SEL2_Pin LL_GPIO_PIN_2
#define SEL2_GPIO_Port GPIOB
#define DATA3_Pin LL_GPIO_PIN_10
#define DATA3_GPIO_Port GPIOB
#define DATA_EN4_Pin LL_GPIO_PIN_11
#define DATA_EN4_GPIO_Port GPIOB
#define DATA_EN5_Pin LL_GPIO_PIN_12
#define DATA_EN5_GPIO_Port GPIOB
#define DATA_EN6_Pin LL_GPIO_PIN_13
#define DATA_EN6_GPIO_Port GPIOB
#define LED_G_Pin LL_GPIO_PIN_14
#define LED_G_GPIO_Port GPIOB
#define DATA1_Pin LL_GPIO_PIN_9
#define DATA1_GPIO_Port GPIOA
#define LED_R_Pin LL_GPIO_PIN_11
#define LED_R_GPIO_Port GPIOA
#define DATA5_Pin LL_GPIO_PIN_3
#define DATA5_GPIO_Port GPIOB
#define CHARGE_INT1_Pin LL_GPIO_PIN_4
#define CHARGE_INT1_GPIO_Port GPIOB
#define CHARGE_INT2_Pin LL_GPIO_PIN_5
#define CHARGE_INT2_GPIO_Port GPIOB
#define CHARGE_INT3_Pin LL_GPIO_PIN_6
#define CHARGE_INT3_GPIO_Port GPIOB
#define CHARGE_INT4_Pin LL_GPIO_PIN_7
#define CHARGE_INT4_GPIO_Port GPIOB
#define CHARGE_INT5_Pin LL_GPIO_PIN_8
#define CHARGE_INT5_GPIO_Port GPIOB
#define CHARGE_INT6_Pin LL_GPIO_PIN_9
#define CHARGE_INT6_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
