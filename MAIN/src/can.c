#include "can.h"
#include "init.h"

GenSetCAN GeneralSettingsCAN;
uint16_t massiveParameterNumber = 0; 

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
	write_LED(LED_CANTX, ENABLE);
	CAN_Transmit(MDR_CAN1, usedCANBuf, &TxMsg);
	/*
	CAN_TxHeaderTypeDef TxMessage;
    uint32_t usedMailbox;
	TxMessage.StdId = (package->destinationAddress << 3) | (package->responseTypeSign << 2) | (package->packageNumber);
	TxMessage.ExtId = 0x00;
	TxMessage.IDE = CAN_ID_STD;
	TxMessage.RTR = CAN_RTR_DATA; 
	TxMessage.DLC = package->packageLength;
	package->data[0] = GeneralSettingsCAN.ID & 0xFF;
	HAL_CAN_AddTxMessage(&hcan_handle, &TxMessage, package->data, &usedMailbox);
	*/
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
	write_LED(LED_CANTX, ENABLE);
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
	write_LED(LED_CANTX, ENABLE);
	CAN_Transmit(MDR_CAN1, usedCANBuf, &TxMsg);

	/*
	CAN_TxHeaderTypeDef TxMessage;
    uint32_t *usedMailbox = 0;
	TxMessage.StdId = (package->destinationAddress << 3) | (package->packageNumber);          
	TxMessage.ExtId = 0x00;                          
	TxMessage.IDE = CAN_ID_STD;              
	TxMessage.RTR = CAN_RTR_DATA;                    
	TxMessage.DLC = 1;                              
	package->data[0] = GeneralSettingsCAN.ID & 0xFF;
	package->data[1] = 0xBB;
	HAL_CAN_AddTxMessage(&hcan_handle, &TxMessage, package->data, usedMailbox);
	*/
}

static uint32_t SyncMemory(FT_ProtocolPackageStruct *Package)
{
	//ParametersSystem.doMemorySync();
	FT_ProtocolPackageStruct responsePackage;
    responsePackage.destinationAddress = Package->sourceAddress;
	responsePackage.packageNumber = Package->packageNumber;
    responsePackage.responseTypeSign = 0;
    responsePackage.sourceAddress = GeneralSettingsCAN.ID;
    responsePackage.packageLength = 2;
	CAN_PositiveMsg(&responsePackage);
	return 0;
}

uint32_t NOPE(FT_ProtocolPackageStruct *Package)
{
	FT_ProtocolPackageStruct responsePackage;
    responsePackage.destinationAddress = Package->sourceAddress;
	responsePackage.packageNumber = Package->packageNumber;
    responsePackage.responseTypeSign = 0;
    responsePackage.sourceAddress = GeneralSettingsCAN.ID;
    responsePackage.packageLength = 2;
	CAN_NegativeMsg(&responsePackage);
	return 0;
}

uint32_t ReadParam(FT_ProtocolPackageStruct *receivePackage)
{
    FT_ProtocolPackageStruct responsePackage;
    // responsePackage.destinationAddress = receivePackage->sourceAddress;
    // uint16_t paramNumber = (receivePackage->data[3] << 8) | (receivePackage->data[2]);
    // ParamDescript Param = ParametersSystem.getParameter(paramNumber);
	// responsePackage.packageNumber = receivePackage->packageNumber;
    // responsePackage.responseTypeSign = 0;
    // responsePackage.sourceAddress = GeneralSettingsCAN.ID;
	// /*
	// if((Param.ParametersAddress == 0) || (Param.Description.readProtectionType > CurrentState.currentAccessLevel))
	// {
	// 	CAN_NegativeMsg(&responsePackage);
	// 	return 0;
	// }
	// */
    // //================== Проверка на наличие параметра ====================//
	// uint8_t *temporaryParameter = (uint8_t*)Param.ParametersAddress;
	// if(Param.Description.bitTag == 0)
	// {
	// 	for(int i = 0; i < Param.Description.parameterSize; i++)
	// 	{
	// 		responsePackage.data[i + 1] = (*(uint8_t*)(temporaryParameter));
	// 		temporaryParameter++;
	// 	}
	// 	responsePackage.packageLength = Param.Description.parameterSize + 1;
	// }else{
	// 	responsePackage.data[1] = ((*(uint8_t*)(temporaryParameter)) & (0x1 << Param.Description.parameterSize)) >> Param.Description.parameterSize;
	// 	responsePackage.packageLength = 2;
	// }
    CAN_SendMsg(&responsePackage);
    return 0;
}

