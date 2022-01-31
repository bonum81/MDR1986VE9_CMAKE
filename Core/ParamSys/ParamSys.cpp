#include "ParamSys.hpp"
#include "init.hpp"
#include "ParamHandlers.hpp"
#if defined(__cplusplus)
extern "C"{
#endif  
#include "MDR32F9Qx_eeprom.h"

#if defined(__cplusplus)
}
#endif

ParamSys& ParametersSystem = ParamSys::getInstance();
ParamHandlers& ParameterHandlers = ParamHandlers::getInstance();
CurState CurrentState;
settigns_general GeneralSettingsK104M;

static uint32_t K104_IpAddress    					= 192168055;
static uint32_t K104_IpMask   						= 2552552550;
static uint32_t K104_IpGateway 					    = 19216801;
static 	uint8_t K104_MacAddress[6] 					= {0x8A,0xA4,0xBE,0x2E,0x7D,0xA4};


typedef struct 
{
	uint32_t param = 0xFFFFFFFF;
}empty_initialized_UINT32_param;
typedef struct 
{
	uint16_t param = 0xFFFF;
}empty_initialized_UINT16_param;
typedef struct 
{
	uint8_t param = 0xFF;
}empty_initialized_UINT8_param;

uint32_t NULL_PARAM = 0;

struct 
{
	uint16_t DeviceType 	= 0x1105;
	uint16_t FactoryNumber  = 0x0001;
	uint16_t ProgramVersion = FIRMWARE_VER;
	uint16_t SerialNumber   = 0x0000;
	uint8_t  AppProgramName [24];
	uint8_t  factoryProjName[24];
	uint16_t screenPassword = 0;
}GeneralSystemParameters_0xF0;

struct 
{
	uint32_t param_0xF005 = 0;
	uint32_t param_0xF006 = 0;
	uint32_t param_0xF007 = 0;
	uint32_t param_0xF008 = 0;
	uint32_t param_0xF009 = 0;
	uint32_t param_0xF010 = 0;
	uint32_t param_0xF021 = 0;
	uint32_t param_0xF01D = 0x1080;
}kostil;


struct 
{
	bool  FreqInput0 		= false;
	bool  FreqInput1 		= false;
	uint16_t IntegralInput0 	= 0;
	uint16_t IntegralInput1 	= 0;
}FrequencyMeasurements;

struct 
{
	bool  EnableFiltering 	  = false;
	uint32_t AllowedIP_1      = 192168056;
	uint32_t AllowedIP_2      = 192168057;
	uint32_t AllowedIP_3      = 192168058;
	uint32_t AllowedIP_4      = 192168059;
	uint32_t AllowedIP_5   	  = 192168060;
	uint32_t AllowedIP_6   	  = 192168061;
	uint32_t AllowedIP_7   	  = 192168062;
	uint32_t AllowedIP_8      = 192168063;
	uint32_t AllowedIP_9   	  = 192168064;
	uint32_t AllowedIP_10     = 192168065;
}FilteringInputIP;




struct _TYPE_AppProgramData
{
	ApplicationProgramTask  		ApplicationProgramInstructionList[FUNC_LIST_LENGHT];        							//<! Очередь задач / Инструкции прикладной программы
	empty_initialized_UINT16_param  FlexibleParametersTable[FLEX_PARAM_TABLE_LENGHT]; 		 								//<! Массив параметров функций
	empty_initialized_UINT32_param  FlexibleParametersDescriptorTable[FLEX_PARAM_DESCR_TABLE_LENGHT]; 						//<! Массив описателей параметров
	empty_initialized_UINT32_param  ApplicationProgramIdentificator[4] = {0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF};	//<! Идентификатор прикладной программы
	//empty_initialized_UINT8_param  AppName[256];																			//<! Идентификатор прикладной программы
	uint32_t AppProgFuncListLenght 	= FUNC_LIST_LENGHT; 								   									//<! Размер массива очереди
	uint32_t FlexParamTableLenght  	= FLEX_PARAM_TABLE_LENGHT;								  								//<! Размер массива параметров
	uint32_t FlexParamDescrTableLenght = FLEX_PARAM_DESCR_TABLE_LENGHT; 							   						//<! Размер массива описателей параметров
	uint32_t appProgramCRC32 = 0;																							//<! Контрольная сумма по CRC32
};


struct _TYPE_AppProgParamRAM_Data
{
	empty_initialized_UINT8_param applicationProgramRAMBank1[1776];
};

struct _TYPE_AppProgParamFlashData
{
	empty_initialized_UINT8_param  applicationProgramFlashBank1[1776];
	empty_initialized_UINT32_param BankCRC32;
};

const uint16_t AppProgramParameters[] = {0x0704, 0x070A, 0x070B, 0x070C, 0x070E, 0x070D, 0x0728, 0x0729}; //<! Параметры относящиеся к прикладной программе (0xF0 заменён на 0x0A)


/*==================================================================================================================================================*/
// FLASH_31	-  4 kB	
const _TYPE_AppProgramData AppProgramFlashData									__attribute__((section(".applicationProgram"))) __attribute__((used));
/*<!  */
	  _TYPE_AppProgramData AppProgramFlashData_RAM_BUFFER 						; 

/*===================================================================================================================================================*/	  
/*<! */
// FLASH_30 - 4 kB
const _TYPE_AppProgParamFlashData AppProgramParametersFlashData 				__attribute__((section(".appProgramParameters"))) __attribute__((used));//__attribute__((section(".appProgramParameters"))) __attribute__((used));
/*<!  */
	  _TYPE_AppProgParamFlashData AppProgramParametersFlashData_RAM_BUFFER		;

/*=================================================================================================================================================*/
_TYPE_AppProgParamRAM_Data  AppProgramParametersRAM_Data  					;


/*=================================================================================================================================================*/

//const uint8_t DeviceConstParameters [DEVICE_CONST_LENGHT] __attribute__((used, section(".deviceParameters"))) = {}; //__attribute__((section(".deviceParameters"))) __attribute__((used));
const empty_initialized_UINT8_param DeviceConstParameters [DEVICE_CONST_LENGHT] __attribute__((section(".deviceParameters"))) __attribute__((used)) ; //__attribute__((section(".deviceParameters"))) __attribute__((used));
/*=================================================================================================================================================*/


