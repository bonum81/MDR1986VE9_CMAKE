#include "init.hpp"


void init_leds(){
    // Структура для инициализации портов
    PORT_InitTypeDef gpio_led_port;

    // Разрешить тактирование требуемых периферийных устройств
    RST_CLK_PCLKcmd (RST_CLK_PCLK_PORTE, ENABLE);

    //Выставить настройки структуры в дефолтное состояние
    PORT_StructInit (&gpio_led_port);

    gpio_led_port.PORT_Pin   = LED_RSTX | LED_RSRX | LED_CANTX | LED_CANRX | SPI1_RST_W5500;
    //gpio_led_port.PORT_Pin   = LED_CANTX;
    gpio_led_port.PORT_MODE  = PORT_MODE_DIGITAL;
    gpio_led_port.PORT_FUNC  = PORT_FUNC_PORT;
    gpio_led_port.PORT_OE    = PORT_OE_OUT;
    gpio_led_port.PORT_SPEED = PORT_SPEED_FAST;
    PORT_Init(LED_PORT, &gpio_led_port); 

}


void init_clk(void)
{

    RST_CLK_HSEconfig(RST_CLK_HSE_ON);                  // Включить тактирование от внешнего кварца 8 Мгц
    if (RST_CLK_HSEstatus() != SUCCESS)                 // Ждем пока не запустится
    {
        /* Trap */
        while (1)
        {
        }
    }
    /* CPU_C1_SEL = HSE */
    RST_CLK_CPU_PLLconfig(RST_CLK_CPU_PLLsrcHSEdiv1, RST_CLK_CPU_PLLmul6);  // Предделитель 1, множитеть PLL=6. Системная частота 8МГц * 6 = 48МГц
    RST_CLK_CPU_PLLcmd(ENABLE);
    if (RST_CLK_CPU_PLLstatus() != SUCCESS)                                 // Ждем старт PLL
    {
        /* Trap */
        while (1)
        {
        }
    }
    /* CPU_C3_SEL = CPU_C2_SEL */
    RST_CLK_CPUclkPrescaler(RST_CLK_CPUclkDIV1); 
    /* CPU_C2_SEL = PLL */
    RST_CLK_CPU_PLLuse(ENABLE);
    /* HCLK_SEL = CPU_C3_SEL */
    RST_CLK_CPUclkSelection(RST_CLK_CPUclkCPU_C3);
}


void write_LED(uint16_t PORT_Pin, FunctionalState state)
{
    if(state)
    {
        PORT_ResetBits(LED_PORT, PORT_Pin);
    }
    else
    {
        PORT_SetBits(LED_PORT, PORT_Pin);
    }
}


void init_Timers(MDR_TIMER_TypeDef* periphealTimer)
{
    if (periphealTimer == MDR_TIMER1)
    {
        MDR_RST_CLK->PER_CLOCK     |= 0x00004000;   // Enable tim1 clock
        MDR_RST_CLK->TIM_CLOCK     |= (1<<24);         // 48Mhz / 128 = 375kHz
        MDR_RST_CLK->TIM_CLOCK     |= 0x07;         // 48Mhz / 128 = 375kHz

        periphealTimer->CNTRL       = 0x00000000;
        periphealTimer->CNT         = 0x00000000;
        periphealTimer->PSG         = 375-1;         // 750kHz / 75 = 10kHz
        periphealTimer->ARR         = 1000-1;      // 
        periphealTimer->IE          = 0x00000002;
            //MDR_RST_CLK->TIM_CLOCK     |= 0x01000006;   // 48Mhz / 4 = 750kHz
        TIMER_ITConfig(MDR_TIMER1, TIMER_STATUS_CNT_ARR, ENABLE);
        NVIC_EnableIRQ(TIMER1_IRQn);
    }
    else if (periphealTimer == MDR_TIMER2)
    {
        MDR_RST_CLK->PER_CLOCK     |= 0x00008000;   // Enable tim2 clock
        MDR_RST_CLK->TIM_CLOCK     |= (1<<25);         // 48Mhz / 128 = 375kHz
        MDR_RST_CLK->TIM_CLOCK     |= (0x07)<<8;         // 48Mhz / 128 = 375kHz
            

        periphealTimer->STATUS      = 0x0;
        periphealTimer->CNTRL       = 0x00000000;
        periphealTimer->CNT         = 0x00000000;
        periphealTimer->PSG         = 375-1;         // 750kHz / 75 = 10kHz
        periphealTimer->ARR         = 1000-1;      // 
        periphealTimer->IE          = 0x00000002;
            
        //MDR_RST_CLK->TIM_CLOCK     |= 0x02000006;   // 48Mhz / 4 = 750kHz
        TIMER_ITConfig(MDR_TIMER2, TIMER_STATUS_CNT_ARR, ENABLE);
        NVIC_EnableIRQ(TIMER2_IRQn);
    }
    else if (periphealTimer == MDR_TIMER3)
    {
        MDR_RST_CLK->PER_CLOCK     |= 0x00010000;         // Разрешение тактирования
        MDR_RST_CLK->TIM_CLOCK     |= (1<<26);           // Разрешение тактовой частоты на таймер
        MDR_RST_CLK->TIM_CLOCK     |= (0x07)<<16;         // 48Mhz / 128 = 375kHz

        periphealTimer->CNTRL       = 0x0;
        periphealTimer->CNT         = 0x00000000;
        periphealTimer->PSG         = 375-1;         // 750kHz / 75 = 10kHz
        periphealTimer->ARR         = 200-1;      // 
        periphealTimer->IE          = 0x00000002;
        //MDR_RST_CLK->TIM_CLOCK     |= 0x04000006;   // 48Mhz / 4 = 750kHz
        TIMER_ITConfig(MDR_TIMER3, TIMER_STATUS_CNT_ARR, ENABLE);
        NVIC_EnableIRQ(TIMER3_IRQn);
        //MDR_TIMER3->CNTRL |=(1 << 0);
    }

}


