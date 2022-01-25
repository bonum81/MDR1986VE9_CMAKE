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

/* Includes H -----------------------------------------------------------------*/


/* Includes HPP --------------------------------------------------------------*/
#include <FreeRTOS.h>
#include <task.h>
#include "app.hpp"
#include "init.hpp"
#include "timers.hpp"
/* Private define ------------------------------------------------------------*/
#define BUFFER_LENGTH                        100

/* Private variables ---------------------------------------------------------*/




/* Private function prototypes -----------------------------------------------*/

volatile uint32_t ticks_delay;

void SysTickTimerInit();
void delay(const uint32_t milliseconds);
static void vBlink_0_Task( void * );

Timers MyTimer(10);


int main()
{
    init_clk();
    init_leds();
    init_GPIO_CAN();
    xTaskCreate( vBlink_0_Task, "Blink_0", 50, ( void * ) 1, tskIDLE_PRIORITY+1, ( TaskHandle_t * ) 1);
    vTaskStartScheduler();  
    //SysTickTimerInit();
    //testTimer = new Timers(32768);
    uint32_t val = MyTimer.ReturnValue();
    
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

static void vBlink_0_Task( void *pvParameters)
{
    for( ;; )
    {
    PORT_SetBits(LED_PORT, LED_CAN1_TX);
    vTaskDelay( 500 );
    PORT_ResetBits(LED_PORT, LED_CAN1_TX);
    vTaskDelay( 2500 );
    }
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

// void SysTick_Handler(void)
// {
// 	ticks_delay++;
//     // write_LED(LED_RSTX, DISABLE);
//     // write_LED(LED_RSRX, DISABLE);
//     // write_LED(LED_CANTX, DISABLE);
//     // write_LED(LED_CANRX, DISABLE);
// }



#if defined(__cplusplus)
}
#endif


