/**
  ******************************************************************************
  * @file   app.c
  * @author  Illarionov Maksim
  * @version V1.0.0
  * @date    10/01/2022
  * @brief   Main program body.
  ******************************************************************************
  * <br><br>
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, MILANDR SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2021 Milandr</center></h2>
  */

/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include <string.h>
#include "app.h"
#include "init.h"
#include "MDR32F9Qx_config.h"
#include "MDR32F9Qx_usb_handlers.h"

/* Private define ------------------------------------------------------------*/
#define BUFFER_LENGTH                        100

/* Private variables ---------------------------------------------------------*/
USB_Clock_TypeDef USB_Clock_InitStruct;
USB_DeviceBUSParam_TypeDef USB_DeviceBUSParam;

static uint8_t Buffer[BUFFER_LENGTH];

#ifdef USB_CDC_LINE_CODING_SUPPORTED
    static USB_CDC_LineCoding_TypeDef LineCoding;
#endif /* USB_CDC_LINE_CODING_SUPPORTED */

/* Private function prototypes -----------------------------------------------*/
static void Setup_CPU_Clock(void);
static void Setup_USB(void);
static void VCom_Configuration(void);
volatile uint32_t ticks_delay;

void SysTickTimerInit();
void delay(const uint32_t milliseconds);



int main()
{
    VCom_Configuration();
    USB_CDC_Init(Buffer, 1, SET);
    init_clk();
    Setup_USB();
    init_leds();
    init_GPIO_CAN();
    SysTickTimerInit();

    while (1)
    {   
            // PORT_SetBits(LED_PORT, LED_CAN1_TX);
            // PORT_SetBits(LED_PORT, LED_CAN1_RX);
            // PORT_SetBits(LED_PORT, LED_CAN2_TX);
            // PORT_SetBits(LED_PORT, LED_CAN2_RX);
            // PORT_SetBits(LED_PORT, LED_USB_TX);
            // PORT_SetBits(LED_PORT, LED_USB_RX);
            // delay(500);
            // PORT_ResetBits(LED_PORT, LED_CAN1_TX);
            // PORT_ResetBits(LED_PORT, LED_CAN1_RX);
            // PORT_ResetBits(LED_PORT, LED_CAN2_TX);
            // PORT_ResetBits(LED_PORT, LED_CAN2_RX);
            // PORT_ResetBits(LED_PORT, LED_USB_TX);
            // PORT_ResetBits(LED_PORT, LED_USB_RX);
            // delay(500);  
    }
    
}


void Setup_USB(void)
{
    /* Enables the CPU_CLK clock on USB */
    RST_CLK_PCLKcmd(RST_CLK_PCLK_USB, ENABLE);

    /* Device layer initialization */
    USB_Clock_InitStruct.USB_USBC1_Source = USB_C1HSEdiv1;
    USB_Clock_InitStruct.USB_PLLUSBMUL    = USB_PLLUSBMUL4;

    USB_DeviceBUSParam.MODE  = USB_SC_SCFSP_Full;
    USB_DeviceBUSParam.SPEED = USB_SC_SCFSR_12Mb;
    USB_DeviceBUSParam.PULL  = USB_HSCR_DP_PULLUP_Set;

    USB_DeviceInit(&USB_Clock_InitStruct, &USB_DeviceBUSParam);

    /* Enable all USB interrupts */
    USB_SetSIM(USB_SIS_Msk);

    USB_DevicePowerOn();

    /* Enable interrupt on USB */
#ifdef USB_INT_HANDLE_REQUIRED
    NVIC_EnableIRQ(USB_IRQn);
#endif /* USB_INT_HANDLE_REQUIRED */

    USB_DEVICE_HANDLE_RESET;
}

static void VCom_Configuration(void)
{
#ifdef USB_CDC_LINE_CODING_SUPPORTED
    LineCoding.dwDTERate = 115200;
    LineCoding.bCharFormat = 0;
    LineCoding.bParityType = 0;
    LineCoding.bDataBits = 8;
#endif /* USB_CDC_LINE_CODING_SUPPORTED */
}



USB_Result USB_CDC_RecieveData(uint8_t* Buffer, uint32_t Length)
{
    USB_Result result;
    uint8_t str1[5] = "L1=1\0";
    uint8_t str2[5] = "L1=0\0";

    uint8_t recvStr[5];

    recvStr[0] = Buffer[0];
    recvStr[1] = Buffer[1];
    recvStr[2] = Buffer[2];
    recvStr[3] = Buffer[3];
    recvStr[4] = Buffer[4];
#ifdef USB_DEBUG_PROTO
    ReceivedByteCount += Length;
#endif /* USB_DEBUG_PROTO */

    /* Send back received data portion */
    if ((Buffer[0] == 'L') && (Length == 5))
    {
        if (strcmp(str1, recvStr) == 0)
        {
            PORT_SetBits(LED_PORT, LED_CAN1_TX);
        }
        else if (strcmp(str2, recvStr) == 0)
        {
            PORT_ResetBits(LED_PORT, LED_CAN1_TX);
        }
    }
    /* My Code */

    /*  */
		
#ifdef USB_DEBUG_PROTO
    if (result == USB_SUCCESS)
    {
        SentByteCount += Length;
    }
#ifndef USB_VCOM_SYNC
    else
    {
        SkippedByteCount += Length;
    }
#endif /* !USB_VCOM_SYNC */
#endif /* USB_DEBUG_PROTO */

#ifdef USB_VCOM_SYNC
    if (result != USB_SUCCESS)
    {
        /* If data cannot be sent now, it will await nearest possibility
        * (see USB_CDC_DataSent) */
        PendingDataLength = Length;
    }
    return result;
#else
    return USB_SUCCESS;
#endif /* USB_VCOM_SYNC */
}


USB_Result USB_CDC_GetLineCoding(uint16_t wINDEX, USB_CDC_LineCoding_TypeDef* DATA)
{
    assert_param(DATA);
    if (wINDEX != 0)
    {
        /* Invalid interface */
        return USB_ERR_INV_REQ;
    }

    /* Just store received settings */
    *DATA = LineCoding;
    return USB_SUCCESS;
}


USB_Result USB_CDC_SetLineCoding(uint16_t wINDEX, const USB_CDC_LineCoding_TypeDef* DATA)
{
    assert_param(DATA);
    if (wINDEX != 0)
    {
        /* Invalid interface */
        return USB_ERR_INV_REQ;
    }

    /* Just send back settings stored earlier */
    LineCoding = *DATA;
    return USB_SUCCESS;
}




void SysTickTimerInit()
{
    SysTick->LOAD = 48000-1;
    SysTick->CTRL |= SysTick_CTRL_CLKSOURCE_Msk;
    SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk;

    NVIC_EnableIRQ(SysTick_IRQn);
    SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
}

void delay(const uint32_t milliseconds) 
{

    uint32_t start = ticks_delay;

    while((ticks_delay - start) < milliseconds);

}




#if defined(__cplusplus)
extern "C"{
#endif  

void SysTick_Handler(void)
{
	ticks_delay++;
    // write_LED(LED_RSTX, DISABLE);
    // write_LED(LED_RSRX, DISABLE);
    // write_LED(LED_CANTX, DISABLE);
    // write_LED(LED_CANRX, DISABLE);
}



#if defined(__cplusplus)
}
#endif


