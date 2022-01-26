/**
  ******************************************************************************
  * @file    ft_can.c
  * @author  Illarionov Maksim
  * @version V1.0.0
  * @date    10/01/2022
  * @brief   FT_CAN protocol.
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
#ifdef __cplusplus
extern "C"{
#endif
/* C Librares ----------------------------------------------------------------*/
#include "socket.h"

#ifdef __cplusplus
}
#endif

/* CPP Librares --------------------------------------------------------------*/
#include <list>
#include <algorithm>
#include "ft_can.hpp"
#include "ParamSys.hpp"
#include "swTimers.hpp"

using namespace std;

/* Private variables ---------------------------------------------------------*/
uint8_t responsePacket[20];
extern ParamSys& ParametersSystem;
extern CurState CurrentState;
bool CanBusyFlag = false;
uint8_t packetNumber = 0;

// list <FT_ProtocolDequeStruct*> *CanDeque;
// list <FT_ProtocolDequeStruct*>::iterator *CanDequeIterator;
// SwTimer *CanFaultTimer;

list <FT_ProtocolDequeStruct*> CanDeque;
list <FT_ProtocolDequeStruct*>::iterator *CanDequeIterator;
SwTimer CanFaultTimer(500, (void*)ControllerAreaNetwork::canTimeoutHandler, false);

/* Private function prototypes -----------------------------------------------*/
static uint32_t ReadParam(uint8_t *data, uint32_t *len);
static uint32_t ReadMassiveParam(uint8_t *data, uint32_t *len);
static uint32_t AccessLevel(uint8_t *data, uint32_t *len);

void initFtContainers()
{
    // CanDeque = new list <FT_ProtocolDequeStruct*>;
    // CanDequeIterator = new list <FT_ProtocolDequeStruct*>::iterator;
    // CanFaultTimer = new SwTimer(500, (void*)ControllerAreaNetwork::canTimeoutHandler, false);
}

namespace ControllerAreaNetwork
{
    //============================= Request functions ==========================//
    
    bool CAN_ErrorRequestFunctionCode(FT_ProtocolDequeStruct *CanPacket)   //<! CAN Error function code handler
    {
        return (false);
    }
    uint32_t CAN_GetDequeSize()
    {
        // return CanDeque->size();
        return CanDeque.size();
    }
    bool CAN_SendRequest_ReadFloatParam(FT_ProtocolDequeStruct *CanPacket)
    {
        CAN_SendMsg(&CanPacket->package);
        return (true);
    }

    //============================= Response functions ==========================//

    bool CAN_ErrorResponseFunctionCode(FT_ProtocolDequeStruct *CanPacket, FT_ProtocolPackageStruct *receivePacket)   //<! CAN Error function code handler
    {
        return (false);
    }