const ParamDescript ParamDescrTable_0x00[] =                                                                        //<! Таблица параметров группы 0x00xx 
{
    /*0000*/	{(uint32_t*)& NULL_PARAM,        	  								{ROM, INTERNAL, DATA, BYTE, SINGLE, 	LENGHT_1, RES_WINDOW, RD_1, WR_2}},		//CAN ID
	/*0001*/	{(uint32_t*)& NULL_PARAM,        				  					{RAM, INTERNAL, DATA, BYTE, SINGLE, 	LENGHT_1, RES_WINDOW, RD_0, WR_0}},		//-
	/*0002*/	{(uint32_t*)& NULL_PARAM,        				  					{RAM, INTERNAL, DATA, BYTE, SINGLE, 	LENGHT_1, RES_WINDOW, RD_0, WR_0}},		//-
	/*0003*/	{(uint32_t*)& NULL_PARAM,       				  					{RAM, INTERNAL, DATA, BYTE, SINGLE, 	LENGHT_1, RES_WINDOW, RD_0, WR_0}},		//-
	/*0004*/	{(uint32_t*)& NULL_PARAM,  	  										{ROM, INTERNAL, DATA, BYTE, SINGLE, 	LENGHT_2, RES_WINDOW, RD_1, WR_2}},		//CAN Speed
	/*0005*/	{(uint32_t*)& NULL_PARAM,  	  										{ROM, INTERNAL, DATA, BIT,  SINGLE, 		   0, RES_WINDOW, RD_1, WR_2}},		//-
	/*0006*/	{(uint32_t*)& NULL_PARAM,  											{ROM, INTERNAL, DATA, BYTE, SINGLE, 	LENGHT_1, RES_WINDOW, RD_1, WR_2}},		//RS232 SPEED
	/*0007*/	{(uint32_t*)& NULL_PARAM,   										{ROM, INTERNAL, DATA, BYTE, SINGLE, 	LENGHT_2, RES_WINDOW, RD_1, WR_2}},		//RS232 Parity
	/*0008*/	{(uint32_t*)& K104_MacAddress,   									{ROM, INTERNAL, DATA, BYTE, MASSIVE, 	LENGHT_1, RES_WINDOW, RD_1, WR_2}},		
	/*0009*/	{(uint32_t*)& K104_IpAddress,   									{ROM, INTERNAL, DATA, BYTE, SINGLE, 	LENGHT_4, RES_WINDOW, RD_1, WR_2}},
	/*000A*/	{(uint32_t*)& K104_IpGateway,   									{ROM, INTERNAL, DATA, BYTE, SINGLE, 	LENGHT_4, RES_WINDOW, RD_1, WR_2}},
	/*000B*/	{(uint32_t*)& K104_IpMask,   										{ROM, INTERNAL, DATA, BYTE, SINGLE, 	LENGHT_4, RES_WINDOW, RD_1, WR_2}},
	/*000C*/	{(uint32_t*)& GeneralSettingsK104M.port_RS,   						{ROM, INTERNAL, DATA, BYTE, SINGLE, 	LENGHT_2, RES_WINDOW, RD_1, WR_2}},
	/*000D*/	{(uint32_t*)& GeneralSettingsK104M.port_CAN,   						{ROM, INTERNAL, DATA, BYTE, SINGLE, 	LENGHT_2, RES_WINDOW, RD_1, WR_2}},
	/*000E*/	{(uint32_t*)& GeneralSettingsK104M.port_telnet,   					{ROM, INTERNAL, DATA, BYTE, SINGLE, 	LENGHT_2, RES_WINDOW, RD_1, WR_2}},
	/*000F*/	{(uint32_t*)& GeneralSettingsK104M.waitingTimeNextByteRS232,   		{ROM, INTERNAL, DATA, BYTE, SINGLE, 	LENGHT_2, RES_WINDOW, RD_1, WR_2}},
	/*0010*/	{(uint32_t*)& GeneralSettingsK104M.waitingTimeStartResponseRS232,  	{ROM, INTERNAL, DATA, BYTE, SINGLE, 	LENGHT_2, RES_WINDOW, RD_1, WR_2}},
	/*0011*/	{(uint32_t*)& NULL_PARAM,       				  					{RAM, INTERNAL, DATA, BYTE, SINGLE, 	LENGHT_1, RES_WINDOW, RD_0, WR_0}},		//-
	/*0012*/	{(uint32_t*)& GeneralSettingsK104M.responseWaitingTimeCAN,   		{ROM, INTERNAL, DATA, BYTE, SINGLE, 	LENGHT_2, RES_WINDOW, RD_1, WR_2}},
	/*0013*/	{(uint32_t*)& NULL_PARAM,       				  		 			{RAM, INTERNAL, DATA, BYTE, SINGLE, 	LENGHT_1, RES_WINDOW, RD_0, WR_0}},		//-
	/*0014*/	{(uint32_t*)& GeneralSettingsK104M.EthernetSpeed,   				{ROM, INTERNAL, DATA, BIT,  SINGLE, 		   0, RES_WINDOW, RD_1, WR_2}},
	/*0015*/	{(uint32_t*)& GeneralSettingsK104M.modeTCP_UPD,   					{ROM, INTERNAL, DATA, BYTE, SINGLE, 	LENGHT_1, RES_WINDOW, RD_1, WR_2}},
	/*0016*/	{(uint32_t*)& NULL_PARAM,       				  					{RAM, INTERNAL, DATA, BYTE, SINGLE, 	LENGHT_1, RES_WINDOW, RD_0, WR_0}},
	/*0017*/	{(uint32_t*)& NULL_PARAM,       				  					{RAM, INTERNAL, DATA, BYTE, SINGLE, 	LENGHT_1, RES_WINDOW, RD_0, WR_0}},
	/*0018*/	{(uint32_t*)& GeneralSettingsK104M.typeOfSlave,       				{RAM, INTERNAL, DATA, BYTE, SINGLE, 	LENGHT_1, RES_WINDOW, RD_0, WR_0}},
	/*0019*/	{(uint32_t*)& NULL_PARAM,       						{ROM, INTERNAL, DATA, BIT,  SINGLE, 	LENGHT_1, RES_WINDOW, RD_1, WR_2}},
	/*001A*/	{(uint32_t*)& NULL_PARAM,       						{ROM, INTERNAL, DATA, BIT,  SINGLE, 	LENGHT_1, RES_WINDOW, RD_1, WR_2}},
};

const ParamDescript ParamDescrTable_0x01[] =                                                                        //<! Таблица параметров группы 0x00xx 
{
	/*0001*/	{(uint32_t*)& NULL_PARAM,        				  {RAM, INTERNAL, DATA, BYTE, SINGLE, 	LENGHT_1, RES_WINDOW, RD_0, WR_0}},		//-
};

const ParamDescript ParamDescrTable_0x02[] =                                                                        //<! Таблица параметров группы 0x00xx 
{
	/*0200*/	{(uint32_t*)& NULL_PARAM,        				  {RAM, INTERNAL, DATA, BYTE, SINGLE, 	LENGHT_1, RES_WINDOW, RD_0, WR_0}},		//-
	/*0201*/	{(uint32_t*)& NULL_PARAM,        				  {RAM, INTERNAL, DATA, BYTE, SINGLE, 	LENGHT_1, RES_WINDOW, RD_0, WR_0}},		//-
	/*0202*/	{(uint32_t*)& NULL_PARAM,        				  {RAM, INTERNAL, DATA, BYTE, SINGLE, 	LENGHT_1, RES_WINDOW, RD_0, WR_0}},		//-
	/*0203*/	{(uint32_t*)& NULL_PARAM,        				  {RAM, INTERNAL, DATA, BYTE, SINGLE, 	LENGHT_1, RES_WINDOW, RD_0, WR_0}},		//-
	/*0204*/	{(uint32_t*)& NULL_PARAM,        				  {RAM, INTERNAL, DATA, BYTE, SINGLE, 	LENGHT_1, RES_WINDOW, RD_0, WR_0}},		//-
	/*0205*/	{(uint32_t*)& NULL_PARAM,        				  {RAM, INTERNAL, DATA, BYTE, SINGLE, 	LENGHT_1, RES_WINDOW, RD_0, WR_0}},		//-
	/*0206*/	{(uint32_t*)& NULL_PARAM,        				  {RAM, INTERNAL, DATA, BYTE, SINGLE, 	LENGHT_1, RES_WINDOW, RD_0, WR_0}},		//-
	/*0207*/	{(uint32_t*)& NULL_PARAM,        				  {RAM, INTERNAL, DATA, BYTE, SINGLE, 	LENGHT_1, RES_WINDOW, RD_0, WR_0}},		//-
	/*0208*/	{(uint32_t*)& FrequencyMeasurements.IntegralInput0,    {RAM, INTERNAL, DATA, BYTE, SINGLE, 	LENGHT_2, RES_WINDOW, RD_0, WR_0}},		//-
	/*0209*/	{(uint32_t*)& FrequencyMeasurements.IntegralInput1,    {RAM, INTERNAL, DATA, BYTE, SINGLE, 	LENGHT_2, RES_WINDOW, RD_0, WR_0}},		//-	
};

const ParamDescript ParamDescrTable_0x03[] =                                                                        //<! Таблица параметров группы 0x00xx 
{
	/*0300*/	{(uint32_t*)& FilteringInputIP.EnableFiltering,   {ROM, INTERNAL, DATA, BIT, SINGLE, 		   0, RES_WINDOW, RD_1, WR_2}},		//-
	/*0301*/	{(uint32_t*)& FilteringInputIP.AllowedIP_1,       {ROM, INTERNAL, DATA, BYTE, SINGLE, 	LENGHT_4, RES_WINDOW, RD_1, WR_2}},		//-
	/*0302*/	{(uint32_t*)& FilteringInputIP.AllowedIP_2,       {ROM, INTERNAL, DATA, BYTE, SINGLE, 	LENGHT_4, RES_WINDOW, RD_1, WR_2}},		//-
	/*0303*/	{(uint32_t*)& FilteringInputIP.AllowedIP_3,       {ROM, INTERNAL, DATA, BYTE, SINGLE, 	LENGHT_4, RES_WINDOW, RD_1, WR_2}},			//-
	/*0304*/	{(uint32_t*)& FilteringInputIP.AllowedIP_4,       {ROM, INTERNAL, DATA, BYTE, SINGLE, 	LENGHT_4, RES_WINDOW, RD_1, WR_2}},			//-
	/*0305*/	{(uint32_t*)& FilteringInputIP.AllowedIP_5,       {ROM, INTERNAL, DATA, BYTE, SINGLE, 	LENGHT_4, RES_WINDOW, RD_1, WR_2}},			//-
	/*0306*/	{(uint32_t*)& FilteringInputIP.AllowedIP_6,       {ROM, INTERNAL, DATA, BYTE, SINGLE, 	LENGHT_4, RES_WINDOW, RD_1, WR_2}},			//-
	/*0307*/	{(uint32_t*)& FilteringInputIP.AllowedIP_7,       {ROM, INTERNAL, DATA, BYTE, SINGLE, 	LENGHT_4, RES_WINDOW, RD_1, WR_2}},			//-
	/*0308*/	{(uint32_t*)& FilteringInputIP.AllowedIP_8,       {ROM, INTERNAL, DATA, BYTE, SINGLE, 	LENGHT_4, RES_WINDOW, RD_1, WR_2}},			//-
	/*0309*/	{(uint32_t*)& FilteringInputIP.AllowedIP_9,       {ROM, INTERNAL, DATA, BYTE, SINGLE, 	LENGHT_4, RES_WINDOW, RD_1, WR_2}},			//-
	/*030A*/	{(uint32_t*)& FilteringInputIP.AllowedIP_10,      {ROM, INTERNAL, DATA, BYTE, SINGLE, 	LENGHT_4, RES_WINDOW, RD_1, WR_2}},			//-
};