uint32_t WriteParam(FT_ProtocolPackageStruct *receivePackage)
{
	// uint16_t paramNumber = (receivePackage->data[3] << 8) | (receivePackage->data[2]);
	// uint32_t newParameterValue = 0;
	// for(int i = 0; i < 4; i++)
	// {
	// 	newParameterValue = newParameterValue | (receivePackage->data[i + 4] << (i * 8));
	// }
	// ParamDescript Param = ParametersSystem.getParameter(paramNumber);
    FT_ProtocolPackageStruct responsePackage;
    // responsePackage.destinationAddress = receivePackage->sourceAddress;
	// responsePackage.packageNumber 	 = receivePackage->packageNumber;
    // responsePackage.responseTypeSign = 0;
    // responsePackage.sourceAddress  	 = GeneralSettingsCAN.ID;
	// if((Param.ParametersAddress == 0) || (Param.Description.writeProtectionType > CurrentState.currentAccessLevel) || (Param.Description.writeProtectionType == 0))
	// {
	// 	CAN_NegativeMsg(&responsePackage);
	// 	return 0;
	// }
	// ParametersSystem.setParameter(paramNumber, newParameterValue);
	//добавить проверку о соответствии параметра
    CAN_PositiveMsg(&responsePackage);
    return 0;
}

uint32_t SetMassiveParameterNumber(FT_ProtocolPackageStruct *receivePackage)
{
	// FT_ProtocolPackageStruct responsePackage;
    // responsePackage.destinationAddress = receivePackage->sourceAddress;
    // uint16_t paramNumber = (receivePackage->data[3] << 8) | (receivePackage->data[2]);
    // ParamDescript Param = ParametersSystem.getParameter(paramNumber);
	// responsePackage.packageNumber 	 = receivePackage->packageNumber;
    // responsePackage.responseTypeSign = 0;
    // responsePackage.sourceAddress  	 = GeneralSettingsCAN.ID;
	// if(Param.ParametersAddress == 0)
	// {
	// 	CAN_NegativeMsg(&responsePackage);
	// 	return 0;
	// }
	// if(Param.Description.massiveTag != 0)
	// {
	// 	massiveParameterNumber = (receivePackage->data[3] << 8) | (receivePackage->data[2]);
	// 	CAN_PositiveMsg(&responsePackage);
	// }else
	// {
	// 	CAN_NegativeMsg(&responsePackage);
	// }
	return 0;	
}

uint32_t ReadMassiveParam(FT_ProtocolPackageStruct *receivePackage)
{
    FT_ProtocolPackageStruct responsePackage;
    // responsePackage.destinationAddress = receivePackage->sourceAddress;
    // uint16_t paramNumber = (receivePackage->data[3] << 8) | (receivePackage->data[2]);
	// uint32_t massiveIndex = (receivePackage->data[6] << 16) | (receivePackage->data[5] << 8) | receivePackage->data[4];
	// ParamDescript Param = ParametersSystem.getMassiveParameter(paramNumber, massiveIndex);
    // //================== Проверка на наличие параметра ====================//
	// uint8_t *temporaryParameter = (uint8_t*)Param.ParametersAddress;
    // responsePackage.packageNumber = receivePackage->packageNumber;
    // responsePackage.responseTypeSign = 0;
    // responsePackage.sourceAddress = GeneralSettingsCAN.ID;
    // responsePackage.packageLength = Param.Description.parameterSize + 1;
	// if((Param.ParametersAddress == 0) || (Param.Description.readProtectionType > CurrentState.currentAccessLevel))
	// {
	// 	CAN_NegativeMsg(&responsePackage);
	// 	return 0;
	// }
	// for(int i = 0; i < Param.Description.parameterSize; i++)
    // {
	// 	responsePackage.data[i + 1] = (*(uint8_t*)(temporaryParameter));
	// 	temporaryParameter++;
    // }
    CAN_SendMsg(&responsePackage);
    return 0;
}

