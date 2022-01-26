#ifndef FT_CAN_HPP
#define FT_CAN_HPP


#ifdef __cplusplus
extern "C"{
#endif

//Include C libs//

#include "MDR32Fx.h"
#include "MDR32F9Qx_can.h"

#ifdef __cplusplus
}
#endif

#include "can.hpp"

typedef struct
{
    FT_ProtocolPackageStruct package;
    uint16_t  paramNumber;
    uint8_t   FunctionalCode;
    uint8_t   faultCounterLimit;
    uint8_t   faultCounter;
    uint8_t   FTCommandCode;
    bool resetFlag;
    bool InProgress;
    bool Fault;
}FT_ProtocolDequeStruct;

void initFtContainers();

namespace ControllerAreaNetwork
{
    void nope();
    void pushBackToQueue(FT_ProtocolDequeStruct *CanPacket);            // Добавить пакет в конец очереди
    FT_ProtocolDequeStruct* getFrontElementFromQueue();                 // Получить первый элемент очереди     
    void CanTransferInitiate();                                         // Инициировать передачу по CAN
    void addPacketToQueue(FT_ProtocolDequeStruct *CanPacket);           // Добавить пакет в очередь и инициировать передачу
    void canTimeoutHandler();                                           // Вызов по таймеру
    void deletePacketFromQueue(FT_ProtocolDequeStruct *CanPacket);      // Найти и удалить элемент из очереди
    void updateFrontQueuePacket(FT_ProtocolDequeStruct *CanPacket, bool packetFaultFlag);   // Обновить состояние первого элемента очереди
    uint32_t CAN_GetDequeSize();
    //=========================== Request function ===========================//
    bool CAN_ErrorRequestFunctionCode(FT_ProtocolDequeStruct *CanPacket);   //<! CAN Error function code handler
    bool CAN_SendRequest_ReadFloatParam(FT_ProtocolDequeStruct *CanPacket);
    

    //========================== Response functions ==========================//
    bool CAN_ErrorResponseFunctionCode(FT_ProtocolDequeStruct *CanPacket, FT_ProtocolPackageStruct *receivePacket);   //<! CAN Error function code handler
    bool CAN_ReceiveResponse_ReadFloatParam(FT_ProtocolDequeStruct *CanPacket, FT_ProtocolPackageStruct *receivePacket);
}

static bool (*CanRequestTable[])(FT_ProtocolDequeStruct *CanPacket)  __attribute__((used))
{
   &ControllerAreaNetwork::CAN_ErrorRequestFunctionCode,      // 0x00
   &ControllerAreaNetwork::CAN_ErrorRequestFunctionCode,      // 0x01
   &ControllerAreaNetwork::CAN_SendRequest_ReadFloatParam,    // 0x02   // Отправить запрос
   &ControllerAreaNetwork::CAN_ErrorRequestFunctionCode,      // 0x03
};


static bool (*CanResponseTable[])(FT_ProtocolDequeStruct *CanPacket, FT_ProtocolPackageStruct *receivePacket)  __attribute__((used))
{
    &ControllerAreaNetwork::CAN_ErrorResponseFunctionCode,          // 0x00
    &ControllerAreaNetwork::CAN_ErrorResponseFunctionCode,          // 0x01
    &ControllerAreaNetwork::CAN_ReceiveResponse_ReadFloatParam      // 0x02 // Получить ответ
};



uint32_t FT_receive_handler(uint8_t *recieveBuf, uint32_t len);












#endif