const ParamDescript ParamDescrTable_0x04[] =                                                                        //<! Таблица параметров группы 0x00xx 
{
	/*0001*/	{(uint32_t*)& NULL_PARAM,        				   {RAM, INTERNAL, DATA, BYTE, SINGLE, 	LENGHT_1, RES_WINDOW, RD_0, WR_0}},		//-
};

const ParamDescript ParamDescrTable_0x05[] =                                                                        //<! Таблица параметров группы 0x00xx 
{
	/*0500*/	{(uint32_t*)& NULL_PARAM,        				  {RAM, INTERNAL, DATA, BYTE, SINGLE, 	LENGHT_1, RES_WINDOW, RD_0, WR_0}},		//-
};

const ParamDescript ParamDescrTable_0x06[] =                                                                        //<! Таблица параметров группы 0x00xx 
{
	/*0001*/	{(uint32_t*)& NULL_PARAM,        				  {RAM, INTERNAL, DATA, BYTE, SINGLE, 	LENGHT_1, RES_WINDOW, RD_0, WR_0}},		//-
};

/*
const ParamDescript ParamDescrTable_0x07[] =                                                                        //<! Таблица параметров группы 0x00xx 
{
		{(uint32_t*)& NULL_PARAM,        				  {RAM, INTERNAL, DATA, BYTE, SINGLE, 	LENGHT_1, RES_WINDOW, RD_0, WR_0}},		//-
};

const ParamDescript ParamDescrTable_0x08[] =                                                                        //<! Таблица параметров группы 0x00xx 
{
		{(uint32_t*)& NULL_PARAM,        				  {RAM, INTERNAL, DATA, BYTE, SINGLE, 	LENGHT_1, RES_WINDOW, RD_0, WR_0}},		//-
};

const ParamDescript ParamDescrTable_0x09[] =                                                                        //<! Таблица параметров группы 0x00xx 
{
		{(uint32_t*)& NULL_PARAM,        				  {RAM, INTERNAL, DATA, BYTE, SINGLE, 	LENGHT_1, RES_WINDOW, RD_0, WR_0}},		//-
};
*/
const ParamDescript ParamDescrTable_0xF0[] =                                                                         //<! Таблица параметров группы 0xF0xx
{
    /*F000*/	{(uint32_t*)& GeneralSystemParameters_0xF0.DeviceType,        					{ROM, INTERNAL, DATA, BYTE, SINGLE , LENGHT_2, RES_WINDOW, RD_1, WR_3}},		//<! Device Type
	/*F001*/	{(uint32_t*)& GeneralSystemParameters_0xF0.FactoryNumber,       				{ROM, INTERNAL, DATA, BYTE, SINGLE , LENGHT_2, RES_WINDOW, RD_1, WR_3}},		//Factory Number
	/*F002*/	{(uint32_t*)& GeneralSystemParameters_0xF0.ProgramVersion,      				{ROM, INTERNAL, DATA, BYTE, SINGLE , LENGHT_1, RES_WINDOW, RD_1, WR_3}},		//Program Version
	/*F003*/	{(uint32_t*)& NULL_PARAM,        												{ROM, INTERNAL, DATA, BYTE, SINGLE , LENGHT_2, RES_WINDOW, RD_0, WR_0}},		//------------------------------------------
	/*F004*/	{(uint32_t*)& AppProgramFlashData_RAM_BUFFER.ApplicationProgramInstructionList, {ROM, INTERNAL, DATA, BYTE, MASSIVE, LENGHT_4, RES_WINDOW, RD_1, WR_2}},		//Queue of Tasks	    - Очередь задач
	/*F005*/	{(uint32_t*)& kostil.param_0xF005,        										{ROM, INTERNAL, DATA, BYTE, SINGLE , LENGHT_2, RES_WINDOW, RD_1, WR_2}},		//Timers Tasks 512Hz	-	Не фурычит
	/*F006*/	{(uint32_t*)& kostil.param_0xF006,        										{ROM, INTERNAL, DATA, BYTE, SINGLE , LENGHT_2, RES_WINDOW, RD_1, WR_2}},		//Timers Tasks 64Hz		-	Не фурычит
	/*F007*/	{(uint32_t*)& kostil.param_0xF007,        										{ROM, INTERNAL, DATA, BYTE, SINGLE , LENGHT_2, RES_WINDOW, RD_1, WR_2}},		//Timers Tasks 8Hz		-	Не фурычит
	/*F008*/	{(uint32_t*)& kostil.param_0xF008,        										{ROM, INTERNAL, DATA, BYTE, SINGLE , LENGHT_2, RES_WINDOW, RD_1, WR_2}},		//Timers Tasks 1Hz		-	Не фурычит
	/*F009*/	{(uint32_t*)& kostil.param_0xF009,        										{ROM, INTERNAL, DATA, BYTE, SINGLE , LENGHT_2, RES_WINDOW, RD_1, WR_2}},		//Interrupt Tasks			-	Не фурычит
	/*F00A*/	{(uint32_t*)& AppProgramFlashData_RAM_BUFFER.FlexibleParametersTable,       	{ROM, INTERNAL, DATA, BYTE, MASSIVE, LENGHT_4, RES_WINDOW, RD_1, WR_2}},		//Flexible Parameter Table
	/*F00B*/	{(uint32_t*)& AppProgramFlashData_RAM_BUFFER.FlexibleParametersDescriptorTable, {ROM, INTERNAL, DATA, BYTE, MASSIVE, LENGHT_4, RES_WINDOW, RD_1, WR_2}},		//Flexible Parameter Descriptor Table
	/*F00C*/	{(uint32_t*)& AppProgramFlashData_RAM_BUFFER.AppProgFuncListLenght,        		{ROM, INTERNAL, DATA, BYTE, SINGLE , LENGHT_4, RES_WINDOW, RD_1, WR_2}},		//Task Queue Length
	/*F00D*/	{(uint32_t*)& AppProgramFlashData_RAM_BUFFER.FlexParamTableLenght,        		{ROM, INTERNAL, DATA, BYTE, SINGLE , LENGHT_4, RES_WINDOW, RD_1, WR_2}},		//Flexible Parameter Table Lenght
	/*F00E*/	{(uint32_t*)& AppProgramFlashData_RAM_BUFFER.FlexParamDescrTableLenght,        	{ROM, INTERNAL, DATA, BYTE, SINGLE , LENGHT_4, RES_WINDOW, RD_1, WR_2}},		//Flexible Parameter Descriptor Table Lenght
	/*F00F*/	{(uint32_t*)& NULL_PARAM,        												{ROM, INTERNAL, DATA, BYTE, SINGLE , LENGHT_2, RES_WINDOW, RD_1, WR_3}},		//Длина области размещения гибких параметров
	/******************************************************************************************************************************************************************/
	/*F010*/	{(uint32_t*)& NULL_PARAM,        												{RAM, INTERNAL, DATA, BYTE, SINGLE , LENGHT_2, RES_WINDOW, RD_0, WR_0}},
	/*F011*/	{(uint32_t*)& NULL_PARAM,        												{RAM, INTERNAL, DATA, BYTE, SINGLE , LENGHT_2, RES_WINDOW, RD_0, WR_0}},
	/*F012*/	{(uint32_t*)& NULL_PARAM,       												{RAM, INTERNAL, DATA, BYTE, SINGLE , LENGHT_2, RES_WINDOW, RD_0, WR_0}},
	/*F013*/	{(uint32_t*)& NULL_PARAM,        												{RAM, INTERNAL, DATA, BYTE, SINGLE , LENGHT_2, RES_WINDOW, RD_0, WR_0}},
	/*F014*/	{(uint32_t*)& NULL_PARAM,        												{RAM, INTERNAL, DATA, BYTE, SINGLE , LENGHT_2, RES_WINDOW, RD_0, WR_0}},
	/*F015*/	{(uint32_t*)& NULL_PARAM,       												{RAM, INTERNAL, DATA, BYTE, SINGLE , LENGHT_2, RES_WINDOW, RD_0, WR_0}},
	/*F016*/	{(uint32_t*)& NULL_PARAM,        												{RAM, INTERNAL, DATA, BYTE, SINGLE , LENGHT_2, RES_WINDOW, RD_0, WR_0}},
	/*F017*/	{(uint32_t*)& NULL_PARAM,        												{RAM, INTERNAL, DATA, BYTE, SINGLE , LENGHT_2, RES_WINDOW, RD_0, WR_0}},
	/*F018*/	{(uint32_t*)& NULL_PARAM,       												{RAM, INTERNAL, DATA, BYTE, SINGLE , LENGHT_2, RES_WINDOW, RD_0, WR_0}},
	/*F019*/	{(uint32_t*)& NULL_PARAM,        												{RAM, INTERNAL, DATA, BYTE, SINGLE , LENGHT_1, RES_WINDOW, RD_0, WR_0}},
	/*F01A*/	{(uint32_t*)& NULL_PARAM,        											{ROM, INTERNAL, DATA, BYTE, SINGLE , LENGHT_1, RES_WINDOW, RD_1, WR_2}},
	/*F01B*/	{(uint32_t*)& NULL_PARAM,        												{RAM, INTERNAL, DATA, BYTE, SINGLE , LENGHT_2, RES_WINDOW, RD_0, WR_0}},
	/*F01C*/	{(uint32_t*)& CurrentState.currentAccessLevel,         							{RAM, INTERNAL, DATA, BYTE, SINGLE , LENGHT_1, RES_WINDOW, RD_1, WR_0}},
	/*F01D*/	{(uint32_t*)& kostil.param_0xF01D,                                              {RAM, INTERNAL, DATA, BYTE, SINGLE , LENGHT_4, RES_WINDOW, RD_1, WR_3}},
	/*F01E*/	{(uint32_t*)& NULL_PARAM,        												{RAM, INTERNAL, DATA, BYTE, SINGLE , LENGHT_4, RES_WINDOW, RD_0, WR_0}},
	/*F01F*/	{(uint32_t*)& NULL_PARAM,       												{RAM, INTERNAL, DATA, BYTE, SINGLE , LENGHT_2, RES_WINDOW, RD_0, WR_0}},
	/******************************************************************************************************************************************************************/	
	/*F020*/	{(uint32_t*)& NULL_PARAM,        												{RAM, INTERNAL, DATA, BYTE, SINGLE , LENGHT_2, RES_WINDOW, RD_0, WR_0}},
	/*F021*/	{(uint32_t*)& kostil.param_0xF021,        										{RAM, INTERNAL, DATA, BYTE, SINGLE , LENGHT_4, RES_WINDOW, RD_1, WR_3}},
	/*F022*/	{(uint32_t*)& NULL_PARAM,        												{RAM, INTERNAL, DATA, BYTE, SINGLE , LENGHT_2, RES_WINDOW, RD_0, WR_0}},
	/*F023*/	{(uint32_t*)& NULL_PARAM,        												{RAM, INTERNAL, DATA, BYTE, SINGLE , LENGHT_2, RES_WINDOW, RD_0, WR_0}},
	/*F024*/	{(uint32_t*)& Passwords.userPassword,        									{ROM, INTERNAL, DATA, BYTE, SINGLE , LENGHT_1, RES_WINDOW, RD_2, WR_2}},
	/*F025*/	{(uint32_t*)& Passwords.adjusterPassword,         								{ROM, INTERNAL, DATA, BYTE, SINGLE , LENGHT_4, RES_WINDOW, RD_2, WR_2}},		//Ïàðîëü íàëàä÷èêà
	/*F026*/	{(uint32_t*)& Passwords.developerPassword,        								{ROM, INTERNAL, DATA, BYTE, SINGLE , LENGHT_4, RES_WINDOW, RD_3, WR_0}},		//Factory password
	/*F027*/	{(uint32_t*)& GeneralSystemParameters_0xF0.screenPassword,						{ROM, INTERNAL, DATA, BYTE, SINGLE , LENGHT_2, RES_WINDOW, RD_2, WR_2}},
	/*F028*/	{(uint32_t*)& AppProgramFlashData_RAM_BUFFER.ApplicationProgramIdentificator,   {RAM, INTERNAL, DATA, BYTE, MASSIVE, LENGHT_4, RES_WINDOW, RD_2, WR_2}},
	/*F029*/	{(uint32_t*)& NULL_PARAM,        												{RAM, INTERNAL, DATA, BYTE, MASSIVE, LENGHT_1, RES_WINDOW, RD_0, WR_0}},
	/*F02A*/	{(uint32_t*)& NULL_PARAM,        												{RAM, INTERNAL, DATA, BYTE, SINGLE , LENGHT_2, RES_WINDOW, RD_0, WR_0}},
	/*F02B*/	{(uint32_t*)& NULL_PARAM,        												{RAM, INTERNAL, DATA, BYTE, SINGLE , LENGHT_2, RES_WINDOW, RD_0, WR_0}},
	/*F02C*/	{(uint32_t*)& NULL_PARAM,        												{RAM, INTERNAL, DATA, BYTE, SINGLE , LENGHT_2, RES_WINDOW, RD_0, WR_0}},
	/*F02D*/	{(uint32_t*)& NULL_PARAM,       												{RAM, INTERNAL, DATA, BYTE, SINGLE , LENGHT_2, RES_WINDOW, RD_0, WR_0}},
	/*F02E*/	{(uint32_t*)& NULL_PARAM,        												{RAM, INTERNAL, DATA, BYTE, SINGLE , LENGHT_2, RES_WINDOW, RD_0, WR_0}},
	/*F02F*/	{(uint32_t*)& NULL_PARAM,        												{RAM, INTERNAL, DATA, BYTE, SINGLE , LENGHT_2, RES_WINDOW, RD_0, WR_0}},
	/******************************************************************************************************************************************************************/	
	/*F030*/	{(uint32_t*)& NULL_PARAM,        												{RAM, INTERNAL, DATA, BYTE, SINGLE , LENGHT_2, RES_WINDOW, RD_0, WR_0}},
	/*F031*/	{(uint32_t*)& NULL_PARAM,        												{RAM, INTERNAL, DATA, BYTE, SINGLE , LENGHT_2, RES_WINDOW, RD_0, WR_0}},
	/*F032*/	{(uint32_t*)& NULL_PARAM,        												{RAM, INTERNAL, DATA, BYTE, SINGLE , LENGHT_2, RES_WINDOW, RD_0, WR_0}},
	/*F033*/	{(uint32_t*)& NULL_PARAM,        												{RAM, INTERNAL, DATA, BYTE, SINGLE , LENGHT_2, RES_WINDOW, RD_0, WR_0}},
	/*F034*/	{(uint32_t*)& GeneralSystemParameters_0xF0.AppProgramName,        				{RAM, INTERNAL, DATA, BYTE, MASSIVE, LENGHT_4, RES_WINDOW, RD_1, WR_2}},
	/*F035*/	{(uint32_t*)& GeneralSystemParameters_0xF0.factoryProjName,      				{RAM, INTERNAL, DATA, BYTE, MASSIVE, LENGHT_4, RES_WINDOW, RD_1, WR_2}},	/*F036*/	{(uint32_t*)& NULL_PARAM,        												{RAM, INTERNAL, DATA, BYTE, SINGLE , LENGHT_2, RES_WINDOW, RD_1, WR_3}},
	/*F037*/	{(uint32_t*)& NULL_PARAM,        												{RAM, INTERNAL, DATA, BYTE, SINGLE , LENGHT_2, RES_WINDOW, RD_0, WR_0}},
	/*F038*/	{(uint32_t*)& NULL_PARAM,        												{RAM, INTERNAL, DATA, BYTE, SINGLE , LENGHT_2, RES_WINDOW, RD_0, WR_0}},
	/*F039*/	{(uint32_t*)& NULL_PARAM,        												{RAM, INTERNAL, DATA, BYTE, SINGLE , LENGHT_2, RES_WINDOW, RD_0, WR_0}},
	/*F03A*/	{(uint32_t*)& NULL_PARAM,        												{RAM, INTERNAL, DATA, BYTE, SINGLE , LENGHT_2, RES_WINDOW, RD_0, WR_0}},
	/*F03B*/	{(uint32_t*)& NULL_PARAM,        												{RAM, INTERNAL, DATA, BYTE, SINGLE , LENGHT_2, RES_WINDOW, RD_0, WR_0}},
	/*F03C*/	{(uint32_t*)& NULL_PARAM,        												{RAM, INTERNAL, DATA, BYTE, SINGLE , LENGHT_2, RES_WINDOW, RD_0, WR_0}},
	/*F03D*/	{(uint32_t*)& NULL_PARAM,        												{RAM, INTERNAL, DATA, BYTE, SINGLE , LENGHT_2, RES_WINDOW, RD_0, WR_0}},
	/*F03E*/	{(uint32_t*)& NULL_PARAM,        												{RAM, INTERNAL, DATA, BYTE, SINGLE , LENGHT_2, RES_WINDOW, RD_0, WR_0}},
	/*F03F*/	{(uint32_t*)& NULL_PARAM,        												{RAM, INTERNAL, DATA, BYTE, SINGLE , LENGHT_2, RES_WINDOW, RD_0, WR_0}},
	/******************************************************************************************************************************************************************/	
	/*F040*/	{(uint32_t*)& GeneralSystemParameters_0xF0.SerialNumber,        				{ROM, INTERNAL, DATA, BYTE, SINGLE , LENGHT_2, RES_WINDOW, RD_1, WR_3}}		//Номер серии приборов			
};

