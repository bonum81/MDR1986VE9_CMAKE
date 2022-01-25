/**
  ******************************************************************************
  * @file   can.c
  * @author  Illarionov Maksim
  * @version V1.0.0
  * @date    10/01/2022
  * @brief   Main program body.
  ******************************************************************************
  * <br><br>
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, MILANDR SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2021 Milandr</center></h2>
  */


/* Includes ------------------------------------------------------------------*/
#include "can.hpp"
#include "init.hpp"

struct GeneralSettingsCAN
{
    uint8_t  ID;
    uint16_t baudRate;
} GeneralSettingsCAN;


void CAN_SendMsg(FT_ProtocolPackageStruct *package)
{
	CAN_TxMsgTypeDef TxMsg;
	
	uint32_t usedCANBuf = 0;
	TxMsg.IDE = CAN_ID_STD;
	TxMsg.PRIOR_0 = DISABLE;

	TxMsg.ID = ((package->destinationAddress << 3) | (package->responseTypeSign << 2) | (package->packageNumber)) << 18;
	TxMsg.DLC = package->packageLength;
	package->data[0] = GeneralSettingsCAN.ID & 0xFF;
	TxMsg.Data[0] = package->data[3] << 24 | package->data[2] << 16 | package->data[1] << 8 | package->data[0];
	TxMsg.Data[1] = package->data[7] << 24 | package->data[6] << 16 | package->data[5] << 8 | package->data[4];

	CAN_Transmit(MDR_CAN1, usedCANBuf, &TxMsg);
}

void CAN_PositiveMsg(FT_ProtocolPackageStruct *package)
{
	CAN_TxMsgTypeDef TxMsg;

    uint32_t usedCANBuf = 0;
	TxMsg.IDE = CAN_ID_STD;
	TxMsg.PRIOR_0 = DISABLE;

	TxMsg.ID = ((package->destinationAddress << 3) | (package->packageNumber)) << 18;
	TxMsg.DLC = 2; 

	package->data[0] = GeneralSettingsCAN.ID & 0xFF;
	package->data[1] = 0xAA;
	
	TxMsg.Data[0] = package->data[3] << 24 | package->data[2] << 16 | package->data[1] << 8 | package->data[0];
	TxMsg.Data[1] = package->data[7] << 24 | package->data[6] << 16 | package->data[5] << 8 | package->data[4];

	CAN_Transmit(MDR_CAN1, usedCANBuf, &TxMsg);
}

void CAN_NegativeMsg(FT_ProtocolPackageStruct *package)
{
	CAN_TxMsgTypeDef TxMsg;

    uint32_t usedCANBuf = 0;
	TxMsg.IDE = CAN_ID_STD;
	TxMsg.PRIOR_0 = DISABLE;

	TxMsg.ID = ((package->destinationAddress << 3) | (package->packageNumber)) << 18;
	TxMsg.DLC = 1; 

	package->data[0] = GeneralSettingsCAN.ID & 0xFF;
	package->data[1] = 0xBB;
	
	TxMsg.Data[0] = package->data[3] << 24 | package->data[2] << 16 | package->data[1] << 8 | package->data[0];
	TxMsg.Data[1] = package->data[7] << 24 | package->data[6] << 16 | package->data[5] << 8 | package->data[4];


	CAN_Transmit(MDR_CAN1, usedCANBuf, &TxMsg);

}

