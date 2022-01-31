#ifndef INIT_HPP
#define INIT_HPP

/*-----------------------------------------*/
//     Defines for indicate leds K104
/*-----------------------------------------*/

#define LED_PORT  MDR_PORTE
#define LED_CANTX PORT_Pin_3
#define LED_CANRX PORT_Pin_6
#define LED_RSTX  PORT_Pin_2
#define LED_RSRX  PORT_Pin_7


/*-----------------------------------------*/
//    Defines for SPI1 connected to W5500
/*-----------------------------------------*/
#define SPI1_PORT MDR_PORTF
#define SPI1_MOSI PORT_Pin_0
#define SPI1_SCLK PORT_Pin_1
#define SPI1_SCS  PORT_Pin_2
#define SPI1_MISO PORT_Pin_3
#define SPI1_RST_W5500 PORT_Pin_1

/*-----------------------------------------*/
//    Defines for switches KL1 KL2
/*-----------------------------------------*/
#define KL1 PORT_Pin_5
#define KL2 PORT_Pin_4

#ifdef __cplusplus
extern "C"{
#endif


#include "MDR32Fx.h"
#include "MDR32F9Qx_it.h"
#include "MDR32F9Qx_port.h"
#include "MDR32F9Qx_rst_clk.h"
#include "MDR32F9Qx_timer.h"
#include "MDR32F9Qx_ssp.h"
#include "MDR32F9Qx_uart.h"
#include "MDR32F9Qx_can.h"
#include <stdbool.h>

#ifdef __cplusplus
}
#endif


typedef struct
{
    uint16_t BaudRate; //= 0xFD00;
    bool StopBits; //= 0;
    bool EnParity; //= 0;
    bool BitParity; //= 0;
}UARTSettings;

void init_GPIO_CAN(void);
void init_GPIO_UART(void);
void SetUARTSettings(UARTSettings settings);
void sendByte(uint16_t Data);
void sendBuf(uint8_t* buf, uint32_t size);


void init_clk(void);
void init_leds();
void init_SPI(void);
void write_LED(uint16_t PORT_Pin, FunctionalState state);
void init_Timers(MDR_TIMER_TypeDef* periphealTimer);
void init_switches();

/* Function for W5500 --------------------------*/
void InitW5500(void);
void TCP_Connection(void);
void W5500_Select(void);
void W5500_Unselect(void);
void W5500_ReadBuff(uint8_t* buff, uint16_t len);
void W5500_WriteBuff(uint8_t* buff, uint16_t len);
uint8_t W5500_ReadByte(void);
void W5500_WriteByte(uint8_t byte);

/* Task Receive w5500 --------------------------*/
void vReceiveW5500(void *pvParameters);


#endif