const ParamDescript * const generalGroupTable[]                //<! Сводный массив указателей на таблицы параметров по группам 
{
	&ParamDescrTable_0x00[0],
	&ParamDescrTable_0x01[0],
    &ParamDescrTable_0x02[0],
    &ParamDescrTable_0x03[0],
    &ParamDescrTable_0x04[0],
    &ParamDescrTable_0x05[0],
    &ParamDescrTable_0x06[0],
//    &ParamDescrTable_0x07[0],
//    &ParamDescrTable_0x08[0],
//    &ParamDescrTable_0x09[0],
    &ParamDescrTable_0xF0[0]
};


const int ParametersTablesLenght[] =		                    //<! Длины каждого типа (каждой группы) жестких параметров 
{
	(sizeof(ParamDescrTable_0x00) / sizeof(ParamDescript)),
	(sizeof(ParamDescrTable_0x01) / sizeof(ParamDescript)),
	(sizeof(ParamDescrTable_0x02) / sizeof(ParamDescript)),
	(sizeof(ParamDescrTable_0x03) / sizeof(ParamDescript)),
	(sizeof(ParamDescrTable_0x04) / sizeof(ParamDescript)),
	(sizeof(ParamDescrTable_0x05) / sizeof(ParamDescript)),
	(sizeof(ParamDescrTable_0x06) / sizeof(ParamDescript)),
	(sizeof(ParamDescrTable_0xF0) / sizeof(ParamDescript))
//	(sizeof(ParamDescrTable_0x08) / sizeof(ParamDescript)),
//	(sizeof(ParamDescrTable_0x09) / sizeof(ParamDescript)),
//	(sizeof(ParamDescrTable_0xF0) / sizeof(ParamDescript))
};


