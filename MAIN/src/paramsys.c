#include "paramsys.h"

typedef struct 
{
	uint32_t param;
}empty_initialized_UINT32_param;

typedef struct 
{
	uint16_t param;
}empty_initialized_UINT16_param;

typedef struct 
{
	uint8_t param;
}empty_initialized_UINT8_param;

uint32_t NULL_PARAM = 0;

const empty_initialized_UINT8_param DeviceConstParameters [DEVICE_CONST_LENGHT] __attribute__((section(".deviceParameters"))); //__attribute__((section(".deviceParameters"))) __attribute__((used)) 


const struct ParamDescript ParamDescrTable_0x00[] =                                                                        //<! Таблица параметров группы 0x00xx 
{
    /*0000*/	{(uint32_t*)& NULL_PARAM,        	  					{ROM, INTERNAL, DATA, BYTE, SINGLE, 	LENGHT_1, RES_WINDOW, RD_1, WR_2}},		//CAN ID
	/*0001*/	{(uint32_t*)& NULL_PARAM,        				  		{RAM, INTERNAL, DATA, BYTE, SINGLE, 	LENGHT_1, RES_WINDOW, RD_0, WR_0}},		//-
	/*0002*/	{(uint32_t*)& NULL_PARAM,        				  		{RAM, INTERNAL, DATA, BYTE, SINGLE, 	LENGHT_1, RES_WINDOW, RD_0, WR_0}},		//-
	/*0003*/	{(uint32_t*)& NULL_PARAM,       				  		{RAM, INTERNAL, DATA, BYTE, SINGLE, 	LENGHT_1, RES_WINDOW, RD_0, WR_0}},		//-
	/*0004*/	{(uint32_t*)& NULL_PARAM,  	  					        {ROM, INTERNAL, DATA, BYTE, SINGLE, 	LENGHT_2, RES_WINDOW, RD_1, WR_2}},		//CAN Speed
	/*0005*/	{(uint32_t*)& NULL_PARAM,  	  						    {ROM, INTERNAL, DATA, BIT,  SINGLE, 		   0, RES_WINDOW, RD_1, WR_2}},		//-
	/*0006*/	{(uint32_t*)& NULL_PARAM,  								{ROM, INTERNAL, DATA, BYTE, SINGLE, 	LENGHT_1, RES_WINDOW, RD_1, WR_2}},		//RS232 SPEED
	/*0007*/	{(uint32_t*)& NULL_PARAM,   							{ROM, INTERNAL, DATA, BYTE, SINGLE, 	LENGHT_2, RES_WINDOW, RD_1, WR_2}},		//RS232 Parity
	/*0008*/	{(uint32_t*)& NULL_PARAM,   							{ROM, INTERNAL, DATA, BYTE, MASSIVE, 	LENGHT_1, RES_WINDOW, RD_1, WR_2}},		
	/*0009*/	{(uint32_t*)& NULL_PARAM,   							{ROM, INTERNAL, DATA, BYTE, SINGLE, 	LENGHT_4, RES_WINDOW, RD_1, WR_2}},
	/*000A*/	{(uint32_t*)& NULL_PARAM,   							{ROM, INTERNAL, DATA, BYTE, SINGLE, 	LENGHT_4, RES_WINDOW, RD_1, WR_2}},
	/*000B*/	{(uint32_t*)& NULL_PARAM,   							{ROM, INTERNAL, DATA, BYTE, SINGLE, 	LENGHT_4, RES_WINDOW, RD_1, WR_2}},
	/*000C*/	{(uint32_t*)& NULL_PARAM,   						    {ROM, INTERNAL, DATA, BYTE, SINGLE, 	LENGHT_2, RES_WINDOW, RD_1, WR_2}},
	/*000D*/	{(uint32_t*)& NULL_PARAM,   						    {ROM, INTERNAL, DATA, BYTE, SINGLE, 	LENGHT_2, RES_WINDOW, RD_1, WR_2}},
	/*000E*/	{(uint32_t*)& NULL_PARAM,   					        {ROM, INTERNAL, DATA, BYTE, SINGLE, 	LENGHT_2, RES_WINDOW, RD_1, WR_2}},
	/*000F*/	{(uint32_t*)& NULL_PARAM,   		                    {ROM, INTERNAL, DATA, BYTE, SINGLE, 	LENGHT_2, RES_WINDOW, RD_1, WR_2}},
	/*0010*/	{(uint32_t*)& NULL_PARAM,  	                            {ROM, INTERNAL, DATA, BYTE, SINGLE, 	LENGHT_2, RES_WINDOW, RD_1, WR_2}},
	/*0011*/	{(uint32_t*)& NULL_PARAM,       				  		{RAM, INTERNAL, DATA, BYTE, SINGLE, 	LENGHT_1, RES_WINDOW, RD_0, WR_0}},		//-
	/*0012*/	{(uint32_t*)& NULL_PARAM,   		                    {ROM, INTERNAL, DATA, BYTE, SINGLE, 	LENGHT_2, RES_WINDOW, RD_1, WR_2}},
	/*0013*/	{(uint32_t*)& NULL_PARAM,       				  		{RAM, INTERNAL, DATA, BYTE, SINGLE, 	LENGHT_1, RES_WINDOW, RD_0, WR_0}},		//-
	/*0014*/	{(uint32_t*)& NULL_PARAM,   				            {ROM, INTERNAL, DATA, BIT,  SINGLE, 		   0, RES_WINDOW, RD_1, WR_2}},
	/*0015*/	{(uint32_t*)& NULL_PARAM,   					        {ROM, INTERNAL, DATA, BYTE, SINGLE, 	LENGHT_1, RES_WINDOW, RD_1, WR_2}},
	/*0016*/	{(uint32_t*)& NULL_PARAM,       				  		{RAM, INTERNAL, DATA, BYTE, SINGLE, 	LENGHT_1, RES_WINDOW, RD_0, WR_0}},
	/*0017*/	{(uint32_t*)& NULL_PARAM,       				  		{RAM, INTERNAL, DATA, BYTE, SINGLE, 	LENGHT_1, RES_WINDOW, RD_0, WR_0}},
	/*0018*/	{(uint32_t*)& NULL_PARAM,       				        {RAM, INTERNAL, DATA, BYTE, SINGLE, 	LENGHT_1, RES_WINDOW, RD_0, WR_0}},
	/*0019*/	{(uint32_t*)& NULL_PARAM,       						{ROM, INTERNAL, DATA, BIT,  SINGLE, 	LENGHT_1, RES_WINDOW, RD_1, WR_2}},
	/*001A*/	{(uint32_t*)& NULL_PARAM,       						{ROM, INTERNAL, DATA, BIT,  SINGLE, 	LENGHT_1, RES_WINDOW, RD_1, WR_2}},
};