void init_SPI(void)
{
    SSP_InitTypeDef sSSP;
	PORT_InitTypeDef PORT_InitStructure;

    RST_CLK_PCLKcmd(RST_CLK_PCLK_PORTF, ENABLE);
    RST_CLK_PCLKcmd((RST_CLK_PCLK_RST_CLK | RST_CLK_PCLK_SSP1),ENABLE);

    PORT_DeInit(SPI1_PORT);
    PORT_StructInit(&PORT_InitStructure);

    PORT_InitStructure.PORT_FUNC = PORT_FUNC_ALTER; 
	PORT_InitStructure.PORT_MODE = PORT_MODE_DIGITAL;
	PORT_InitStructure.PORT_SPEED = PORT_SPEED_MAXFAST;
    PORT_InitStructure.PORT_OE = PORT_OE_OUT;
	PORT_InitStructure.PORT_Pin = (SPI1_MOSI | SPI1_SCLK );
	PORT_Init(SPI1_PORT, &PORT_InitStructure);
	
    PORT_InitStructure.PORT_OE = PORT_OE_IN;
    PORT_InitStructure.PORT_Pin = (SPI1_MISO);
	PORT_Init(SPI1_PORT, &PORT_InitStructure);

    PORT_InitStructure.PORT_FUNC = PORT_FUNC_PORT;
    PORT_InitStructure.PORT_MODE = PORT_MODE_DIGITAL;
	PORT_InitStructure.PORT_SPEED = PORT_SPEED_MAXFAST;
    PORT_InitStructure.PORT_OE = PORT_OE_OUT;
    PORT_InitStructure.PORT_Pin = (SPI1_SCS);
    PORT_Init(SPI1_PORT, &PORT_InitStructure);

	// Reset all SSP settings
	SSP_DeInit(MDR_SSP1);
	SSP_BRGInit(MDR_SSP1, SSP_HCLKdiv1); // 48 Mhz / 1 = 48 Mhz

	// SSP2 MASTER configuration ------------------------------------------------
	SSP_StructInit(&sSSP);
	sSSP.SSP_SPH = SSP_SPH_1Edge;
	sSSP.SSP_SPO = SSP_SPO_Low;
	sSSP.SSP_SCR = 0;  //
	sSSP.SSP_CPSDVSR = 2; //    48 Mhz / 2 = 24 Mhz (this diviner multiple of 2)
	sSSP.SSP_Mode = SSP_ModeMaster;
	sSSP.SSP_WordLength = SSP_WordLength8b;
	sSSP.SSP_FRF = SSP_FRF_SPI_Motorola;
	sSSP.SSP_HardwareFlowControl = SSP_HardwareFlowControl_SSE; //

	SSP_Init(MDR_SSP1, &sSSP);
	PORT_SetBits(SPI1_PORT, SPI1_SCS);

	SSP_Cmd(MDR_SSP1, ENABLE);
}