void ParamSys::clearRAM()
{
	uint8_t *address = (uint8_t*)&AppProgramFlashData_RAM_BUFFER;
	uint32_t size = sizeof(AppProgramFlashData_RAM_BUFFER);
	for(uint32_t i = 0; i < sizeof(AppProgramFlashData_RAM_BUFFER); i++)
	{
		*(uint8_t*)address = 0xFF;
		address++;
	}
	address  = (uint8_t*)&AppProgramParametersFlashData_RAM_BUFFER;
	for(uint32_t i = 0; i < sizeof(AppProgramParametersFlashData_RAM_BUFFER); i++)
	{
		*(uint8_t*)address = 0xFF;
		address++;
	}
	address = (uint8_t*)&AppProgramParametersRAM_Data;
	for(uint32_t i = 0; i < sizeof(AppProgramParametersRAM_Data); i++)
	{
		*(uint8_t*)address = 0xFF;
		address++;
	}
}


void ParamSys::copyFromAllFlashToRAM()
{
	
	uint8_t *destinationAddress = (uint8_t*)&AppProgramFlashData_RAM_BUFFER;
	uint8_t *sourceAddress = (uint8_t*)&AppProgramFlashData;
	
	uint32_t AppProgramFlashDataSize = sizeof(AppProgramFlashData);
	for(uint32_t i = 0; i < AppProgramFlashDataSize; i++)
	{
		*(uint8_t*)destinationAddress = *(uint8_t*)sourceAddress;
		sourceAddress++;
		destinationAddress++;
	}
	destinationAddress = (uint8_t*)&AppProgramParametersFlashData_RAM_BUFFER;
	sourceAddress = (uint8_t*)&AppProgramParametersFlashData;
	uint32_t AppProgramParametersFlashDataSize = sizeof(AppProgramParametersFlashData);
	for(uint32_t i = 0; i < AppProgramParametersFlashDataSize; i++)
	{
		*(uint8_t*)destinationAddress = *(uint8_t*)sourceAddress;
		sourceAddress++;
		destinationAddress++;
	}
	/*
	destinationAddress = (uint8_t*)&EventLogFlashData_RAM_BUFFER;
	sourceAddress = (uint8_t*)&EventLogFlashData;
	uint32_t EventLogFlashDataSize = sizeof(EventLogFlashData);
	for(uint32_t i = 0; i < EventLogFlashDataSize; i++)
	{
		*(uint8_t*)destinationAddress = *(uint8_t*)sourceAddress;
		sourceAddress++;
		destinationAddress++;
	}
	*/
	ParamDescript parameter;
	for(uint32_t paramType = 0; paramType <= LAST_TYPE_PARAMETERS; paramType++)			// Цикл прохождения по типам жестких параметрам ParamDescrTable_ХХ
	{
		if(paramType == 0x01) // unused parameters
		{
			continue;
		}
		if(paramType == 0x04) // unused parameters
		{
			continue;
		}
		if(paramType == 0x05) // unused parameters
		{
			continue;
		}
		if(paramType == 0x06) // unused parameters
		{
			continue;
		}
		if(paramType == 0x07) // unused parameters
		{
			continue;
		}
		if(paramType == 0x09) // Event Log
		{
			continue;
		}

 		for(int paramNumber = 0; paramNumber < ParametersTablesLenght[paramType]; paramNumber++) // Цикл прохождения по каждому параметру в таблице
		{
			uint16_t param = (paramType << 8) + paramNumber;	// номера текущего параметра 
			bool wrongParameter = 0;
			
			for(uint32_t i = 0; i < sizeof(AppProgramParameters) / sizeof(uint16_t); i++)
			{
				if(AppProgramParameters[i] == param)
				{
					wrongParameter = 1;
					continue;
				}
			}
			if(wrongParameter == 1)
			{
				wrongParameter = 0;
				continue;
			}
			
			parameter = generalGroupTable[paramType][paramNumber];
			destinationAddress =  (uint8_t*)parameter.ParametersAddress;			// Адрес куда пишем
			//uint8_t *destinationAddress = (uint8_t*)parameter.ParametersAddress;	// Адрес куда пишем
			int32_t fullParamNumber = paramNumber + (paramType << 8);				// полный номер параметра
			sourceAddress = ((uint8_t*)&DeviceConstParameters[0]) + (fullParamNumber * 4); // откуда пишем
			//uint8_t *sourceAddress = ((uint8_t*)&DeviceConstParameters[0]) + (fullParamNumber * 4);	// откуда пишем
			if(parameter.Description.memoryTypeTag == ROM)		// метка хранения параметра во FLASH
			{
				for(int byteNumber = 0; byteNumber < parameter.Description.parameterSize; byteNumber++)
				{
					*destinationAddress = *(uint8_t*)sourceAddress;
					destinationAddress++;
					sourceAddress++;
				}
			}
			
		}
	}
}

void ParamSys::copyFromAllRAMToFlash()
{
	this->AppProgramDataDesyncFlag = true;
	this->AppProgramParametersDesyncFlag = true;
	this->DeviceConstParametersDesyncFlag = true;
	doMemorySync();
}