    bool CAN_ReceiveResponse_ReadFloatParam(FT_ProtocolDequeStruct *CanPacket, FT_ProtocolPackageStruct *receivePacket)
    {
        // uint16_t paramNumber = CanPacket->paramNumber;
        // ParamDescript Param = ParametersSystem.getParameter(paramNumber);
        // uint8_t paramSize = Param.Description.parameterSize;
        // if(Param.Description.bitTag == false)                           
        // {
        //     if(paramSize == (receivePacket->packageLength - 1))
        //     {
        //         uint32_t newParameterValue = 0;
        //         for(int i = 0; i < receivePacket->packageLength - 1; i++)
        //         {
        //             newParameterValue = newParameterValue | (receivePacket->data[i + 1] << (i * 8));
        //         }
        //         ParametersSystem.setParameter(paramNumber, newParameterValue);
        //         ControllerAreaNetwork::updateFrontQueuePacket(CanPacket, false);
        //     }else
        //     {
        //         ControllerAreaNetwork::updateFrontQueuePacket(CanPacket, true);
        //         return false;
        //     }
        // }else
        // {
        //     uint32_t newParameterValue = 0;
            
        //     for(int i = 0; i < receivePacket->packageLength - 1; i++)
        //     {
        //         newParameterValue = newParameterValue | (receivePacket->data[i + 1] << (i * 8));
        //     }
        //     if (newParameterValue > 0)              // Если пришел не ноль то пишем еденицу
        //     {
        //         newParameterValue = 1;
        //     }else
        //     {
        //         newParameterValue = 0;
        //     }
        //     ParametersSystem.setParameter(paramNumber, newParameterValue);
        //     ControllerAreaNetwork::updateFrontQueuePacket(CanPacket, false);
        // }
        uint32_t lenghtPacket = receivePacket->packageLength;
        uint32_t lastElement = 5;
        uint8_t CheckSum = 0;
        switch (CanPacket->FTCommandCode)
        {
        case 0x11:
            /* code */
            responsePacket[0] = 0x68;
            responsePacket[1] = 2 + (receivePacket->packageLength-1);
            responsePacket[2] = 2 + (receivePacket->packageLength-1);
            responsePacket[3] = 0x68;
            responsePacket[4] = packetNumber&0xF;
            responsePacket[5] = receivePacket->destinationAddress;
            for(uint32_t i = 1; i < lenghtPacket; i++)
            {
                lastElement++;
                responsePacket[lastElement] = receivePacket->data[i];
                CheckSum = CheckSum + receivePacket->data[i];
            }
            CheckSum = (CheckSum + responsePacket[4] + responsePacket[5])&0xFF;
            responsePacket[++lastElement] = CheckSum;
            responsePacket[++lastElement] = 0x16;

            send(0, responsePacket, ++lastElement);
            ControllerAreaNetwork::updateFrontQueuePacket(CanPacket, false);

            break;
        case 0x14:
            responsePacket[0] = 0x68;
            responsePacket[1] = 0x03;
            responsePacket[2] = 0x03;
            responsePacket[3] = 0x68;
            responsePacket[4] = packetNumber&0xF;
            responsePacket[5] = receivePacket->destinationAddress;
            responsePacket[6] = receivePacket->data[1];
            responsePacket[7] = (responsePacket[4]+responsePacket[5]+responsePacket[6])&0xFF;
            responsePacket[8] = 0x16;

            send(0, responsePacket, 9);

            ControllerAreaNetwork::updateFrontQueuePacket(CanPacket, false);
            break;
        case 0x19:
            responsePacket[0] = 0x68;
            responsePacket[1] = 0x03;
            responsePacket[2] = 0x03;
            responsePacket[3] = 0x68;
            responsePacket[4] = packetNumber&0xF;
            responsePacket[5] = receivePacket->destinationAddress;
            send(0, responsePacket, 9);
            ControllerAreaNetwork::updateFrontQueuePacket(CanPacket, false);
            break;
        default:
            break;
        }                         
        return 0;
    }

    FT_ProtocolDequeStruct* getFrontElementFromQueue()
    {
        //return CanDeque->begin().operator*();
        return CanDeque.begin().operator*();
    }

    void eraseFrontElementFromQueue()
    {
        // if(CanDeque->size() != 0)
        // {
        //     *CanDequeIterator = CanDeque->begin();
        //     CanDeque->erase(*CanDequeIterator);
        // }
        if(CanDeque.size() != 0)
        {
            *CanDequeIterator = CanDeque.begin();
            CanDeque.erase(*CanDequeIterator);
        }
    }

