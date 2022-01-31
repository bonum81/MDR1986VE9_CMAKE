#include "init.hpp"
#include <w5500.h>
#include <socket.h>
#include <stdio.h>
#include <FreeRTOS.h>
#include <task.h>

#define TCP_SOCKET              0       // Номер сокета
#define TCP_PORT                51960   // Номер порта соединения

uint32_t len = 0;
uint8_t stat = 0;                       // Статус созданного соединения
uint8_t ReceiveData[1024];
char DebugBuf[64];                      // Буфер для дебага по UART


wiz_NetInfo gWIZNETINFO = { .mac = {0x8A, 0xBE, 0x44, 0x54, 0xED, 0x1C},        // Сетевые настройки
                            .ip = {192, 168, 0, 89},
                            .sn = {255, 255, 255, 0},
                            .gw = {192, 168, 0, 254},
                            .dns = {192, 168, 0, 160},
                            .dhcp = NETINFO_STATIC };

void vReceiveW5500 (void *pvParameters)
{
    for(;;)
    {
        if (getSn_RX_RSR(TCP_SOCKET) != 0x0000)    // если входной буфер не пуст
        {
            len = getSn_RX_RSR(TCP_SOCKET);    // получение длинны данных в буфере
            // recv(0, ReceiveData, len);
            // FT_receive_handler(ReceiveData, len);
            // sendBuf((uint8_t*)ReceiveData, len);
            if (len > 255)                      
            {
                while (len > 255)
                {
                    recv(0, ReceiveData, 255);
                    len = len - 255;
                }
            }

            recv(0, ReceiveData, len);
            //FT_receive_handler(ReceiveData, len);
            sendBuf((uint8_t*)ReceiveData, len);
        }
        vTaskDelay( 100 );
    }
} 


void InitW5500(void)
{
    PORT_ResetBits(MDR_PORTE, SPI1_RST_W5500);
    //delay(1);
    PORT_SetBits(MDR_PORTE, SPI1_RST_W5500);
    //delay(1000);
    
    reg_wizchip_cs_cbfunc(W5500_Select, W5500_Unselect);            // Установка ф-ций для Chip select
    reg_wizchip_spi_cbfunc(W5500_ReadByte, W5500_WriteByte);        // Установка ф-ции чтения записи байта по SPI
    reg_wizchip_spiburst_cbfunc(W5500_ReadBuff, W5500_WriteBuff);   // Установка ф-ции чтения записи буфера по SPI

    uint8_t rx_tx_buff_sizes[] = {2, 2, 2, 2, 2, 2, 2, 2};           // Установка размеров входных буферов
  
    wizchip_init(rx_tx_buff_sizes, rx_tx_buff_sizes);
	
	ctlnetwork(CN_SET_NETINFO, (void*) &gWIZNETINFO);
    
    //delay(1000);
}


void TCP_Connection(void)
{
    stat = socket(0, Sn_MR_TCP, TCP_PORT, SF_TCP_NODELAY);
    if (stat != 0)
    {
            sprintf(DebugBuf, "socket() failed, code = %d\r\n", stat);
            sendBuf((uint8_t*)DebugBuf, 30);
    }
    else
    {
            sprintf(DebugBuf, "Socket created, connecting...\r\n");
            sendBuf((uint8_t*)DebugBuf, 32);
    }

    stat = listen(TCP_SOCKET);

    if(stat != SOCK_OK)
    {
        sprintf(DebugBuf, "listen() failed, code = %d\r\n", stat);
        sendBuf((uint8_t*)DebugBuf, 26);
    } 
    else 
    {
        sprintf(DebugBuf, "listen() OK\r\n");
        sendBuf((uint8_t*)DebugBuf, 13);
    }
    
}


void W5500_Select(void)
{
    while (SSP_GetFlagStatus(MDR_SSP1, SSP_FLAG_BSY) == SET);

    PORT_ResetBits(SPI1_PORT, SPI1_SCS);
    
}

void W5500_Unselect(void) 
{
    while (SSP_GetFlagStatus(MDR_SSP1, SSP_FLAG_BSY) == SET);

    PORT_SetBits(SPI1_PORT, SPI1_SCS);
}

void W5500_ReadBuff(uint8_t* buff, uint16_t len) 
{
    
    for (uint32_t numBytes = 0; numBytes < len; numBytes++)
    {
        MDR_SSP1->DR = 0x00;
        while (SSP_GetFlagStatus(MDR_SSP1, SSP_FLAG_BSY ) == SET);
        buff[numBytes] = MDR_SSP1->DR;
    }

    return;
    
}

void W5500_WriteBuff(uint8_t* buff, uint16_t len) 
{
    uint16_t trash = 0;
    for (uint32_t numBytes = 0; numBytes < len; numBytes++)
    {
        while (SSP_GetFlagStatus(MDR_SSP1, SSP_FLAG_TFE ) == RESET);
        SSP_SendData(MDR_SSP1, buff[numBytes]);
        
        while (SSP_GetFlagStatus(MDR_SSP1, SSP_FLAG_RNE ) == RESET);
        trash = MDR_SSP1->DR;
    }

    return;

}

uint8_t W5500_ReadByte(void) 
{
    uint8_t byte;
    W5500_ReadBuff(&byte, sizeof(byte));
    return byte;
}

void W5500_WriteByte(uint8_t byte) 
{
    W5500_WriteBuff(&byte, sizeof(byte));
}