void canSettingsHandler()
{
	CAN_InitTypeDef setingsCAN;
	CAN_FilterInitTypeDef filterCAN;

	switch(GeneralSettingsCAN.baudRate)
	{
		case 0x3FE0:							// 1000 кБод
		{
			setingsCAN.CAN_BRP  = 2;
			break;
		}
		case 0x40E0:							// 500 кБод
		{
			setingsCAN.CAN_BRP  = 5;
			break;
		}
		case 0x41E0: 							//300 кБод
		{
			setingsCAN.CAN_BRP  = 9;
			break;
		}
		case 0x42E0: 							//250 кБод
		{
			setingsCAN.CAN_BRP  = 11;
			break;
		}
		case 0x43E0: 							//150 кБод
		{
			setingsCAN.CAN_BRP  = 19;	
			break;
		}
		case 0x45E0:					 		//100 кБод
		{
			setingsCAN.CAN_BRP  = 29;
			break;
		}
		case 0x4BE0:					 		//50 кБод
		{
			setingsCAN.CAN_BRP  = 59;
			break;
		}
		case 0x5DE0: 							//20 кБод
		{
			setingsCAN.CAN_BRP  = 149;
			break;
		}
		default:		
		{
			setingsCAN.CAN_BRP  = 9;
			GeneralSettingsCAN.baudRate = 0x41E0;
			break;
		}
	}

	setingsCAN.CAN_ROP  = DISABLE;
    setingsCAN.CAN_SAP  = DISABLE;
    setingsCAN.CAN_STM  = DISABLE;
    setingsCAN.CAN_ROM  = DISABLE;
    setingsCAN.CAN_PSEG = CAN_PSEG_Mul_6TQ;
    setingsCAN.CAN_SEG1 = CAN_SEG1_Mul_7TQ;
    setingsCAN.CAN_SEG2 = CAN_SEG2_Mul_2TQ;
    setingsCAN.CAN_SJW  = CAN_SJW_Mul_1TQ;
    setingsCAN.CAN_SB   = CAN_SB_3_SAMPLE;

	CAN_Init(MDR_CAN1,&setingsCAN);

	CAN_ITConfig(MDR_CAN1, CAN_IT_RXINTEN, ENABLE); 	// RX_INT_EN = 1
	//CAN_Receive(MDR_CAN1, 0, ENABLE); 								
	CAN_Receive(MDR_CAN1, 1, DISABLE); 								
	//CAN_Receive(MDR_CAN1, 2, DISABLE); 								
	//CAN_Receive(MDR_CAN1, 3, DISABLE); 								
	//CAN_Receive(MDR_CAN1, 4, DISABLE); 								
	//CAN_Receive(MDR_CAN1, 5, DISABLE); 								
	MDR_CAN1->INT_RX |= (1 << 1); // |(1 << 1)|((1 << 2))|(1 << 3)|(1 << 4)|(1 << 5)

	filterCAN.Filter_ID 	= 	(GeneralSettingsCAN.ID << 21);
	filterCAN.Mask_ID		=	0x1FE00000;
	CAN_FilterInit(MDR_CAN1, 0, &filterCAN);

	filterCAN.Filter_ID 	= 	(GeneralSettingsCAN.ID << 21);
	filterCAN.Mask_ID		=	0x1FE00000;
	CAN_FilterInit(MDR_CAN1, 1, &filterCAN);

	filterCAN.Filter_ID 	= 	(GeneralSettingsCAN.ID << 21);
	filterCAN.Mask_ID		=	0x1FE00000;
	CAN_FilterInit(MDR_CAN1, 2, &filterCAN);

	filterCAN.Filter_ID 	= 	(GeneralSettingsCAN.ID << 21);
	filterCAN.Mask_ID		=	0x1FE00000;
	CAN_FilterInit(MDR_CAN1, 3, &filterCAN);

	filterCAN.Filter_ID 	= 	(GeneralSettingsCAN.ID << 21);
	filterCAN.Mask_ID		=	0x1FE00000;
	CAN_FilterInit(MDR_CAN1, 4, &filterCAN);

	filterCAN.Filter_ID 	= 	(GeneralSettingsCAN.ID << 21);
	filterCAN.Mask_ID		=	0x1FE00000;
	CAN_FilterInit(MDR_CAN1, 5, &filterCAN);

	CAN_ITConfig(MDR_CAN1, CAN_IT_GLBINTEN, ENABLE); 	// Разрешить прерывание CAN1
	NVIC_EnableIRQ(CAN1_IRQn);							// Разрешить обработчик прерывания CAN1
	CAN_Cmd(MDR_CAN1, ENABLE);
}

void  CAN_RX_Callback(void)
{
	CAN_RxMsgTypeDef RxMsg = {0x00, 0x00, 0x00 ,0x00, 0x00};
	uint32_t NumberOfBuffers = 0;
	uint32_t dataLenght = 0;
    FT_ProtocolPackageStruct recevePackege;

	if ((MDR_CAN1->STATUS&CAN_STATUS_ERR_STATUS_Msk) == CAN_STATUS_ERR_STATUS_Msk)		// error passive or BUS off
	{
		//write_LED(LED_CANRX, ENABLE);
		//write_LED(LED_CANTX, ENABLE);																														
	}
	if ((MDR_CAN1->STATUS&CAN_STATUS_RX_READY)== CAN_STATUS_RX_READY)					// флаг прерывания по приему сообщения
	{
		//write_LED(LED_CANRX, ENABLE);
		for (NumberOfBuffers = 1; NumberOfBuffers <= 5; NumberOfBuffers++)
		{
			if ((MDR_CAN1->BUF_CON[NumberOfBuffers]& 0x43) == 0x43)						// если в этом буфере есть данные
			{
				CAN_GetRawReceivedData(MDR_CAN1, NumberOfBuffers, &RxMsg);				// забрать данные из буфера
				
				recevePackege.data[0] = RxMsg.Data[0] & 0xFF;
				recevePackege.data[1] = (RxMsg.Data[0] & 0xFFFF) >> 8;
				recevePackege.data[2] = (RxMsg.Data[0] & 0xFFFFFF)>>16;
				recevePackege.data[3] = RxMsg.Data[0] >> 24;
				recevePackege.data[4] = RxMsg.Data[1] & 0xFF;
				recevePackege.data[5] = (RxMsg.Data[1] & 0xFFFF) >> 8;
				recevePackege.data[6] = (RxMsg.Data[1] & 0xFFFFFF) >> 16;
				recevePackege.data[7] = RxMsg.Data[1] >> 24;

				recevePackege.sourceAddress = recevePackege.data[0];
				recevePackege.packageNumber = (RxMsg.Rx_Header.ID >> 18) & 0x00000003;
				recevePackege.packageLength = RxMsg.Rx_Header.DLC;
				recevePackege.destinationAddress = RxMsg.Rx_Header.ID >> 21;
				recevePackege.responseTypeSign = (RxMsg.Rx_Header.ID >> 20) & 0x01;
				
				dataLenght = RxMsg.Rx_Header.DLC;

				for (;dataLenght < 8; dataLenght++)
				{
					recevePackege.data[dataLenght] = 0x00;
				}
				

				CAN_ITClearRxTxPendingBit(MDR_CAN1, NumberOfBuffers, CAN_STATUS_RX_READY); // очистить флаг занятости буфера
				
				if(recevePackege.responseTypeSign == 1)	//if plc receive request
				{
					if(recevePackege.data[1] <= 0x09)
					{
						//FT_Functions[recevePackege.data[1]](&recevePackege);
					}else
					{
						//need create error function
					}
				}else	//if plc receive response
				{
					//receiveResponseFunction(&recevePackege);
				}

				
			}
		}
	}
}


#if defined(__cplusplus)
extern "C"{
#endif  

void CAN1_IRQHandler(void)
{
	CAN_RX_Callback();
}

#if defined(__cplusplus)
}
#endif