    void updateFrontQueuePacket(FT_ProtocolDequeStruct *CanPacket, bool packetFaultFlag)
    {
        if(packetFaultFlag == true)                                             // Если есть флаг ошибки
        {
            if(CanPacket->faultCounterLimit > 0)                                // Если лимит счетчика допустимых ошибок больше 0
            {
                CanPacket->faultCounter++;                                      // Добавляем ошибку в счетчик ошибок
                if(CanPacket->faultCounter >= CanPacket->faultCounterLimit)     // Если счетчик ошибок привысил или равен лимиту
                {
                    CanPacket->Fault = true;                                    // Флаг ошибки
                    CanPacket->InProgress = false;                              // Флаг выполнения 
                    eraseFrontElementFromQueue();                               // Удалить элемент из очереди
                }
                else                                                            
                {
                    if(CanPacket->resetFlag == false)                           // Нет флага сброса
                    {
                        pushBackToQueue(CanPacket);                             // Добавить в конец очереди
                    }
                    else
                    {  
                        CanPacket->InProgress = false;                          // Сбросить флаг выполнения
                    }
                    eraseFrontElementFromQueue();                               // Удалить текущий первый элемент очереди
                }

                CanBusyFlag = false;   // Сбросить флаг занятости

            }
            else if(CanPacket->faultCounterLimit == 0)          // Если лимит по ошибкам 0
            {
                eraseFrontElementFromQueue();                   // Стираем первый течущий элемент очереди
                CanPacket->Fault = true;                        
                CanPacket->InProgress = false;
                CanBusyFlag = false;
            }
        }
        else
        { 
            eraseFrontElementFromQueue();
            CanPacket->InProgress = false;
            CanBusyFlag = false;
        }
        CanTransferInitiate();
    }

    void canTimeoutHandler()
    {
        // if(CanDeque->size() == 0)               // Очередь пуста, выходим
        // {
        //     return;
        // }else
        if(CanDeque.size() == 0)               // Очередь пуста, выходим
        {
            return;
        }else
        {
            FT_ProtocolDequeStruct *CanPacket = getFrontElementFromQueue();  // Получаем первый элемент
            updateFrontQueuePacket(CanPacket, true);                         // 
        }
    }

    void pushBackToQueue(FT_ProtocolDequeStruct *CanPacket)
    {
        // CanDeque->push_back(CanPacket);
        CanDeque.push_back(CanPacket);
    }

    void CanTransferInitiate()
    {
        FT_ProtocolDequeStruct *currentRequest;
        // if((CanBusyFlag == false) && (CanDeque->size() != 0))                       // Если CAN не занят и очередь не пуста
        // {
        //     currentRequest = ControllerAreaNetwork::getFrontElementFromQueue();     // Получаем первый элемент очереди
        //     CanRequestTable[currentRequest->FunctionalCode](currentRequest);        // Запускаем запрос исходя из таблицы
        //     CanBusyFlag = true;                                                     // Ставим флаг занятости CAN
        //     CanFaultTimer->reloadTimer();                                           // Перезагружаем таймер ошибки
        //     CanFaultTimer->startTimer();                                            // Запускаем таймер
        // }
        if((CanBusyFlag == false) && (CanDeque.size() != 0))                       // Если CAN не занят и очередь не пуста
        {
            currentRequest = ControllerAreaNetwork::getFrontElementFromQueue();     // Получаем первый элемент очереди
            CanRequestTable[currentRequest->FunctionalCode](currentRequest);        // Запускаем запрос исходя из таблицы
            CanBusyFlag = true;                                                     // Ставим флаг занятости CAN
            CanFaultTimer.reloadTimer();                                           // Перезагружаем таймер ошибки
            CanFaultTimer.startTimer();                                            // Запускаем таймер
        }
    }

    void addPacketToQueue(FT_ProtocolDequeStruct *CanPacket)    
    {
        ControllerAreaNetwork::pushBackToQueue(CanPacket);                          // Добавляем пакет в конец очереди 
        CanPacket->InProgress = true;                                               // Ставим флаг обработки пакета
        ControllerAreaNetwork::CanTransferInitiate();                               // Инициируем передачу
    }

