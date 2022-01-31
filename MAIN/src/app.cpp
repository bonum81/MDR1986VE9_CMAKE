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
#include <MDR32F9Qx_eeprom.h>
#include "ParamSys.hpp"
#include "can.hpp"
#include "app.hpp"
#include "init.hpp"
/* Private define ------------------------------------------------------------*/


/* Private variables ---------------------------------------------------------*/
UARTSettings RS232_Set;
uint32_t test = 0;
uint32_t* ptest;
/* Tasks FreeRTOS -----------------------------------------------*/
static void vBlink_0_Task( void * );



int main()
{
    init_clk();
    init_leds();
    init_GPIO_CAN();
    init_GPIO_UART();
    init_SPI();
    // EEPROM_SetLatency(EEPROM_Latency_1);
    // EEPROM_ErasePage(0x0801E000, EEPROM_Main_Bank_Select);
    // EEPROM_ProgramWord(0x0801E000, EEPROM_Main_Bank_Select, 0x11223344);
    // test = EEPROM_ReadWord(0x0801E000, EEPROM_Main_Bank_Select);

    write_LED(LED_CANTX, DISABLE);
    write_LED(LED_CANRX, DISABLE);
    write_LED(LED_RSTX, DISABLE);
    write_LED(LED_RSRX, DISABLE);

    GeneralSettingsCAN.ID = 0x01;
    GeneralSettingsCAN.baudRate = 0x41E0;
    canSettingsHandler();

    RS232_Set.BaudRate = 0xFD00; 
    RS232_Set.EnParity = 0;
    RS232_Set.BitParity = 0;
    RS232_Set.StopBits = 1;
    SetUARTSettings(RS232_Set);

    InitW5500();
    TCP_Connection();


    ParametersSystem.startParametersSystem();

    xTaskCreate( vBlink_0_Task, "Blink_0", 50, ( void * ) 1, tskIDLE_PRIORITY+1, ( TaskHandle_t * ) 1);
    xTaskCreate( vReceiveW5500, "ReceiveW5500", 50, (void *)1, tskIDLE_PRIORITY+2,( TaskHandle_t * ) 1);
    vTaskStartScheduler();  
    
    while (1)
    {   

    }
    
}

static void vBlink_0_Task( void *pvParameters)
{
    for( ;; )
    {
    write_LED(LED_CANTX, DISABLE);
    write_LED(LED_CANRX, DISABLE);
    write_LED(LED_RSTX, DISABLE);
    write_LED(LED_RSRX, DISABLE);

    vTaskDelay( 100 );
    }
}







