/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stdint.h"
#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include "UARTRX.h"
#include "uart.h"
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
TIM_HandleTypeDef htim1;

UART_HandleTypeDef huart1;
DMA_HandleTypeDef hdma_usart1_rx;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_TIM1_Init(void);
static void MX_USART1_UART_Init(void);
/* USER CODE BEGIN PFP */
void procesa_rx(uint8_t *datos, uint16_t n);
void menu_leds(void);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

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
  MX_DMA_Init();
  MX_TIM1_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */
  HAL_TIM_Base_Start_IT(&htim1);
  uartRX_it_idle_dma_init( &UARTRX1);
  menu_leds();

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    if (UARTRX1.flag_rx==1)
	  {
	      procesa_rx(UARTRX1.trama_rx, UARTRX1.num_datos); //procesa los comandos de LEDs
		  uartRX_DMA_Re_init(&UARTRX1);
	  }
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
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief TIM1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM1_Init(void)
{

  /* USER CODE BEGIN TIM1_Init 0 */

  /* USER CODE END TIM1_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM1_Init 1 */

  /* USER CODE END TIM1_Init 1 */
  htim1.Instance = TIM1;
  htim1.Init.Prescaler = 16000-1;
  htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim1.Init.Period = 100-1;
  htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim1.Init.RepetitionCounter = 0;
  htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
  if (HAL_TIM_Base_Init(&htim1) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim1, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM1_Init 2 */

  /* USER CODE END TIM1_Init 2 */

}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 9600;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

/**
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA2_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA2_Stream2_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA2_Stream2_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA2_Stream2_IRQn);

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  /* USER CODE BEGIN MX_GPIO_Init_1 */

  /* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, LED3_Pin|LED_Pin|LED2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LED4_GPIO_Port, LED4_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : LED3_Pin LED_Pin LED2_Pin */
  GPIO_InitStruct.Pin = LED3_Pin|LED_Pin|LED2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : LED4_Pin */
  GPIO_InitStruct.Pin = LED4_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LED4_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : IN_Pin */
  GPIO_InitStruct.Pin = IN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(IN_GPIO_Port, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI2_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI2_IRQn);

  /* USER CODE BEGIN MX_GPIO_Init_2 */

  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
/* ================= CONTROL DE 4 LEDs POR UART (VIRTUAL TERMINAL) =================
 * Comandos rapidos (una sola tecla, sin Enter):
 *    A = LED1 ON   a = LED1 OFF
 *    B = LED2 ON   b = LED2 OFF
 *    C = LED3 ON   c = LED3 OFF
 *    D = LED4 ON   d = LED4 OFF
 *    T = todos ON  t = todos OFF      ? = estado
 * Comandos de texto (terminar con Enter):
 *    L1=1  L1=0  ...  L4=1  L4=0   ALL=1  ALL=0   ?
 * LED1 = PA1 (LED), LED2 = PA2 (LED2), LED3 = PA0 (LED3), LED4 = PB0 (LED4)
 * ================================================================================ */
static GPIO_TypeDef * const led_port[4] = {LED_GPIO_Port, LED2_GPIO_Port, LED3_GPIO_Port, LED4_GPIO_Port};
static const uint16_t       led_pin[4]  = {LED_Pin,       LED2_Pin,       LED3_Pin,       LED4_Pin};

#define CMD_MAX 16
static char    cmd_buf[CMD_MAX];
static uint8_t cmd_len = 0;

static void led_set(uint8_t n, uint8_t on)   // n = 0..3
{
  HAL_GPIO_WritePin(led_port[n], led_pin[n], on ? GPIO_PIN_SET : GPIO_PIN_RESET);
  char msg[24];
  sprintf(msg, "\r\nLED%d %s\r\n", n + 1, on ? "ON" : "OFF");
  uartx_write_text(&huart1, msg);
}

static void led_all(uint8_t on)
{
  for (uint8_t i = 0; i < 4; i++) led_set(i, on);
}

static void led_estado(void)
{
  char msg[64];
  sprintf(msg, "\r\nLED1:%d LED2:%d LED3:%d LED4:%d\r\n",
          HAL_GPIO_ReadPin(led_port[0], led_pin[0]),
          HAL_GPIO_ReadPin(led_port[1], led_pin[1]),
          HAL_GPIO_ReadPin(led_port[2], led_pin[2]),
          HAL_GPIO_ReadPin(led_port[3], led_pin[3]));
  uartx_write_text(&huart1, msg);
}

void menu_leds(void)
{
  uartx_write_text(&huart1,
    "\r\n=== CONTROL DE LEDs ===\r\n"
    "A/a LED1 ON/OFF  B/b LED2 ON/OFF\r\n"
    "C/c LED3 ON/OFF  D/d LED4 ON/OFF\r\n"
    "T/t TODOS ON/OFF   ? estado\r\n"
    "o escriba L1=1 .. L4=0, ALL=1, ALL=0 + Enter\r\n> ");
}

// Atiende una tecla rapida. Devuelve 1 si la reconocio.
static uint8_t tecla_rapida(char c)
{
  switch (c)
  {
    case 'A': led_set(0, 1); return 1;   case 'a': led_set(0, 0); return 1;
    case 'B': led_set(1, 1); return 1;   case 'b': led_set(1, 0); return 1;
    case 'C': led_set(2, 1); return 1;   case 'c': led_set(2, 0); return 1;
    case 'D': led_set(3, 1); return 1;   case 'd': led_set(3, 0); return 1;
    case 'T': led_all(1);    return 1;   case 't': led_all(0);    return 1;
    case '?': led_estado();  return 1;
    default: return 0;
  }
}

// Procesa una linea completa (L1=1, ALL=0, ...)
static void procesa_linea(char *linea)
{
  for (char *p = linea; *p; p++) if (*p >= 'a' && *p <= 'z') *p -= 32;  // a mayusculas

  if (linea[0] == 'L' && linea[1] >= '1' && linea[1] <= '4' && linea[2] == '=' &&
      (linea[3] == '0' || linea[3] == '1') && linea[4] == 0)
  {
    led_set(linea[1] - '1', linea[3] == '1');
  }
  else if (strcmp(linea, "ALL=1") == 0) led_all(1);
  else if (strcmp(linea, "ALL=0") == 0) led_all(0);
  else if (strcmp(linea, "?") == 0)     led_estado();
  else if (linea[0] != 0)
  {
    uartx_write_text(&huart1, "\r\nComando no valido\r\n");
    menu_leds();
    return;
  }
  uartx_write_text(&huart1, "> ");
}

// Se llama con lo que llego por la UART (puede ser 1 tecla o varias)
void procesa_rx(uint8_t *datos, uint16_t n)
{
  for (uint16_t i = 0; i < n; i++)
  {
    char c = (char)datos[i];

    if (c == '\r' || c == '\n')                 // Enter: ejecuta la linea escrita
    {
      if (cmd_len > 0)
      {
        cmd_buf[cmd_len] = 0;
        procesa_linea(cmd_buf);
        cmd_len = 0;
      }
      continue;
    }

    if (cmd_len == 0 && tecla_rapida(c))        // tecla rapida (solo si no se esta escribiendo una linea)
    {
      uartx_write_text(&huart1, "> ");
      continue;
    }

    if (c == 8 || c == 127)                     // retroceso
    {
      if (cmd_len > 0) { cmd_len--; uartx_write_text(&huart1, "\b \b"); }
      continue;
    }

    if (cmd_len < CMD_MAX - 1)                  // guarda y hace eco
    {
      cmd_buf[cmd_len++] = c;
      uartx_write(&huart1, (uint8_t)c);
    }
  }
}
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
#ifdef USE_FULL_ASSERT
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