    void deletePacketFromQueue(FT_ProtocolDequeStruct *CanPacket)
    {
        list <FT_ProtocolDequeStruct*>::iterator subCanDequeIterator;
        // subCanDequeIterator = find(CanDeque->begin(), CanDeque->end(), CanPacket);
        // if(subCanDequeIterator != CanDeque->end())
        // {
        //     CanDeque->erase(subCanDequeIterator);
        // }
        subCanDequeIterator = find(CanDeque.begin(), CanDeque.end(), CanPacket);
        if(subCanDequeIterator != CanDeque.end())
        {
            CanDeque.erase(subCanDequeIterator);
        }
    }
}


uint32_t FT_receive_handler(uint8_t *buffer, uint32_t len)
{
    //FT_ProtocolPackageStruct CanTxPackage;
    FT_ProtocolDequeStruct *CanPacket = new FT_ProtocolDequeStruct;;

        if ((buffer[0] != 0x10) && (buffer[0] != 0x68))
        {
            len = 0;
            return 0;
        }
        if ((buffer[6] == 0x14) && (len > 9)) // Передача во внешний модуль
        {
            packetNumber = buffer[4];
            CanPacket->package.packageNumber = 0;
            CanPacket->package.destinationAddress = buffer[8];
            CanPacket->package.packageLength = buffer[7];
            CanPacket->package.responseTypeSign = 1;
            CanPacket->package.sourceAddress = GeneralSettingsCAN.ID;
            CanPacket->package.data[0] = GeneralSettingsCAN.ID;
            CanPacket->package.data[1] = buffer[9];  // Код команды 
            CanPacket->package.data[2] = buffer[10];
            CanPacket->package.data[3] = buffer[11];
            CanPacket->package.data[4] = buffer[12];
            CanPacket->package.data[5] = buffer[13];
            CanPacket->package.data[6] = buffer[14];
            CanPacket->package.data[7] = buffer[15];
            CanPacket->InProgress = false;
            CanPacket->FunctionalCode = 0x02; 
            CanPacket->FTCommandCode = 0x14;
            CanPacket->faultCounterLimit = 100;
            CanPacket->faultCounter = 0;
            CanPacket->resetFlag = false;
            ControllerAreaNetwork::addPacketToQueue(CanPacket);

            return 0;
        }
        else if((buffer[6] == 0x15) && (len > 9)) // Чтение индексного параметра
        {
            return 0;
        } 
        else if((buffer[6] == 0x16) && (len > 9)) // Запись индексного параметра
        {
            return 0;
        }
        else if((buffer[6] == 0x18) && (len > 9)) // Очистка индексного параметра
        {
            return 0;
        }
        else if((buffer[6] == 0x19) && (len > 9)) // Чтение индексного параметра из внешнего модуля
        {
            packetNumber = buffer[4];
            CanPacket->package.packageNumber = 0;
            CanPacket->package.destinationAddress = buffer[7];          // ИД кому
            CanPacket->package.packageLength = 6;
            CanPacket->package.responseTypeSign = 1;
            CanPacket->package.sourceAddress = GeneralSettingsCAN.ID;  // Мой ИД
            CanPacket->package.data[0] = GeneralSettingsCAN.ID;        // Мой Ид
            CanPacket->package.data[1] = buffer[9];    // Код команды 
            CanPacket->package.data[2] = buffer[10];   // Номер парметра
            CanPacket->package.data[3] = buffer[11];   // Тип параметра
            CanPacket->package.data[4] = buffer[12];   // Младший байт значения индекса
            CanPacket->package.data[5] = buffer[13];   // Старший байт значения индекса
            CanPacket->package.data[6] = buffer[14];   // No use 
            CanPacket->package.data[7] = buffer[15];   // No use
            CanPacket->InProgress = false;
            CanPacket->FunctionalCode = 0x02; 
            CanPacket->FTCommandCode = 0x19;
            CanPacket->faultCounterLimit = 100;
            CanPacket->faultCounter = 0;
            CanPacket->resetFlag = false;
            ControllerAreaNetwork::addPacketToQueue(CanPacket);
            return 0;
        }
        else if((buffer[6] == 0x1C) && (len > 9)) // Чтение списка параметров внешних модулей
        {
            return 0;
        }
        else if ((buffer[6] == 0x15) && (len > 9)) // Чтение индексного параметра
        {
            ReadMassiveParam(buffer, &len);
        }
        else if((buffer[3] == 0x01) && (len == 9))
        {
            ReadParam(buffer, &len);

            return 0;
        }
        else if ((buffer[6] == 0x17) && (len == 9)) // Установка уровня доступа
        {
            AccessLevel(buffer, &len);
            return 0;
        }
        else if ((buffer[3] == 0x11) && (len == 9)) // Чтение параметра из внешнего модуля
        {

            packetNumber = buffer[1];
            CanPacket->package.packageNumber = 0;
            CanPacket->package.destinationAddress = buffer[4];
            CanPacket->package.packageLength = 4;
            CanPacket->package.responseTypeSign = 1;
            CanPacket->package.sourceAddress = GeneralSettingsCAN.ID;
            CanPacket->package.data[0] = GeneralSettingsCAN.ID;
            CanPacket->package.data[1] = 0x02;
            CanPacket->package.data[2] = buffer[5];
            CanPacket->package.data[3] = buffer[6];
            CanPacket->InProgress = false;
            CanPacket->FunctionalCode = 0x02;
            CanPacket->FTCommandCode = 0x11;
            CanPacket->faultCounterLimit = 100;
            CanPacket->faultCounter = 0;
            CanPacket->resetFlag = false;
            ControllerAreaNetwork::addPacketToQueue(CanPacket);

            return 0;
        }
}