void init_GPIO_UART(void)
{
    PORT_InitTypeDef PORT_InitUART;
    RST_CLK_PCLKcmd(RST_CLK_PCLK_PORTD,ENABLE);
	
	PORT_DeInit(MDR_PORTD);
	/* Fill PortInit structure*/
    PORT_InitUART.PORT_PULL_UP = PORT_PULL_UP_OFF;
    PORT_InitUART.PORT_PULL_DOWN = PORT_PULL_DOWN_ON;
    PORT_InitUART.PORT_PD_SHM = PORT_PD_SHM_OFF;
    PORT_InitUART.PORT_PD = PORT_PD_DRIVER;
    PORT_InitUART.PORT_GFEN = PORT_GFEN_OFF;
    PORT_InitUART.PORT_FUNC = PORT_FUNC_ALTER;
    PORT_InitUART.PORT_SPEED = PORT_SPEED_MAXFAST;
    PORT_InitUART.PORT_MODE = PORT_MODE_DIGITAL;
	
    /* Configure PORTD pins 0 (UART_RX) as input */
    PORT_InitUART.PORT_OE = PORT_OE_IN;
    PORT_InitUART.PORT_Pin = PORT_Pin_0;
    PORT_Init(MDR_PORTD, &PORT_InitUART);
		
    /* Configure PORTD pins 1 (UART_TX) as output */
    PORT_InitUART.PORT_OE = PORT_OE_OUT;
    PORT_InitUART.PORT_Pin = PORT_Pin_1;
    PORT_Init(MDR_PORTD, &PORT_InitUART);
	
    RST_CLK_PCLKcmd(RST_CLK_PCLK_UART2, ENABLE);

}



void init_GPIO_CAN(void)
{
    PORT_InitTypeDef PORT_InitCAN;
    PORT_InitTypeDef switches_port;


    RST_CLK_PCLKcmd(RST_CLK_PCLK_PORTA, ENABLE);
	
	PORT_DeInit(MDR_PORTA);
	

    /* Configure PIN for Switches KL1 and KL2 */

    switches_port.PORT_Pin = KL1 | KL2;
    switches_port.PORT_MODE = PORT_MODE_DIGITAL;
    switches_port.PORT_FUNC = PORT_FUNC_PORT;
    switches_port.PORT_OE   = PORT_OE_IN;
    switches_port.PORT_PULL_DOWN = PORT_PULL_DOWN_ON;
    switches_port.PORT_SPEED = PORT_SPEED_FAST;

    PORT_Init(MDR_PORTA, &switches_port);



	/* Fill PortInit structure*/
    PORT_InitCAN.PORT_PULL_UP = PORT_PULL_UP_OFF;
    PORT_InitCAN.PORT_PULL_DOWN = PORT_PULL_DOWN_OFF;
    PORT_InitCAN.PORT_PD_SHM = PORT_PD_SHM_OFF;
    PORT_InitCAN.PORT_PD = PORT_PD_DRIVER;
    PORT_InitCAN.PORT_GFEN = PORT_GFEN_OFF;
    PORT_InitCAN.PORT_FUNC = PORT_FUNC_ALTER;
    PORT_InitCAN.PORT_SPEED = PORT_SPEED_MAXFAST;
    PORT_InitCAN.PORT_MODE = PORT_MODE_DIGITAL;
	
    /* Configure PORTA pins 7 (CAN2_RX) as input */
    PORT_InitCAN.PORT_OE = PORT_OE_IN;
    PORT_InitCAN.PORT_Pin = PORT_Pin_7;
    PORT_Init(MDR_PORTA, &PORT_InitCAN);
	
    /* Configure PORTA pins 6 (CAN2_TX) as output */
    PORT_InitCAN.PORT_OE = PORT_OE_OUT;
    PORT_InitCAN.PORT_Pin = PORT_Pin_6;
    PORT_Init(MDR_PORTA, &PORT_InitCAN);
	
	
	//MDR_RST_CLK->PER_CLOCK |= (1UL << 0);
	RST_CLK_PCLKcmd(RST_CLK_PCLK_CAN1, ENABLE);    // Clock to periph CAN
		
	CAN_BRGInit(MDR_CAN1, CAN_HCLKdiv1);		    // Diviner to periph CAN=0, HCLK = 48MHz

}