//--------------------------------------------------------
// CRC32 Table
//--------------------------------------------------------
const uint32_t Crc32Table[256]=
  {
	0x00000000, 0x77073096, 0xEE0E612C, 0x990951BA,
    0x076DC419, 0x706AF48F, 0xE963A535, 0x9E6495A3,
    0x0EDB8832, 0x79DCB8A4, 0xE0D5E91E, 0x97D2D988,
    0x09B64C2B, 0x7EB17CBD, 0xE7B82D07, 0x90BF1D91,
    0x1DB71064, 0x6AB020F2, 0xF3B97148, 0x84BE41DE,
    0x1ADAD47D, 0x6DDDE4EB, 0xF4D4B551, 0x83D385C7,
    0x136C9856, 0x646BA8C0, 0xFD62F97A, 0x8A65C9EC,
    0x14015C4F, 0x63066CD9, 0xFA0F3D63, 0x8D080DF5,
    0x3B6E20C8, 0x4C69105E, 0xD56041E4, 0xA2677172,
    0x3C03E4D1, 0x4B04D447, 0xD20D85FD, 0xA50AB56B,
    0x35B5A8FA, 0x42B2986C, 0xDBBBC9D6, 0xACBCF940,
    0x32D86CE3, 0x45DF5C75, 0xDCD60DCF, 0xABD13D59,
    0x26D930AC, 0x51DE003A, 0xC8D75180, 0xBFD06116,
    0x21B4F4B5, 0x56B3C423, 0xCFBA9599, 0xB8BDA50F,
    0x2802B89E, 0x5F058808, 0xC60CD9B2, 0xB10BE924,
    0x2F6F7C87, 0x58684C11, 0xC1611DAB, 0xB6662D3D,
    0x76DC4190, 0x01DB7106, 0x98D220BC, 0xEFD5102A,
    0x71B18589, 0x06B6B51F, 0x9FBFE4A5, 0xE8B8D433,
    0x7807C9A2, 0x0F00F934, 0x9609A88E, 0xE10E9818,
    0x7F6A0DBB, 0x086D3D2D, 0x91646C97, 0xE6635C01,
    0x6B6B51F4, 0x1C6C6162, 0x856530D8, 0xF262004E,
    0x6C0695ED, 0x1B01A57B, 0x8208F4C1, 0xF50FC457,
    0x65B0D9C6, 0x12B7E950, 0x8BBEB8EA, 0xFCB9887C,
    0x62DD1DDF, 0x15DA2D49, 0x8CD37CF3, 0xFBD44C65,
    0x4DB26158, 0x3AB551CE, 0xA3BC0074, 0xD4BB30E2,
    0x4ADFA541, 0x3DD895D7, 0xA4D1C46D, 0xD3D6F4FB,
    0x4369E96A, 0x346ED9FC, 0xAD678846, 0xDA60B8D0,
    0x44042D73, 0x33031DE5, 0xAA0A4C5F, 0xDD0D7CC9,
    0x5005713C, 0x270241AA, 0xBE0B1010, 0xC90C2086,
    0x5768B525, 0x206F85B3, 0xB966D409, 0xCE61E49F,
    0x5EDEF90E, 0x29D9C998, 0xB0D09822, 0xC7D7A8B4,
    0x59B33D17, 0x2EB40D81, 0xB7BD5C3B, 0xC0BA6CAD,
    0xEDB88320, 0x9ABFB3B6, 0x03B6E20C, 0x74B1D29A,
    0xEAD54739, 0x9DD277AF, 0x04DB2615, 0x73DC1683,
    0xE3630B12, 0x94643B84, 0x0D6D6A3E, 0x7A6A5AA8,
    0xE40ECF0B, 0x9309FF9D, 0x0A00AE27, 0x7D079EB1,
    0xF00F9344, 0x8708A3D2, 0x1E01F268, 0x6906C2FE,
    0xF762575D, 0x806567CB, 0x196C3671, 0x6E6B06E7,
    0xFED41B76, 0x89D32BE0, 0x10DA7A5A, 0x67DD4ACC,
    0xF9B9DF6F, 0x8EBEEFF9, 0x17B7BE43, 0x60B08ED5,
    0xD6D6A3E8, 0xA1D1937E, 0x38D8C2C4, 0x4FDFF252,
    0xD1BB67F1, 0xA6BC5767, 0x3FB506DD, 0x48B2364B,
    0xD80D2BDA, 0xAF0A1B4C, 0x36034AF6, 0x41047A60,
    0xDF60EFC3, 0xA867DF55, 0x316E8EEF, 0x4669BE79,
    0xCB61B38C, 0xBC66831A, 0x256FD2A0, 0x5268E236,
    0xCC0C7795, 0xBB0B4703, 0x220216B9, 0x5505262F,
    0xC5BA3BBE, 0xB2BD0B28, 0x2BB45A92, 0x5CB36A04,
    0xC2D7FFA7, 0xB5D0CF31, 0x2CD99E8B, 0x5BDEAE1D,
    0x9B64C2B0, 0xEC63F226, 0x756AA39C, 0x026D930A,
    0x9C0906A9, 0xEB0E363F, 0x72076785, 0x05005713,
    0x95BF4A82, 0xE2B87A14, 0x7BB12BAE, 0x0CB61B38,
    0x92D28E9B, 0xE5D5BE0D, 0x7CDCEFB7, 0x0BDBDF21,
    0x86D3D2D4, 0xF1D4E242, 0x68DDB3F8, 0x1FDA836E,
    0x81BE16CD, 0xF6B9265B, 0x6FB077E1, 0x18B74777,
    0x88085AE6, 0xFF0F6A70, 0x66063BCA, 0x11010B5C,
    0x8F659EFF, 0xF862AE69, 0x616BFFD3, 0x166CCF45,
    0xA00AE278, 0xD70DD2EE, 0x4E048354, 0x3903B3C2,
    0xA7672661, 0xD06016F7, 0x4969474D, 0x3E6E77DB,
    0xAED16A4A, 0xD9D65ADC, 0x40DF0B66, 0x37D83BF0,
    0xA9BCAE53, 0xDEBB9EC5, 0x47B2CF7F, 0x30B5FFE9,
    0xBDBDF21C, 0xCABAC28A, 0x53B39330, 0x24B4A3A6,
    0xBAD03605, 0xCDD70693, 0x54DE5729, 0x23D967BF,
    0xB3667A2E, 0xC4614AB8, 0x5D681B02, 0x2A6F2B94,
    0xB40BBE37, 0xC30C8EA1, 0x5A05DF1B, 0x2D02EF8D
  };
//--------------------------------------------------------

void generateDeveloperPassword(void)
{
	uint32_t	crc = 0;
	uint16_t factoryNumber = GeneralSystemParameters_0xF0.FactoryNumber;
	uint16_t deviceType = GeneralSystemParameters_0xF0.DeviceType;
	uint8_t buffer[4] = {0, 0, 0, 0};
	//Passwords.developerPassword = 0;
	buffer[0] = (deviceType) & 0xFF;
	buffer[1] = ((factoryNumber) & 0xFF00) >> 8;
	buffer[2] = ((deviceType) & 0xFF00) >> 8;
	buffer[3] = (factoryNumber) & 0xFF;
	crc = 0xFFFFFFFF;
	for	(uint8_t i = 0; i <= 3; i++)
	{
		crc = (crc >> 8) ^ Crc32Table[(crc ^ buffer[i]) & 0xFF];
	}
	crc = crc ^ 0xFFFFFFFF;
	buffer[0] = (uint8_t)(crc >> 24);
	buffer[1] = (uint8_t)(crc >> 16);
	buffer[2] = (uint8_t)(crc >> 8);
	buffer[3] = (uint8_t)(crc);
	for(uint8_t i = 0; i < 4; i++)
	{
		//Passwords.developerPassword |= buffer[i] << (i * 8);
	}

	return;
}



void ParamSys::startParametersSystem()
{
	// AppProgramFlashData_RAM_BUFFER_SCOPE.startAddress 			= (uint32_t)&AppProgramFlashData_RAM_BUFFER;
	// AppProgramFlashData_RAM_BUFFER_SCOPE.endAddress 			= (uint32_t)(&AppProgramFlashData_RAM_BUFFER) + (sizeof(AppProgramFlashData_RAM_BUFFER)) - 1;
	// AppProgramParametersFlashData_SCOPE.startAddress 			= (uint32_t)&AppProgramParametersFlashData;
	// AppProgramParametersFlashData_SCOPE.endAddress 				= (uint32_t)(&AppProgramParametersFlashData) + (sizeof(AppProgramParametersFlashData));
	// AppProgramParametersFlashData_RAM_BUFFER_SCOPE.startAddress = (uint32_t)&AppProgramParametersFlashData_RAM_BUFFER;
	// AppProgramParametersFlashData_RAM_BUFFER_SCOPE.endAddress	= (uint32_t)(&AppProgramParametersFlashData_RAM_BUFFER) + (sizeof(AppProgramParametersFlashData_RAM_BUFFER)) - 1;
	// AppProgramParametersRAM_Data_SCOPE.startAddress 			= (uint32_t)&AppProgramParametersRAM_Data;
	// AppProgramParametersRAM_Data_SCOPE.endAddress 				= (uint32_t)(&AppProgramParametersRAM_Data) + (sizeof(AppProgramParametersRAM_Data)) - 1;
	DeviceConstParameters_SCOPE.startAddress 					= (uint32_t)&DeviceConstParameters;
	DeviceConstParameters_SCOPE.endAddress						= (uint32_t)(&DeviceConstParameters) + (sizeof(DeviceConstParameters)) - 1;


	this->clearRAM();
	this->copyFromAllFlashToRAM();
	ParamDescript parameter;
	parameter = getParameter(0xF000);
	//setParameter(0xF002, FIRMWARE_VER);
	uint16_t deviceType = *(uint16_t*)parameter.ParametersAddress;
	if(deviceType != 0x1105)											//добавить проверку по CRC32
	{
		this->returnToFuctorySettings();
		this->copyFromAllRAMToFlash();
	}
	/*
	for(int i = 0; i < SECTOR_USED_NUMBER; i++)
	{
		erasedSectors[i] = 0xFFFFFFFF;
	}
	*/
	for(uint32_t i = 0; i < sizeof(AppProgramParametersRAM_Data.applicationProgramRAMBank1); i++)
	{
		AppProgramParametersRAM_Data.applicationProgramRAMBank1[i].param = 0;
	}
	generateDeveloperPassword();
}

void ParamSys::setDesynchronizationFlag(uint16_t param)
{
	ParamDescript Param = getParameter(param);
	uint32_t *parameterAddress = Param.ParametersAddress;
	if(Param.Description.memoryTypeTag == RAM)
		return;
	if(((uint32_t)parameterAddress >= AppProgramFlashData_RAM_BUFFER_SCOPE.startAddress) && ((uint32_t)parameterAddress <= AppProgramFlashData_RAM_BUFFER_SCOPE.endAddress))
	{
		this->AppProgramDataDesyncFlag = true;
	}else if(((uint32_t)parameterAddress >= AppProgramParametersFlashData_RAM_BUFFER_SCOPE.startAddress) && ((uint32_t)parameterAddress <= AppProgramParametersFlashData_RAM_BUFFER_SCOPE.endAddress))
	{
		this->AppProgramParametersDesyncFlag = true;
	}else if(((uint32_t)parameterAddress >= EventLogFlashData_RAM_BUFFER_SCOPE.startAddress) && ((uint32_t)parameterAddress <= EventLogFlashData_RAM_BUFFER_SCOPE.endAddress))
	{
		this->EventLogDesyncFlag = true;
	}else
	{
		this->DeviceConstParametersDesyncFlag = true;
	}
}


