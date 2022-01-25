#ifndef INIT_HPP
#define INIT_HPP

/*-----------------------------------------*/
//     Defines for indicate leds K104
/*-----------------------------------------*/

#define LED_PORT  MDR_PORTA
#define LED_CAN1_TX PORT_Pin_0
#define LED_CAN1_RX PORT_Pin_1
#define LED_CAN2_TX PORT_Pin_2
#define LED_CAN2_RX PORT_Pin_3
#define LED_USB_TX  PORT_Pin_4
#define LED_USB_RX  PORT_Pin_5


/*-----------------------------------------*/
//    Defines for SPI1 connected to W5500
/*-----------------------------------------*/
#define SPI1_PORT MDR_PORTF
#define SPI1_MOSI PORT_Pin_0
#define SPI1_SCLK PORT_Pin_1
#define SPI1_SCS  PORT_Pin_2
#define SPI1_MISO PORT_Pin_3
#define SPI1_RST_W5500 PORT_Pin_1

#ifdef __cplusplus
extern "C"{
#endif

/*--Include C libs--*/

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

void init_GPIO_CAN(void);
void init_GPIO_UART(void);
void sendByte(uint16_t Data);
void sendBuf(uint8_t* buf, uint8_t size);
void init_clk(void);
void init_leds(void);
void init_SPI(void);
void write_LED(uint16_t PORT_Pin, FunctionalState state);
void init_Timers(MDR_TIMER_TypeDef* periphealTimer);






#endif