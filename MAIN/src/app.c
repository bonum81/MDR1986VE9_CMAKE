#include "app.h"
#include "init.h"
#include "can.h"
#include <stdio.h>
#include <string.h>
#include "ft_protocol.h"

#if defined(__cplusplus)
extern "C"{
#endif  

#include "socket.h"

#if defined(__cplusplus)
}
#endif

#define HTTP_SOCKET     0
#define PORT_TCPS		5000
#define DATA_BUF_SIZE   2048

volatile uint32_t ticks_delay = 0;

uint8_t gDATABUF[DATA_BUF_SIZE];

// wiz_NetInfo gWIZNETINFO = { .mac = {0x00, 0x08, 0xdc, 0xab, 0xcd, 0xef},
//                             .ip = {192, 168, 88, 200},
//                             .sn = {255, 255, 255, 0},
//                             .gw = {192, 168, 88, 254},
//                             .dns = {8, 8, 8, 8},
//                             .dhcp = NETINFO_STATIC };

wiz_NetInfo gWIZNETINFO = { .mac = {0x00, 0x08, 0xdc, 0xab, 0xcd, 0xef},
                            .ip = {192, 168, 0, 89},
                            .sn = {255, 255, 255, 0},
                            .gw = {192, 168, 0, 254},
                            .dns = {8, 8, 8, 8},
                            .dhcp = NETINFO_STATIC };


uint8_t testIP[4];

uint8_t stat;
uint8_t reqnr;
char Message[128];


void SysTickTimerInit();
void delay(const uint32_t milliseconds);

void W5500_Select(void);
void W5500_Unselect(void);
void W5500_ReadBuff(uint8_t* buff, uint16_t len);
void W5500_WriteBuff(uint8_t* buff, uint16_t len);
uint8_t W5500_ReadByte(void);
void W5500_WriteByte(uint8_t byte);
void Working_W5500();
void TCP_Connection();


uint8_t ReceiveData[128];

UARTSettings RS232_Set;

char DebugBuf[64];




int main()
{
    
    init_clk();
    init_leds();
    init_GPIO_UART();
    init_GPIO_CAN();
    init_SPI();
    
    SysTickTimerInit();
    init_Timers(MDR_TIMER3);
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

    PORT_ResetBits(MDR_PORTE, SPI1_RST_W5500);
    delay(1);
    PORT_SetBits(MDR_PORTE, SPI1_RST_W5500);
    delay(1000);
    
    reg_wizchip_cs_cbfunc(W5500_Select, W5500_Unselect);
    reg_wizchip_spi_cbfunc(W5500_ReadByte, W5500_WriteByte);
    reg_wizchip_spiburst_cbfunc(W5500_ReadBuff, W5500_WriteBuff);

    uint8_t rx_tx_buff_sizes[] = {2, 2, 2, 2, 2, 2, 2, 2};
  
    wizchip_init(rx_tx_buff_sizes, rx_tx_buff_sizes);
	
    //wizchip_setnetinfo(&gWIZNETINFO);
	
	ctlnetwork(CN_SET_NETINFO, (void*) &gWIZNETINFO);

    delay(1000);

    TCP_Connection();

    while (1)
    {   
        //Working_W5500();

        // if (getSn_SR(HTTP_SOCKET) == SOCK_LISTEN)
        // {
        //     sprintf(DebugBuf, "listen...\r\n");
        //     sendBuf((uint8_t*)DebugBuf, 13);
        // }
        // if (getSn_SR(HTTP_SOCKET) == SOCK_ESTABLISHED)
        // {
        //     //sprintf(DebugBuf, "Establiched.\r\n");
        //     //sendBuf((uint8_t*)DebugBuf, 13);
            
        // }
        // if (getSn_SR(HTTP_SOCKET) == SOCK_CLOSE_WAIT)
        // {
        //     close(HTTP_SOCKET);
        //     sprintf(DebugBuf, "Closed.\r\n");
        //     sendBuf((uint8_t*)DebugBuf, 13);
        //     TCP_Connection();
        // }
        // if (getSn_RX_RSR(HTTP_SOCKET) != 0x0000)
        //     {
        //         uint16_t Len = getSn_RX_RSR(HTTP_SOCKET);
        //         recv(0, ReceiveData, 128);
        //         send(0, ReceiveData, strlen((const char *)ReceiveData));
        //         sendBuf((uint8_t*)ReceiveData, Len);
        //     }
        
        // delay(100);
    }
    
}