void ParamSys:: doMemorySync()
{
	uint32_t destinationAddress = 0;
	uint8_t *sourceAddress = 0;
	
    //__disable_irq();

	if(DeviceConstParametersDesyncFlag == true)
	{
		EEPROM_SetLatency(EEPROM_Latency_1);
    	EEPROM_ErasePage(DEVICE_PARAM_FLASH_PAGE_30, EEPROM_Main_Bank_Select);
		EEPROM_ErasePage(DEVICE_PARAM_FLASH_PAGE_31, EEPROM_Main_Bank_Select);
	}
	if(AppProgramDataDesyncFlag == true)
	{
		EEPROM_SetLatency(EEPROM_Latency_1);
    	EEPROM_ErasePage(APP_FLASH_PAGE_28, EEPROM_Main_Bank_Select);
	}
	if(AppProgramParametersDesyncFlag == true)
	{
		EEPROM_SetLatency(EEPROM_Latency_1);
    	EEPROM_ErasePage(APP_PARAM_FLASH_PAGE_29, EEPROM_Main_Bank_Select);
	}

	if(DeviceConstParametersDesyncFlag == true)
	{
		//flashErasePage(DEVICE_PARAM_FLASH_PAGE_24);
		//flashErasePage(DEVICE_PARAM_FLASH_PAGE_25);
		//flashErasePage(DEVICE_PARAM_FLASH_PAGE_26);
		//flashErasePage(DEVICE_PARAM_FLASH_PAGE_27);	
		//flashErasePage(DEVICE_PARAM_FLASH_PAGE_28);
		//flashErasePage(DEVICE_PARAM_FLASH_PAGE_29);	
		ParamDescript parameter;
		for(int paramType = 0; paramType <= LAST_TYPE_PARAMETERS; paramType++)
		{
			if(paramType == 0x01) // unused parameters
			{
				continue;
			}
			if(paramType == 0x04) // unused parameters
			{
				continue;
			}
			if(paramType == 0x05) // unused parameters
			{
				continue;
			}
			if(paramType == 0x06) // unused parameters
			{
				continue;
			}
			if(paramType == 0x07) // unused parameters
			{
				continue;
			}
			if(paramType == 0x09) // Event Log
			{
				continue;
			}
			for(int paramNumber = 0; paramNumber < ParametersTablesLenght[paramType]; paramNumber++)
			{
				uint16_t param = (paramType << 8) + paramNumber;
				bool wrongParameter = 0;
				
				for(uint32_t i = 0; i < sizeof(AppProgramParameters) / sizeof(uint16_t); i++)
				{
					if(AppProgramParameters[i] == param)
					{
						wrongParameter = 1;
						continue;
					}
				}
				if(wrongParameter == 1)
				{
					wrongParameter = 0;
					continue;
				}
				
				parameter = generalGroupTable[paramType][paramNumber];
				uint32_t fullParamNumber = paramNumber + (paramType << 8);
				sourceAddress =  (uint8_t*)parameter.ParametersAddress;
				destinationAddress = ((uint32_t)&DeviceConstParameters[0]) + fullParamNumber * 4;
				if(parameter.Description.memoryTypeTag == ROM)
				{
					for(int byteNumber = 0; byteNumber < parameter.Description.parameterSize; byteNumber++)
					{
						//flashProgramByte(destinationAddress, *(uint8_t*)sourceAddress);
						EEPROM_ProgramByte(destinationAddress, 0, *(uint8_t*)sourceAddress);
						sourceAddress++;
						destinationAddress++;
					}
				}
			}
		}
		DeviceConstParametersDesyncFlag = false;
		//DeviceConstParametersDataChangeFlag = true;
		
	}

	if(AppProgramDataDesyncFlag == true)
	{
		destinationAddress = (uint32_t)&AppProgramFlashData;
		sourceAddress = (uint8_t*)&AppProgramFlashData_RAM_BUFFER;
		uint32_t AppProgramSize = sizeof(AppProgramFlashData);
		for(uint32_t i = 0; i < AppProgramSize; i++)
		{
			//flashProgramByte(destinationAddress, *(uint8_t*)sourceAddress);
			EEPROM_ProgramByte(destinationAddress, 0, *(uint8_t*)sourceAddress);
			sourceAddress++;
			destinationAddress++;
		}
		AppProgramDataDesyncFlag = false;
	}

	if(AppProgramParametersDesyncFlag == true)
	{
		destinationAddress = (uint32_t)&AppProgramParametersFlashData;
		sourceAddress = (uint8_t*)&AppProgramParametersFlashData_RAM_BUFFER;
		uint32_t AppProgramParametersSize = sizeof(AppProgramParametersFlashData);
		for(uint32_t i = 0; i < AppProgramParametersSize; i++)
		{
			//flashProgramByte(destinationAddress, *(uint8_t*)sourceAddress);
			EEPROM_ProgramByte(destinationAddress, 0, *(uint8_t*)sourceAddress);
			sourceAddress++;
			destinationAddress++;
		}
		AppProgramParametersDesyncFlag = false;
	}

	//__enable_irq();
}

uint32_t memorySyncInterruptHandler(void)
{
	ParametersSystem.doMemorySync();
	return 0;
}


bool ParamSys::checkingParameter(uint16_t param)
{
	uint8_t paramType = param >> 8;
    uint8_t paramNumber = param & 0xFF;
	if(paramType == 0xF0)
		paramType = LAST_TYPE_PARAMETERS;
	if((paramType > LAST_TYPE_PARAMETERS) && (paramType < 0x80))
	{
		return false;
	}
	if(paramType <= LAST_TYPE_PARAMETERS)
	{
		if(ParametersTablesLenght[paramType] < paramNumber)
		{
			return false;
		}
		ParamDescript parameter = generalGroupTable[paramType][paramNumber];
		if(parameter.ParametersAddress == &NULL_PARAM)
		{
			return false;
		}
	}else{
		// ПРОВЕРКА ГИБКИХ ПАРАМЕТРОВ
	}
	return true;
}

ParamDescript ParamSys::getParameter(uint16_t param)
{
    uint8_t paramType = param >> 8;
    uint8_t paramNumber = param & 0xFF;
    ParamDescript parameter;
    if(paramType == 0xF0) 
        paramType = LAST_TYPE_PARAMETERS;
    if(paramType <= LAST_TYPE_PARAMETERS)
    {
		if(ParametersTablesLenght[paramType] < paramNumber)
		{
			parameter.ParametersAddress = 0;			// возвращаю нулевой указатель если нет параметра
			parameter.Description.parameterSize = 0;	// и на всякий случай нулевой размер параметра
			return parameter; 							// доделать
		}
        parameter = generalGroupTable[paramType][paramNumber];
        return  parameter;
    }else if(paramType >= 0x80)
	{
		uint8_t *appParamAddress 	  				= (uint8_t*)generalGroupTable[LAST_TYPE_PARAMETERS][0x0B].ParametersAddress;
		uint8_t *parameterGroupOffsetAdress 		= (uint8_t*)(appParamAddress + (paramType & 0x0F) * sizeof(uint16_t));
		uint16_t groupOffset		  				= (*parameterGroupOffsetAdress) + ((*(parameterGroupOffsetAdress + 1)) << 8);
		if(groupOffset == 0xFFFF)
		{
			parameter.ParametersAddress = 0;			// возвращаю нулевой указатель если нет параметра
			parameter.Description.parameterSize = 0;	// и на всякий случай нулевой размер параметра
			return parameter; 							// непонятно что возвращать
		}
		uint8_t *appParamDescriptorAddress	  		= (uint8_t*)(appParamAddress + groupOffset + paramNumber * sizeof(uint32_t));
		uint32_t ParamAddress						= (*appParamDescriptorAddress) + ((*(appParamDescriptorAddress + 1)) << 8);
		uint16_t ParamDescriptor					= (*(appParamDescriptorAddress + 3)) + ((*(appParamDescriptorAddress + 2)) << 8);
		parameter.Description.memoryTypeTag  		= (ParamDescriptor & 0x8000) >> 15;
		parameter.Description.memoryLocationTag  	= (ParamDescriptor & 0x4000) >> 14;
		parameter.Description.parameterTypeTag  	= (ParamDescriptor & 0x2000) >> 13;
		parameter.Description.bitTag 				= (ParamDescriptor & 0x1000) >> 12;
		parameter.Description.massiveTag  			= (ParamDescriptor & 0x0800) >> 11;
		parameter.Description.parameterSize  		= (ParamDescriptor & 0x0700) >> 8;
		parameter.Description.RESERVE  				= (ParamDescriptor & 0x00F0) >> 4; 
		parameter.Description.readProtectionType  	= (ParamDescriptor & 0x000C) >> 2;
		parameter.Description.writeProtectionType  	= (ParamDescriptor & 0x0003);
		if(parameter.Description.memoryTypeTag == RAM)
		{
			ParamAddress = ParamAddress + ((uint32_t)&AppProgramParametersRAM_Data.applicationProgramRAMBank1);
		}else if(parameter.Description.memoryTypeTag == ROM)
		{
			ParamAddress = ParamAddress + ((uint32_t)&AppProgramParametersFlashData_RAM_BUFFER.applicationProgramFlashBank1);
		}
		parameter.ParametersAddress = (uint32_t*)ParamAddress;
		return parameter;
	}else{
		parameter.ParametersAddress = 0;			//возвращаю нулевой указатель если нет параметра
		parameter.Description.parameterSize = 0;	//и на всякий случай нулевой размер параметра
		return parameter; 							//непонятно что возвращать
	}
	return parameter;
} 

