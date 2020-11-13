/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
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
#include "stm32f0xx_ll_iwdg.h"
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

#if defined(USE_FULL_ASSERT)
#include "stm32_assert.h"
#endif /* USE_FULL_ASSERT */

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "oled.h"

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */
extern uint32_t SystemTimer;
/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define OLED_RST_Pin LL_GPIO_PIN_13
#define OLED_RST_GPIO_Port GPIOC
#define DIP_CAL_Pin LL_GPIO_PIN_0
#define DIP_CAL_GPIO_Port GPIOA
#define DIP_CLOSE_Pin LL_GPIO_PIN_1
#define DIP_CLOSE_GPIO_Port GPIOA
#define DIP_SET2_Pin LL_GPIO_PIN_2
#define DIP_SET2_GPIO_Port GPIOA
#define DIP_SET1_Pin LL_GPIO_PIN_3
#define DIP_SET1_GPIO_Port GPIOA
#define ENC_SS_Pin LL_GPIO_PIN_4
#define ENC_SS_GPIO_Port GPIOA
#define ENC_SCK_Pin LL_GPIO_PIN_5
#define ENC_SCK_GPIO_Port GPIOA
#define ENC_MISO_Pin LL_GPIO_PIN_6
#define ENC_MISO_GPIO_Port GPIOA
#define ENC_MOSI_Pin LL_GPIO_PIN_7
#define ENC_MOSI_GPIO_Port GPIOA
#define CLK_IN_Pin LL_GPIO_PIN_0
#define CLK_IN_GPIO_Port GPIOB
#define DIR_IN_Pin LL_GPIO_PIN_1
#define DIR_IN_GPIO_Port GPIOB
#define EN_IN_Pin LL_GPIO_PIN_2
#define EN_IN_GPIO_Port GPIOB
#define LED_Pin LL_GPIO_PIN_11
#define LED_GPIO_Port GPIOB
#define OLED_CS_Pin LL_GPIO_PIN_12
#define OLED_CS_GPIO_Port GPIOB
#define OLED_DC_Pin LL_GPIO_PIN_13
#define OLED_DC_GPIO_Port GPIOB
#define OLED_SDIN_Pin LL_GPIO_PIN_14
#define OLED_SDIN_GPIO_Port GPIOB
#define OLED_SCLK_Pin LL_GPIO_PIN_15
#define OLED_SCLK_GPIO_Port GPIOB
#define KEY_CONFIRM_Pin LL_GPIO_PIN_8
#define KEY_CONFIRM_GPIO_Port GPIOA
#define UART_TX_Pin LL_GPIO_PIN_9
#define UART_TX_GPIO_Port GPIOA
#define UART_RX_Pin LL_GPIO_PIN_10
#define UART_RX_GPIO_Port GPIOA
#define KEY_BACK_Pin LL_GPIO_PIN_11
#define KEY_BACK_GPIO_Port GPIOA
#define STEP_IN_Pin LL_GPIO_PIN_12
#define STEP_IN_GPIO_Port GPIOA
#define KEY_SELECT_Pin LL_GPIO_PIN_15
#define KEY_SELECT_GPIO_Port GPIOA
#define PWM1_Pin LL_GPIO_PIN_4
#define PWM1_GPIO_Port GPIOB
#define PWM2_Pin LL_GPIO_PIN_5
#define PWM2_GPIO_Port GPIOB
#define BRIDGE1_Pin LL_GPIO_PIN_6
#define BRIDGE1_GPIO_Port GPIOB
#define BRIDGE2_Pin LL_GPIO_PIN_7
#define BRIDGE2_GPIO_Port GPIOB
#define BRIDGE3_Pin LL_GPIO_PIN_8
#define BRIDGE3_GPIO_Port GPIOB
#define BRIDGE4_Pin LL_GPIO_PIN_9
#define BRIDGE4_GPIO_Port GPIOB
#ifndef NVIC_PRIORITYGROUP_0
#define NVIC_PRIORITYGROUP_0         ((uint32_t)0x00000007) /*!< 0 bit  for pre-emption priority,
                                                                 4 bits for subpriority */
#define NVIC_PRIORITYGROUP_1         ((uint32_t)0x00000006) /*!< 1 bit  for pre-emption priority,
                                                                 3 bits for subpriority */
#define NVIC_PRIORITYGROUP_2         ((uint32_t)0x00000005) /*!< 2 bits for pre-emption priority,
                                                                 2 bits for subpriority */
#define NVIC_PRIORITYGROUP_3         ((uint32_t)0x00000004) /*!< 3 bits for pre-emption priority,
                                                                 1 bit  for subpriority */
#define NVIC_PRIORITYGROUP_4         ((uint32_t)0x00000003) /*!< 4 bits for pre-emption priority,
                                                                 0 bit  for subpriority */
#endif
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