uint32_t WriteMassiveParam(FT_ProtocolPackageStruct *receivePackage)
{
    FT_ProtocolPackageStruct responsePackage;
    // responsePackage.destinationAddress = receivePackage->sourceAddress;
    // //================== Проверка на наличие параметра ====================//
	// uint32_t massiveIndex = (receivePackage->data[3] << 8) | receivePackage->data[2];
	// uint32_t newParameterValue = 0;
    // responsePackage.packageNumber = receivePackage->packageNumber;
    // responsePackage.responseTypeSign = 0;
    // responsePackage.sourceAddress = GeneralSettingsCAN.ID;
    // responsePackage.packageLength = 2;
	// for(int i = 0; i < 4; i++)
	// {
	// 	newParameterValue = newParameterValue | (receivePackage->data[i + 4] << (i * 8));
	// }
	// if(massiveParameterNumber == 0)
	// {
	// 	CAN_NegativeMsg(&responsePackage);
	// 	return 0;
	// }
	// ParamDescript Param = ParametersSystem.setMassiveParameter(massiveParameterNumber, massiveIndex, newParameterValue);
	// if((Param.ParametersAddress == 0) || (Param.Description.writeProtectionType > CurrentState.currentAccessLevel) || (Param.Description.writeProtectionType == 0))
	// {
	// 	CAN_NegativeMsg(&responsePackage);
	// 	return 0;
	// }
	CAN_PositiveMsg(&responsePackage);
    return 0;
}