void SetUARTSettings(UARTSettings settings)
{
	switch (settings.BaudRate)
	{
	    case 0xE800:
			MDR_UART2->IBRD = 156;      // 1200
			MDR_UART2->FBRD = 16;
		    break;
		case 0xF400:
			MDR_UART2->IBRD = 78;		// 2400
			MDR_UART2->FBRD = 8;
			break;
		case 0xFA00:
			MDR_UART2->IBRD = 39;		// 4800
			MDR_UART2->FBRD = 4;
			break;
		case 0xFD00:
			MDR_UART2->IBRD = 19;		// 9600
			MDR_UART2->FBRD = 34;
			break;
		case 0xFD80:
			MDR_UART2->IBRD = 9;		// 19200
			MDR_UART2->FBRD = 49;
			break;
		case 0xFF80:
			MDR_UART2->IBRD = 3;		// 57600
			MDR_UART2->FBRD = 16;
			break;
		case 0xFF84:
			MDR_UART2->IBRD = 1;		// 115200
			MDR_UART2->FBRD = 40;
			break;
		default:
			MDR_UART2->IBRD = 19;		// 9600
			MDR_UART2->FBRD = 34;
			break;
	}
	if (settings.EnParity)
	{
		MDR_UART2->LCR_H |= (1 << 1);  				// Parity on.
	}
	else
	{
		MDR_UART2->LCR_H &=~ (1 << 1);  			// Parity off.
	}

	if (settings.BitParity)
	{
		MDR_UART2->LCR_H |= (1 << 2);  				// Дополнение до четного
	}
	else
	{
		MDR_UART2->LCR_H |= (1 << 2);  				// Дополнение до нечетного
	}

	if (settings.StopBits)
	{
		MDR_UART2->LCR_H &=~ (1 << 3); 			// 1 stop bit
	}
	else
	{
		MDR_UART2->LCR_H |= (1 << 3); 			// 2 stop bit
	}


	MDR_UART2->LCR_H |= (3 << 5);			// 8 Data bits		
	UART_BRGInit(MDR_UART2, UART_HCLKdiv16);	
	NVIC_EnableIRQ(UART2_IRQn);	

	MDR_UART2->LCR_H |= (1 << 4);     //FIFO On
	//MDR_UART2->LCR_H &=~ (1 << 4);     //FIFO Off
		
	MDR_UART2->CR = 0;
	MDR_UART2->CR = ((1 << 8)|(1 << 9)|1); 
		
	MDR_UART2-> IFLS = ((0 << 3) | (0 << 4) | (0 << 5));		//FIFO rx interrupt 1/8 FIFO, (2 byte)
	MDR_UART2-> IMSC = ((1 << 6) | (1 << 4) | (1 << 10));
    /* Enable Receiver interrupt */
    UART_ITConfig (MDR_UART2, UART_IT_RX, ENABLE);
	UART_ITConfig (MDR_UART2, UART_IT_RT, ENABLE);    // iterrupt timeout rx
    /* Enables UART2 peripheral */
    UART_Cmd(MDR_UART2,ENABLE);
}

void sendByte(uint16_t Data)
{
	while (UART_GetFlagStatus (MDR_UART2, UART_FLAG_TXFE)!= SET){};
	MDR_UART2->DR = (Data & (uint16_t)0x0FF);
	write_LED(LED_RSTX, ENABLE);
}

void sendBuf(uint8_t* buf, uint8_t size)
{
	uint32_t i = 0;
		
	for (i = 0; i < size; i++)
	{
		while (UART_GetFlagStatus (MDR_UART2, UART_FLAG_TXFE)!= SET){};
		UART_SendData(MDR_UART2, buf[i]);
		write_LED(LED_RSTX, ENABLE);
	}
}




    void UART2_IRQHandler(void)
    {
        //write_LED(LED_RSRX, ENABLE);
        if (UART_GetITStatusMasked(MDR_UART2, UART_IT_RT) == SET)			// interrupt timeout
        {
            // receiveBUF[receiveCounter] = MDR_UART2->DR;
            // receiveCounter++;
            // FT_receive_handler(receiveBUF, &receiveCounter);
            // receiveCounter = 0;
        }
            
        if (UART_GetITStatusMasked(MDR_UART2, UART_IT_RX) == SET)
        {
            // receiveBUF[receiveCounter] = MDR_UART2->DR;
            // receiveCounter++;
        }
    }