static uint32_t ReadParam(uint8_t *data, uint32_t *len)
{
    //int8_t (*responseHandler)(uint8_t*, uint16_t);
    //responseHandler = (int8_t(*)(uint8_t*, uint16_t))handler;
    uint16_t ParamNumber = ((data[5]) << 8) | data[4];
    uint16_t responsePackerSize = 9;
    ParamDescript   Param = ParametersSystem.getParameter(ParamNumber);
    if((Param.ParametersAddress == 0)) //(Param.Description.readProtectionType > CurrentState.currentAccessLevel)
    {
        responsePacket[0] = 0xE5;
        //RS232.sendBuf(responsePacket, 1);
        //responseHandler(responsePacket, 1);
        return 1;
    }
    responsePacket[0] = 0x10;
    responsePacket[1] = data[1] & 0x0F;
    responsePacket[2] = data[2];
    uint32_t size  = Param.Description.parameterSize;
    if(Param.Description.bitTag == 1)
    {
        responsePacket[3] = ((*(uint8_t*)Param.ParametersAddress) >> Param.Description.parameterSize) & 0x1;
    }else
    {
        for(uint32_t i = 0; i < 4; i++)
        {
            if(i < size)
            {
                responsePacket[i + 3] = *(uint8_t*)((uint32_t)(Param.ParametersAddress) + i);
            }else
            {
                responsePacket[i + 3] = 0x0;
            }
        }
    }
    responsePacket[7] = (responsePacket[1] + responsePacket[2] + responsePacket[3] + responsePacket[4] + responsePacket[5] + responsePacket[6]) & 0xFF;
    responsePacket[8] = 0x16;
    //responseHandler(responsePacket, responsePackerSize);
    //RS232.sendBuf(responsePacket, responsePackerSize);
    send(0, responsePacket, responsePackerSize);
	return 0;
}