ParamDescript ParamSys::setParameter(uint16_t parameter, uint32_t value)		
{
	ParamDescript Param = getParameter(parameter);
	if(Param.ParametersAddress == 0)
	{
		return Param;
	}
	uint8_t *temporaryParameter = (uint8_t*)Param.ParametersAddress;
	if(Param.Description.bitTag == 0)
	{
		for(int i = 0; i < Param.Description.parameterSize; i++)
		{
			*temporaryParameter = (uint8_t)(value >> (i * 8)) & 0xFF;
			temporaryParameter++;
		}
	}else{
		if(value == 0)
		{
			*temporaryParameter &= ~(0x1 << Param.Description.parameterSize);
		}else{
			*temporaryParameter |= value << Param.Description.parameterSize;
		}
	}
	ParameterHandlers.doParameterHandler(parameter);
	if(Param.Description.memoryTypeTag == ROM) //ROM
	{
		setDesynchronizationFlag(parameter);
		//doMemorySync();
		//OnSwTimer(&TIM[0],SWTIMER_MODE_EMPTY,0);   		// Удаляем таймер синхронизации памяти
		//OnSwTimer(&TIM[0],SWTIMER_MODE_CYCLE,4000);   	// Добавляем таймер одиничного импульса 
		// TIM[2].On = 1;									// Запускаем таймер
		// TIM[2].Off = 0; 
		//MemorySyncTimeoutTimer.resetTimerCounter();
		//MemorySyncTimeoutTimer.setOnePulseModeState(ENABLE);
		//MemorySyncTimeoutTimer.setCounterLimit(40000);
		//MemorySyncTimeoutTimer.setInterruptHandlerAddress((uint32_t)&memorySyncInterruptHandler);
		//MemorySyncTimeoutTimer.startTimer();
	}
	return Param;
}

ParamDescript ParamSys::getMassiveParameter(uint16_t param, uint32_t index)
{
    ParamDescript Param = ParametersSystem.getParameter(param);
	if(Param.ParametersAddress == 0)
	{
		return Param;
	}
	ParamDescript returnedParam;
	uint8_t *temporaryParameter;
	returnedParam.Description = Param.Description;
    //================== Проверка на наличие параметра ====================//
	temporaryParameter = (uint8_t*)Param.ParametersAddress;
	temporaryParameter = (temporaryParameter + (index * Param.Description.parameterSize));
	returnedParam.ParametersAddress = (uint32_t*)temporaryParameter;
	return returnedParam;
}

ParamDescript ParamSys::setMassiveParameter(uint16_t param, uint32_t index, uint32_t value)
{
	ParamDescript Param = getParameter(param);
	if((Param.ParametersAddress == 0) || (Param.Description.writeProtectionType > CurrentState.currentAccessLevel))
	{
		return Param;
	}
	uint8_t *temporaryParameter = (uint8_t*)Param.ParametersAddress;
	temporaryParameter = temporaryParameter + (index * Param.Description.parameterSize);
    for(int i = 0; i < Param.Description.parameterSize; i++)
	{
		*temporaryParameter = (uint8_t)(value >> (i * 8)) & 0xFF;
		temporaryParameter++;
	}
	if(Param.Description.memoryTypeTag == ROM) //ROM
	{
		setDesynchronizationFlag(param);
		doMemorySync();
		//OnSwTimer(&TIM[0],SWTIMER_MODE_EMPTY,0);   		// Удаляем таймер синхронизации памяти
		//OnSwTimer(&TIM[0],SWTIMER_MODE_SINGLE,4000);   	// Добавляем таймер одиничного импульса 
		// TIM[2].On = 1;									// Запускаем таймер
		// TIM[2].Off = 0; 

		//MemorySyncTimeoutTimer.resetTimerCounter();
		//MemorySyncTimeoutTimer.setOnePulseModeState(ENABLE);
		//MemorySyncTimeoutTimer.setCounterLimit(4000);
		//MemorySyncTimeoutTimer.setInterruptHandlerAddress((uint32_t)&memorySyncInterruptHandler);
		//MemorySyncTimeoutTimer.startTimer();
	}
	return Param;
}

bool ParamSys::getDeviceParametersDataChangeFlag()
{
	return this->DeviceConstParametersDataChangeFlag;
}

void ParamSys::resetDeviceParametersDataChangeFlag()
{
	DeviceConstParametersDataChangeFlag = 0;
}


void ParamSys::returnToFuctorySettings()
{
	this->clearRAM();
	//flashErasePage(APP_FLASH_PAGE_30);
	//flashErasePage(APP_PARAM_FLASH_PAGE_31);
	//flashErasePage(DEVICE_PARAM_FLASH_PAGE_28);
	//flashErasePage(DEVICE_PARAM_FLASH_PAGE_29);
	//flashErasePage(APP_FLASH_PAGE_31);
	//flashErasePage(APP_PARAM_FLASH_PAGE_28);
	//flashErasePage(APP_PARAM_FLASH_PAGE_29);
	for(int i = 0; i < 24; i++)
	{
		GeneralSystemParameters_0xF0.factoryProjName[i] = ' ';
		GeneralSystemParameters_0xF0.AppProgramName[i] = ' ';
	}
	GeneralSystemParameters_0xF0.AppProgramName[0] = 'К';
	GeneralSystemParameters_0xF0.AppProgramName[1] = '-';
	GeneralSystemParameters_0xF0.AppProgramName[2] = '1';
	GeneralSystemParameters_0xF0.AppProgramName[3] = '0';
	GeneralSystemParameters_0xF0.AppProgramName[4] = '4';
	GeneralSystemParameters_0xF0.AppProgramName[5] = 'М';


	setParameter(0xF000, 0x1105);	
	setParameter(0xF001, 0x0000);
	setParameter(0xF002, FIRMWARE_VER);
	setParameter(0xF040, 0x0000);
	setParameter(0xF024, 0x02);
	setParameter(0xF025, 0xFFFFFFFF); 	//Пароли нужно исправить
	setParameter(0xF026, 0xFFFFFFFF); 	//Пароль разраба нужно расчитать отдельно (пока FF)
	setParameter(0xF00C, FUNC_LIST_LENGHT);
	setParameter(0xF00D, FLEX_PARAM_TABLE_LENGHT / 2);
	setParameter(0xF00E, FLEX_PARAM_DESCR_TABLE_LENGHT);


	setParameter(0x000F, 50);			// Время ожидания следующего байта по RS232
	setParameter(0x0010, 500);			// Время начала ответа по RS232
	setParameter(0x0012, 150);			// Время ожидания по CAN
	setParameter(0x0014, 0);			// Скорость по Ethernet 0 auto
	setParameter(0x0015, 0x02);			// UDP/TCP (2/1),  2 - UDP 
	setParameter(0x000C, 0xCAF0);		// Port RS232  (51952)
	setParameter(0x000D, 0xCAF8);		// Port CAN    (51960)
	setParameter(0x000E, 0x16);			// Port Telnet (22)
	//setParameter(0x0005, RS232_Settings.BaudRate);

	setParameter(0x0009, 0x3700A8C0);      // IP адрес (192.168.0.55)
	setParameter(0x000B, 0x00ffffff);      // Маска подсети (255.255.255.0)
	setParameter(0x000A, 0x0100A8C0);      // Шлюз (192.168.0.1)

	setParameter(0x0300, 0);				// 
	setParameter(0x0301, 0x3800A8C0);		// (192.168.0.56)
	setParameter(0x0302, 0x3800A8C0);		// 
	setParameter(0x0303, 0x3800A8C0);		// 
	setParameter(0x0304, 0x3800A8C0);		// 
	setParameter(0x0305, 0x3800A8C0);		// 
	setParameter(0x0306, 0x3800A8C0);		// 
	setParameter(0x0307, 0x3800A8C0);		// 
	setParameter(0x0308, 0x3800A8C0);		// 
	setParameter(0x0309, 0x3800A8C0);		// 
	setParameter(0x030A, 0x3800A8C0);		// 
	
}


 
