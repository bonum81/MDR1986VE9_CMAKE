#ifndef CAN_H
#define CAN_H

#ifdef __cplusplus
extern "C"{
#endif

#include "MDR32Fx.h"

#ifdef __cplusplus
}
#endif

typedef struct 
{
    uint8_t destinationAddress;
    uint8_t sourceAddress;
    uint8_t packageNumber;
    uint8_t packageLength;
    uint8_t responseTypeSign;
    uint8_t data[8];
}FT_ProtocolPackageStruct;

#endif