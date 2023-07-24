/* USER CODE BEGIN Header*/
	/**
	  ******************************************************************************
	  * @file           : main.c
	  * @brief          : Main program body
	  ******************************************************************************
	  * @attention
	  *
	  * Copyright (c) 2023 STMicroelectronics.
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
#include <stdio.h>
#include "stm32f4xx.h"
#include <string.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
typedef struct {
    char gearCombination[3]; // Store gear combination (e.g., "LR", "N", "L1", etc.)
    char stringValue[4];     // Store the corresponding string value (3 characters + '\0' null-terminator)
} GearInfo;

GearInfo gearInfoList[] = {
    { "R", "001" }, 
    { "L1", "101" }, 
    { "L2", "102" }, 
    { "L3", "103" }, 
    { "H1", "111" }, 
    { "H2", "112" }, 
    { "H3", "113" },
};
#define GEAR_INFO_LIST_SIZE (sizeof(gearInfoList) / sizeof(gearInfoList[0]))
const char* getGearStringValue(const char* gearCombination) {
    for (size_t i = 0; i < GEAR_INFO_LIST_SIZE; i++) {
        if (strcmp(gearCombination, gearInfoList[i].gearCombination) == 0) {
            return gearInfoList[i].stringValue;
        }
    }
    return NULL; // Return NULL if gear combination is not found in the lookup table
}
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
CAN_HandleTypeDef hcan1;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_CAN1_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
CAN_TxHeaderTypeDef TxHeader;
CAN_RxHeaderTypeDef RxHeader;

uint8_t TxData[8];
uint8_t RxData[8];


uint32_t TxMailbox;

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
	char gearCombination[5];
	char gearMode[5];
	uint8_t gearIP;
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
  MX_CAN1_Init();
  /* USER CODE BEGIN 2 */
  HAL_CAN_Start(&hcan1);

    HAL_CAN_ActivateNotification(&hcan1,CAN_IT_RX_FIFO0_MSG_PENDING);

    TxHeader.DLC =2;

    TxHeader.IDE =CAN_ID_STD;
    TxHeader.RTR = CAN_RTR_DATA;
    TxHeader.StdId =0x407;
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	  while (1)
	  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
		  
		  GPIO_PinState rotarySwitchState0 = HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_0);
		  GPIO_PinState rotarySwitchState1 = HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_1);
		  GPIO_PinState rotarySwitchState2 = HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_2);
		  GPIO_PinState rotarySwitchState3 = HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_3);

		  GPIO_PinState limitSwitchState1 = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_1);
		  GPIO_PinState limitSwitchState2 = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_2);
		  GPIO_PinState limitSwitchState3 = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_4);
		  
		  GPIO_PinState limitSwitchStatea = HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_10); //Limit switch corresponding to L
		  GPIO_PinState limitSwitchStateb = HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_11); //Limit switch corresponding to H
		  GPIO_PinState limitSwitchStatec = HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_12); //Limit switch corresponding to 1
		  GPIO_PinState limitSwitchStated = HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_13); //Limit switch corresponding to 2
		  GPIO_PinState limitSwitchStatee = HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_14); //Limit switch corresponding to 3
		  GPIO_PinState limitSwitchStatef = HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_15); //Limit switch corresponding to R
		 	  
		 	  if (rotarySwitchState0 == GPIO_PIN_SET){
		 		  strcpy(gearCombination, "LR");
		 	  }
		 	  else if (rotarySwitchState1 == GPIO_PIN_SET){
		 		  strcpy(gearCombination, "N");
		 	  }
		 	  else if (rotarySwitchState2 == GPIO_PIN_SET){
		 	      gearCombination[0] = "L";
		 	  }
		 	  else if (rotarySwitchState3 == GPIO_PIN_SET){
		 	      gearCombination[0] = "H";
		 	  }
		 	  else {
		 	      printf("Error in Rotary switch");
		 	  }

		 	  if (gearCombination[0]=="L"|| gearCombination[0]=="H")
		 	  {
		 	      if (limitSwitchState1 == GPIO_PIN_SET){
		 	      gearCombination[1] = "1";
		 	      }
		 	  else if (limitSwitchState2 == GPIO_PIN_SET){
		 	      gearCombination[1] = "2";
		 	  	  }
		 	  else if (rotarySwitchState3 == GPIO_PIN_SET){
		 	      gearCombination[1] = "3";
		 	  	  }
		 	  }
		 	  if(limitSwitchStatea == GPIO_PIN_SET && limitSwitchStatec == GPIO_PIN_SET)
		 	  {
		 		  strcpy(gearMode, "L1");
		 	  }
		 	  else if(limitSwitchStatea == GPIO_PIN_SET && limitSwitchStated  == GPIO_PIN_SET)
		 	 {
		 		  strcpy(gearMode, "L2");
		 	 }
		 	 else if(limitSwitchStatea == GPIO_PIN_SET && limitSwitchStatee  == GPIO_PIN_SET)
		 	 {
		 	 	  strcpy(gearMode, "L3");
		 	 }
		 	else if(limitSwitchStateb == GPIO_PIN_SET && limitSwitchStatec  == GPIO_PIN_SET)
		 	{
		 		  strcpy(gearMode, "H1");
		 	}
		 	else if(limitSwitchStateb == GPIO_PIN_SET && limitSwitchStated  == GPIO_PIN_SET)
		 	{
		 		  strcpy(gearMode, "H2");
		 	}
		 	else if(limitSwitchStateb == GPIO_PIN_SET && limitSwitchStatee  == GPIO_PIN_SET)
		 	{
		 		  strcpy(gearMode, "H3");
		 	}  
		 	
		 if(!strcmp(gearMode,gearCombination))
		 {
			 
		 }
		 else 
		 {
			printf("Error!"); 
		 }
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
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

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
  * @brief CAN1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_CAN1_Init(void)
{

  /* USER CODE BEGIN CAN1_Init 0 */

  /* USER CODE END CAN1_Init 0 */

  /* USER CODE BEGIN CAN1_Init 1 */

  /* USER CODE END CAN1_Init 1 */
  hcan1.Instance = CAN1;
  hcan1.Init.Prescaler = 16;
  hcan1.Init.Mode = CAN_MODE_NORMAL;
  hcan1.Init.SyncJumpWidth = CAN_SJW_1TQ;
  hcan1.Init.TimeSeg1 = CAN_BS1_1TQ;
  hcan1.Init.TimeSeg2 = CAN_BS2_1TQ;
  hcan1.Init.TimeTriggeredMode = DISABLE;
  hcan1.Init.AutoBusOff = DISABLE;
  hcan1.Init.AutoWakeUp = DISABLE;
  hcan1.Init.AutoRetransmission = DISABLE;
  hcan1.Init.ReceiveFifoLocked = DISABLE;
  hcan1.Init.TransmitFifoPriority = DISABLE;
  if (HAL_CAN_Init(&hcan1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN CAN1_Init 2 */

  /* USER CODE END CAN1_Init 2 */

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
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pins : PB1 PB2 PB3 */
  GPIO_InitStruct.Pin = GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : PD10 PD11 PD12 PD13
                           PD14 PD15 PD0 PD1
                           PD2 PD3 */
  GPIO_InitStruct.Pin = GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_13
                          |GPIO_PIN_14|GPIO_PIN_15|GPIO_PIN_0|GPIO_PIN_1
                          |GPIO_PIN_2|GPIO_PIN_3;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
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
