#ifndef PARAMSYS_H
#define PARAMSYS_H

#include "MDR32Fx.h"

#define DEVICE_CONST_LENGHT  				4096


#define RAM				0x0
#define ROM				0x1
#define INTERNAL	    0x0
#define EXTERNAL	    0x1
#define DATA			0x0
#define PROGRAM		    0x1
#define BYTE			0x0
#define BIT				0x1
#define SINGLE		    0x0
#define MASSIVE		    0x1
#define	LENGHT_1	    0x1
#define	LENGHT_2	    0x2
#define	LENGHT_4	    0x4
#define RD_0			0x0
#define RD_1			0x1
#define RD_2			0x2
#define RD_3			0x3
#define WR_0			0x0
#define WR_1			0x1
#define WR_2			0x2
#define WR_3			0x3
#define RES_WINDOW      0x0



struct DescriptionStruct{
    unsigned memoryTypeTag:       1;
    unsigned memoryLocationTag:   1;
    unsigned parameterTypeTag:    1;
    unsigned bitTag:              1;
    unsigned massiveTag:          1;
    unsigned parameterSize:       3;
    unsigned RESERVE:             4;
    unsigned readProtectionType:  2;
    unsigned writeProtectionType: 2;
};


struct ParamDescript{
    uint32_t *ParametersAddress;
    struct DescriptionStruct Description;
};

#endif