static uint32_t ReadMassiveParam(uint8_t *data, uint32_t *len)
{
    //int8_t (*responseHandler)(uint8_t*, uint16_t);
    //responseHandler = (int8_t(*)(uint8_t*, uint16_t))handler;
    uint16_t ParamNumber = ((data[8]) << 8) | data[7];
    uint16_t responsePackerSize = 9;
    uint32_t index = ((data[10]) << 8) | data[9];
    ParamDescript   Param = ParametersSystem.getMassiveParameter(ParamNumber, index);//getParameter(ParamNumber);
    if((Param.ParametersAddress == 0) ) //|| (Param.Description.readProtectionType > CurrentState.currentAccessLevel)
    {
        responsePacket[0] = 0xE5;
        //responseHandler(responsePacket, 1);
        //RS232.sendBuf(responsePacket, 1);
        return 1;
    }
    responsePacket[0] = 0x10;
    responsePacket[1] = data[4] & 0x0F;
    responsePacket[2] = data[5];
    uint32_t size  = Param.Description.parameterSize;
    if(Param.Description.bitTag == 1)
    {
        responsePacket[3] = ((*(uint8_t*)Param.ParametersAddress) >> Param.Description.parameterSize) & 0x1;
    }else
    {
        for(uint32_t i = 0; i < 4; i++)
        {
            if(i < size)
            {
                responsePacket[i + 3] = *(uint8_t*)((uint32_t)(Param.ParametersAddress) + i);
            }else
            {
                responsePacket[i + 3] = 0x0;
            }
        }
    }
    responsePacket[7] = (responsePacket[1] + responsePacket[2] + responsePacket[3] + responsePacket[4] + responsePacket[5] + responsePacket[6]) & 0xFF;
    responsePacket[8] = 0x16;
    //responseHandler(responsePacket, responsePackerSize);
    send(0, responsePacket, responsePackerSize);
	return 0;
}


static uint32_t AccessLevel(uint8_t *data, uint32_t *len)
{  
	volatile uint32_t Password2 = *(uint32_t*)ParametersSystem.getParameter(0xF025).ParametersAddress;
    volatile uint32_t Password3 = *(uint32_t*)ParametersSystem.getParameter(0xF026).ParametersAddress;
    uint32_t receievedPassword = 0;
    uint8_t currentLevel = 0;
    for(int i = 0; i < 4; i++)
    {
        receievedPassword |= data[i + 8] << (i * 8);
    }
    //int8_t (*responseHandler)(uint8_t*, uint16_t);
    //responseHandler = (int8_t(*)(uint8_t*, uint16_t))handler;
	switch(data[7])
	{
		case 0x01:
		{
            currentLevel = 0x1;
			break;
		}
		case 0x02:
		{
			if(*len == 10)
			{
				if((Password2 == 0xFFFFFFFF) || (Password2 == 0x00000000))
				{
					currentLevel = 0x2;
				}else
				{
					currentLevel = 0x1;
				}
			}else if(*len == 14)
			{
				if(Password2 == receievedPassword)
				{
                    currentLevel = 0x2;
				}else
				{
                    currentLevel = 0x1;
				}
			}
			break;
		}
		case 0x03:
		{
            if(*len == 10)
			{
				if(Password3 == 0)
				{
                    currentLevel = 0x3;
				}else
				{
                    currentLevel = 0x1;
				}
			}
            if(*len == 14)
			{
				if(Password3 == receievedPassword)
				{
                    currentLevel = 0x3;
				}else
				{
                    currentLevel = 0x1;
				}
			}
			break;
		}
        case 0x30:
		{
            if(*len == 14)
			{
				if(receievedPassword == 0xF026F025)
				{
                    currentLevel = 0x3;
				}else
				{
                    currentLevel = 0x1;
				}
			}

			break;
		}
		default:
		{

			break;
		}
	}
    CurrentState.currentAccessLevel = currentLevel;
    responsePacket[0] = 0x68;
    responsePacket[1] = 0x3;
    responsePacket[2] = 0x3;
    responsePacket[3] = 0x68;
    responsePacket[4] = data[4] & 0x0F;
    responsePacket[5] = data[5];
    responsePacket[6] = currentLevel;
    responsePacket[7] = (responsePacket[4] + responsePacket[5] + responsePacket[6]) & 0xFF;
    responsePacket[8] = 0x16;
    uint16_t responsePackerSize = 9;

    send(0, responsePacket, responsePackerSize);

	return 0;
}
