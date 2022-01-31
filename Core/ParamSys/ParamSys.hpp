#ifndef PARAMSYS_HPP
#define PARAMSYS_HPP
//#include <map>
//#include <vector>

#include "MDR32Fx.h"

#define FUNC_LIST_LENGHT      				128
#define FLEX_PARAM_TABLE_LENGHT				444 * 2
#define FLEX_PARAM_DESCR_TABLE_LENGHT		444


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

#define FIRMWARE_VER 						0x22
#define SECTOR_USED_NUMBER                  4
#define DEVICE_CONST_LENGHT  				8192

#define APP_FLASH_PAGE_28                     0x0801C000
#define APP_PARAM_FLASH_PAGE_29               0x0801D000
#define DEVICE_PARAM_FLASH_PAGE_30            0x0801E000
#define DEVICE_PARAM_FLASH_PAGE_31            0x0801F000

#define LAST_TYPE_PARAMETERS                  0x07          // Группа параметров F0

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

struct ApplicationProgramTask
{
    uint16_t TaskNumber = 0xFFFF;
    uint16_t Offset = 0xFFFF;
};

struct ParamDescript{
    uint32_t *ParametersAddress;
    DescriptionStruct Description;
};

struct 
{
    uint8_t userPassword = 0x02;
    uint32_t adjusterPassword = 0;
    uint32_t developerPassword = 0;
}Passwords;

typedef struct 
{
    uint32_t currentAccessLevel = 1;
}CurState;

typedef struct
{
    uint32_t startAddress;
    uint32_t endAddress;
}MassiveScope;


typedef struct 
{
	bool  EthernetSpeed						= false;		// 0 - auto, 1 - 10 mbit/s
	uint32_t IpAddress    					= 192168055;
	uint32_t IpMask   						= 2552552550;
	uint32_t IpGateway 						= 19216801;
	uint16_t port_RS 						= 0xCAF0;
	uint16_t port_CAN 						= 0xCAF8;
	uint16_t port_telnet 					= 0x16;
	uint16_t waitingTimeNextByteRS232 		= 50; 		// ms
	uint16_t waitingTimeStartResponseRS232 	= 500; 		//ms
	uint16_t responseWaitingTimeCAN 		= 150; 		// ms
	uint8_t  modeTCP_UPD					= 2;		//	2 - UDP, 1 - TCP
	uint8_t  typeOfSlave					= 0xFF;
	uint8_t  MacAddress[6] 					= {0x8A,0xA4,0xBE,0x2E,0x7D,0xA4};		
}settigns_general;

class ParamSys
{
private: 
    ParamSys( const ParamSys&);  
    ParamSys& operator=( ParamSys& );
    ParamSys(){};
    uint32_t erasedSectors[SECTOR_USED_NUMBER];
	int 	 erasedSectorsNumber = 0;
    bool     EventLogDesyncFlag = false;
    bool     AppProgramDataDesyncFlag = false;
    bool     AppProgramParametersDesyncFlag = false;
    bool     DeviceConstParametersDesyncFlag = false;
    bool     DeviceConstParametersDataChangeFlag = false;
	MassiveScope AppProgramFlashData_SCOPE;                             //<! Границы области размещения прикладной программы во flash
    MassiveScope AppProgramFlashData_RAM_BUFFER_SCOPE;                  //<! Границы области размещения прикладной программы в  RAM
    MassiveScope AppProgramParametersFlashData_SCOPE;                   //<! Границы области размещения постоянных параметров прикладной программы во flash
    MassiveScope AppProgramParametersFlashData_RAM_BUFFER_SCOPE;        //<! Границы области размещения постоянных параметров прикладной программы в  RAM
    MassiveScope AppProgramParametersRAM_Data_SCOPE;                    //<! Границы области размещения переменных параметров прикладной программы в  RAM
    MassiveScope DeviceConstParameters_SCOPE;                           //<! Границы области размещения постоянных параметров прибора во flash
    MassiveScope EventLogFlashData_SCOPE;
    MassiveScope EventLogFlashData_RAM_BUFFER_SCOPE;
    void eraseSector(uint32_t sector);
    void clearRAM();
    void copyFromAllFlashToRAM();
    void copyFromAllRAMToFlash();
    void returnToFuctorySettings();
public:
    static ParamSys& getInstance() {
        static ParamSys  instance;
        return instance;
    } 
    void doMemorySync(void);
    void startParametersSystem();
    bool checkingParameter(uint16_t param);
    bool getDeviceParametersDataChangeFlag();
    void resetDeviceParametersDataChangeFlag();
    void setDesynchronizationFlag(uint16_t param);
    ParamDescript getParameter(uint16_t param);
    ParamDescript setParameter(uint16_t param, uint32_t value);                             //
    ParamDescript getMassiveParameter(uint16_t param, uint32_t index);
    ParamDescript setMassiveParameter(uint16_t param, uint32_t index, uint32_t value);
};
 
extern ParamSys& ParametersSystem; 

extern settigns_general GeneralSettingsK104M;


#endif 