uint32_t AccessLevel(FT_ProtocolPackageStruct *receivePackage)
{  
	volatile uint32_t Password = 0;
	FT_ProtocolPackageStruct responsePackage;
    // responsePackage.destinationAddress = receivePackage->sourceAddress;
    // responsePackage.packageNumber = receivePackage->packageNumber;
    // responsePackage.responseTypeSign = 0;
    // responsePackage.sourceAddress = GeneralSettingsCAN.ID;
    // responsePackage.packageLength = 2;
	// switch(receivePackage->data[2])
	// {
	// 	case 0x01:
	// 	{
	// 		responsePackage.data[1] = 0x01;
	// 		CurrentState.currentAccessLevel = 1;
	// 		CAN_SendMsg(&responsePackage);
	// 		break;
	// 	}
	// 	case 0x02:
	// 	{
	// 		uint32_t *StartAddress = ParametersSystem.getParameter(0xF025).ParametersAddress;
	// 		if(receivePackage->packageLength == 3)
	// 		{
	// 			if((*(StartAddress) == 0xFFFFFFFF) || (*(StartAddress) == 0x00000000))
	// 			{
	// 				responsePackage.data[1] = 0x02;
	// 				CurrentState.currentAccessLevel = 2;
	// 				CAN_SendMsg(&responsePackage);
	// 			}else
	// 			{
	// 				responsePackage.data[1] = 0x01;
	// 				CurrentState.currentAccessLevel = 1;
	// 				CAN_SendMsg(&responsePackage);
	// 			}
	// 		}else if(receivePackage->packageLength == 7)
	// 		{
	// 			for(int i = 0; i < 4; i++)
	// 			{
	// 				Password |= receivePackage->data[i + 3] << i * 8;
	// 			}
	// 			if(*(__IO uint32_t*)(StartAddress) == Password)
	// 			{
	// 				responsePackage.data[1] = 0x02;
	// 				CurrentState.currentAccessLevel = 2;
	// 				CAN_SendMsg(&responsePackage);
	// 			}else
	// 			{
	// 				responsePackage.data[1] = 0x01;
	// 				CurrentState.currentAccessLevel = 1;
	// 				CAN_SendMsg(&responsePackage);
	// 			}
	// 		}
	// 		break;
	// 	}
	// 	case 0x03:
	// 	{
	// 		uint32_t devicePassword = *(uint32_t*)ParametersSystem.getParameter(0xF026).ParametersAddress;
	// 		if(receivePackage->packageLength == 3)
	// 		{
	// 			if((devicePassword == 0xFFFFFFFF) || (devicePassword == 0x00000000))
	// 			{
	// 				responsePackage.data[1] = 0x03;
	// 				CurrentState.currentAccessLevel = 3;
	// 				CAN_SendMsg(&responsePackage);
	// 			}else
	// 			{
	// 				responsePackage.data[1] = 0x01;
	// 				CurrentState.currentAccessLevel = 1;
	// 				CAN_SendMsg(&responsePackage);
	// 			}
	// 		}else if(receivePackage->packageLength == 7)
	// 		{
	// 			for(int i = 0; i < 4; i++)
	// 			{
	// 				Password |= receivePackage->data[i + 3] << i * 8;
	// 			}
	// 			if(devicePassword == Password)
	// 			{
	// 				responsePackage.data[1] = 0x03;
	// 				CurrentState.currentAccessLevel = 3;
	// 				CAN_SendMsg(&responsePackage);
	// 			}else
	// 			{
	// 				responsePackage.data[1] = 0x01;
	// 				CurrentState.currentAccessLevel = 1;
	// 				CAN_SendMsg(&responsePackage);
	// 			}
	// 		}
	// 		break;
	// 	}
	// 	default:
	// 	{
	// 		CAN_NegativeMsg(&responsePackage);
	// 		break;
	// 	}
	// }
	return 0;
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

uint32_t (*FT_Functions[])(FT_ProtocolPackageStruct*) =    //<! Массив функций, соответствующих кодам команд по CAN FT1.2
{
	NOPE,										/*00:*/
	NOPE,										/*01:*/
	ReadParam,									/*02:*/
	WriteParam,									/*03:*/
	SyncMemory,									/*04:*/
	AccessLevel,								/*05:*/
	SetMassiveParameterNumber,	                /*06:*/
	WriteMassiveParam,						    /*07:*/
	ReadMassiveParam,							/*08:*/
	NOPE										/*09:*/
};


//========================== Response function =====================//

uint32_t errorCodeFunction(FT_ProtocolPackageStruct* receivePacket)
{
	return 0;
}

uint32_t receiveResponseFunction(FT_ProtocolPackageStruct* receivePacket)
{
	// FT_ProtocolDequeStruct *CanPacket;
	
	// CanPacket = ControllerAreaNetwork::getFrontElementFromQueue();
	// uint32_t dequeSize = ControllerAreaNetwork::CAN_GetDequeSize();
	// if(dequeSize == 0)
	// {
	// 	return false;
	// }
	// if(CanPacket->package.packageNumber == receivePacket->packageNumber)
	// {
	// 	if(CanPacket->FunctionalCode > 2)
	// 	{
	// 		return false;
	// 	}
	// 	CanResponseTable[CanPacket->FunctionalCode](CanPacket, receivePacket);
	// }else
	// {
	// 	ControllerAreaNetwork::updateFrontQueuePacket(CanPacket, true);
	// 	return false;
	// }
	return 0;
}

void  CAN_RX_Callback(void)
{
	CAN_RxMsgTypeDef RxMsg = {0x00, 0x00, 0x00 ,0x00, 0x00};
	uint32_t NumberOfBuffers = 0;
	uint32_t dataLenght = 0;
    FT_ProtocolPackageStruct recevePackege;

	if ((MDR_CAN1->STATUS&CAN_STATUS_ERR_STATUS_Msk) == CAN_STATUS_ERR_STATUS_Msk)		// error passive or BUS off
	{
		write_LED(LED_CANRX, ENABLE);
		write_LED(LED_CANTX, ENABLE);																														
	}
	if ((MDR_CAN1->STATUS&CAN_STATUS_RX_READY)== CAN_STATUS_RX_READY)					// флаг прерывания по приему сообщения
	{
		write_LED(LED_CANRX, ENABLE);
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
						FT_Functions[recevePackege.data[1]](&recevePackege);
					}else
					{
						//need create error function
					}
				}else	//if plc receive response
				{
					receiveResponseFunction(&recevePackege);
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
