#include "MDR32Fx.h"
#include "MDR32Fx.h"
#include "MDR32F9Qx_eeprom.h"
#include <FreeRTOS.h>
#include <task.h>
#include "ParamSys.hpp"


static void vCopyParam_RAMtoFLASH_0_Task( void *pvParameters)
{
    uint32_t destinationAddress = 0;
	uint8_t *sourceAddress = 0;
    ParamDescript paramater;

    EEPROM_SetLatency(EEPROM_Latency_1);
    EEPROM_ErasePage(DEVICE_PARAM_FLASH_PAGE_30, EEPROM_Main_Bank_Select);
	EEPROM_ErasePage(DEVICE_PARAM_FLASH_PAGE_31, EEPROM_Main_Bank_Select);

    for(int paramType = 0; paramType <= LAST_TYPE_PARAMETERS; paramType++)
	{
        
    }
}