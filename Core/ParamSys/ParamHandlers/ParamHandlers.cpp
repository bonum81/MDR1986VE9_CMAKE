#include "ParamHandlers.hpp"

void NULL_HANDLER()
{
    return;
}

void (*paramHandlersTable_0x00[])(void) =                                                                        //<! Таблица параметров группы 0x00xx 
{
    /*0000*/    &NULL_HANDLER,
    /*0001*/    &NULL_HANDLER,
    /*0002*/    &NULL_HANDLER,
    /*0003*/    &NULL_HANDLER,
    /*0004*/    &NULL_HANDLER,
    /*0005*/    &NULL_HANDLER,
    /*0006*/    &NULL_HANDLER,
    /*0007*/    &NULL_HANDLER,
    /*0008*/    &NULL_HANDLER,
    /*0009*/    &NULL_HANDLER,
    /*000A*/    &NULL_HANDLER,
    /*000B*/    &NULL_HANDLER,
    /*000C*/    &NULL_HANDLER,
    /*000D*/    &NULL_HANDLER,
    /*000E*/    &NULL_HANDLER,
    /*000F*/    &NULL_HANDLER,
    /*0010*/    &NULL_HANDLER,
    /*0011*/    &NULL_HANDLER,
    /*0012*/    &NULL_HANDLER,
    /*0013*/    &NULL_HANDLER,
    /*0014*/    &NULL_HANDLER,
    /*0015*/    &NULL_HANDLER,
    /*0016*/    &NULL_HANDLER,
    /*0017*/    &NULL_HANDLER,
    /*0018*/    &NULL_HANDLER,
    /*0019*/    &NULL_HANDLER,
    /*001A*/    &NULL_HANDLER    
};

void (*paramHandlersTable_0x01[])(void) =                                                                       //<! Таблица параметров группы 0x01xx 
{
    /*0100*/    &NULL_HANDLER
};

void (*paramHandlersTable_0x02[])(void) =                                                                       //<! Таблица параметров группы 0x02xx 
{
    /*0200*/    &NULL_HANDLER

};

void (*paramHandlersTable_0x03[])(void) =                                                                       //<! Таблица параметров группы 0x03xx 
{

};

void (*paramHandlersTable_0x04[])(void) =                                                                        //<! Таблица параметров группы 0x04xx 
{
    /*0400*/    &NULL_HANDLER

};

void (*paramHandlersTable_0x05[])(void) =                                                                        //<! Таблица параметров группы 0x05xx 
{

};

void (*paramHandlersTable_0x06[])(void) =                                                                       //<! Таблица параметров группы 0x06xx 
{
    &NULL_HANDLER
};

void (*paramHandlersTable_0x07[])(void) =                                                                        //<! Таблица параметров группы 0x07xx 
{
    &NULL_HANDLER,
    &NULL_HANDLER,
    &NULL_HANDLER,
    &NULL_HANDLER,
    &NULL_HANDLER,
};

void (*paramHandlersTable_0x08[])(void) =                                                                        //<! Таблица параметров группы 0x08xx 
{

};

void (*paramHandlersTable_0x09[])(void) =                                                                       //<! Таблица параметров группы 0x09xx 
{

};

void (*paramHandlersTable_0xF0[])(void) =                                                                         //<! Таблица параметров группы 0xF0xx
{
    &NULL_HANDLER,
    &NULL_HANDLER,
    &NULL_HANDLER,
    &NULL_HANDLER,
    &NULL_HANDLER,
    &NULL_HANDLER,
    &NULL_HANDLER,
    &NULL_HANDLER,
    &NULL_HANDLER,
    &NULL_HANDLER,
    &NULL_HANDLER,
    &NULL_HANDLER,
    &NULL_HANDLER,
    &NULL_HANDLER,
    &NULL_HANDLER,
    &NULL_HANDLER,
    &NULL_HANDLER,
    &NULL_HANDLER,
    &NULL_HANDLER,
    &NULL_HANDLER,
    &NULL_HANDLER,
    &NULL_HANDLER,
    &NULL_HANDLER,
    &NULL_HANDLER,
    &NULL_HANDLER,
    &NULL_HANDLER,
    &NULL_HANDLER,
};

void (**generalGroupTable[])(void)                //<! Сводный массив указателей на таблицы параметров по группам 
{
    &paramHandlersTable_0x00[0],
    &paramHandlersTable_0x01[0],
    &paramHandlersTable_0x02[0],
    &paramHandlersTable_0x03[0],
    &paramHandlersTable_0x04[0],
    &paramHandlersTable_0x05[0],
    &paramHandlersTable_0x06[0],
    &paramHandlersTable_0x07[0],
    &paramHandlersTable_0x08[0],
    &paramHandlersTable_0x09[0],
    &paramHandlersTable_0xF0[0]
};

const int ParametersTablesLenght[] =		                    //<! Длины каждого типа (каждой группы) жестких параметров 
{
	(sizeof(paramHandlersTable_0x00) / sizeof(uint32_t*)),
	(sizeof(paramHandlersTable_0x01) / sizeof(uint32_t*)),
	(sizeof(paramHandlersTable_0x02) / sizeof(uint32_t*)),
	(sizeof(paramHandlersTable_0x03) / sizeof(uint32_t*)),
	(sizeof(paramHandlersTable_0x04) / sizeof(uint32_t*)),
	(sizeof(paramHandlersTable_0x05) / sizeof(uint32_t*)),
	(sizeof(paramHandlersTable_0x06) / sizeof(uint32_t*)),
	(sizeof(paramHandlersTable_0x07) / sizeof(uint32_t*)),
	(sizeof(paramHandlersTable_0x08) / sizeof(uint32_t*)),
	(sizeof(paramHandlersTable_0x09) / sizeof(uint32_t*)),
	(sizeof(paramHandlersTable_0xF0) / sizeof(uint32_t*))
};


void ParamHandlers::doParameterHandler(uint16_t param)
{
    uint8_t paramType = param >> 8;
    uint8_t paramNumber = param & 0xFF;
    if(paramType == 0xF0) 
        paramType = 0x0A;
    if((paramType <= 0x0A))
    {
		if(ParametersTablesLenght[paramType] <= paramNumber)
		{
            return;
		}
        (*generalGroupTable[paramType][paramNumber])();
    }
}