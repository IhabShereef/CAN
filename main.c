#include "main.h"
#include "stm32f4xx_hal_can.h"		//add stm32f4xx_hal_can.c too
#include "CAN.h"

Can_HandleTypeDef hcan1;

CAN_TxHeaderTypeDef txheader;
CAN_RxHeaderTypeDef rxheader;

uint8_t txdata[3];

uint32_t txmailbox; 



int main(void)
{
  HAL_CAN_Start(&hcan);
  HAL_CAN_ActivateNotification(&hcan, CAN_IT_RX_FIFO1_MSG_PENDING);


  TxHeader.DLC = 2;  // data length
  TxHeader.IDE = CAN_ID_STD;
  TxHeader.RTR = CAN_RTR_DATA;
  TxHeader.StdId = 0x103;

  CAN_FilterTypeDef filter1;

  	filter1.FilterBank = 14;
	filter1.FilterActivation = ENABLE;
	filter1.FilterFIFOAssignment = 0;
	filter1.FilterIdHigh = 0x125 << 5;
	filter1.FilterIdLow = 0x0000;
	filter1.FilterMaskIdHigh = 0xFFE0;  //if(Mask & 0x124 == 0x124) --> Alow
	filter1.FilterMaskIdLow = 0x0000;
	filter1.FilterMode = CAN_FILTERMODE_IDMASK;
	filter1.SlaveStartFilterBank = 0;
	filter1.FilterScale = CAN_FILTERSCALE_32BIT;

  while(1)
  {
	HAL_CAN_Start(&hcan1);
  }
}