void TCP_Connection()
{
    stat = socket(0, Sn_MR_TCP, 51090, SF_TCP_NODELAY);
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

    stat = listen(HTTP_SOCKET);
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

uint32_t ControlConnect(uint8_t socketNum)
{
    
    switch (getSn_SR(socketNum))
    {
    case SOCK_ESTABLISHED:

        return 0;
        
        break;

    case SOCK_CLOSE_WAIT:
        close(socketNum);
        return 0;

        break;

    case SOCK_CLOSED:
        TCP_Connection();
        break;



    default:
        break;
    }



    return 0;
}

void Working_W5500()
{
        sprintf(DebugBuf, "Closing socket.\r\n");
        sendBuf((uint8_t*)DebugBuf, 17);
        stat = socket(HTTP_SOCKET, Sn_MR_TCP, 80, 0);
        if(stat != HTTP_SOCKET)
        {
            sprintf(DebugBuf, "socket() failed, code = %d\r\n", stat);
            sendBuf((uint8_t*)DebugBuf, 30);
        } 
        else
        {
            sprintf(DebugBuf, "Socket created, connecting...\r\n");
            sendBuf((uint8_t*)DebugBuf, 32);
        } 

        stat = listen(HTTP_SOCKET);
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

        while(getSn_SR(HTTP_SOCKET) == SOCK_LISTEN)
        {
             delay(2);
        }


        sprintf(DebugBuf, "Input connection\r\n");
        sendBuf((uint8_t*)DebugBuf, 19);
        if(getSn_SR(HTTP_SOCKET) != SOCK_ESTABLISHED)
        {
            sprintf(DebugBuf, "Error socket status\r\n");
            sendBuf((uint8_t*)DebugBuf, 22);
        } 
    
        uint8_t rIP[4];
        getsockopt(HTTP_SOCKET, SO_DESTIP, rIP);
        sprintf(DebugBuf, "IP:  %d.%d.%d.%d\r\n", rIP[0], rIP[1], rIP[2], rIP[3]);
        sendBuf((uint8_t*)DebugBuf, 19);
        //printf("IP:  %d.%d.%d.%d\r\n", rIP[0], rIP[1], rIP[2], rIP[3]);
            
        sprintf(Message, "input connection nr - %d", reqnr);
            
        //send(0, (uint8_t*)Message, strlen(Message));
        
        recv(0, ReceiveData, 10);
        //disconnect(HTTP_SOCKET);
        
        // sprintf(DebugBuf, "Closing socket.\r\n");
        // sendBuf((uint8_t*)DebugBuf, 17);
        // close(HTTP_SOCKET);
                    
        delay(100);
        reqnr++;
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





#if defined(__cplusplus)
extern "C"{
#endif  

void SysTick_Handler(void)
{
	ticks_delay++;
    write_LED(LED_RSTX, DISABLE);
    write_LED(LED_RSRX, DISABLE);
    write_LED(LED_CANTX, DISABLE);
    write_LED(LED_CANRX, DISABLE);
}

void Timer3_IRQHandler(void)
{
    MDR_TIMER3->STATUS &= ~0x002; //IE FLAG = 0
    
    ControlConnect(0);

    if (getSn_RX_RSR(HTTP_SOCKET) != 0x0000)
        {
            uint16_t Len = getSn_RX_RSR(HTTP_SOCKET);
            recv(0, ReceiveData, 128);
            //send(0, ReceiveData, strlen((const char *)ReceiveData));
            FT_receive_handler(ReceiveData, Len);
            sendBuf((uint8_t*)ReceiveData, Len);
        }


}


#if defined(__cplusplus)
}
#endif


