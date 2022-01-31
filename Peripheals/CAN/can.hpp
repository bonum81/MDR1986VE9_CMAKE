#ifndef CAN_HPP
#define CAN_HPP

#ifdef __cplusplus
extern "C"{
#endif

#include "MDR32Fx.h"
#include "MDR32F9Qx_can.h"


#ifdef __cplusplus
}
#endif

typedef struct 
{
    uint8_t  ID = 0x00;
    uint16_t baudRate = 0x0000;
}GenSetCAN;

typedef struct 
{
    uint8_t destinationAddress;
    uint8_t sourceAddress;
    uint8_t packageNumber;
    uint8_t packageLength;
    uint8_t responseTypeSign;
    uint8_t data[8];
}FT_ProtocolPackageStruct;

void canSettingsHandler();
void CAN_SendMsg(FT_ProtocolPackageStruct *package);
void init_GPIO_CAN(void);
extern GenSetCAN GeneralSettingsCAN;

#endif
