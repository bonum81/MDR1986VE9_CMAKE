#include "timers.hpp"
#include "init.hpp"
#include <stdlib.h>
//Timer MemorySyncTimeoutTimer(MDR_TIMER1);
Timer hardwareTimer(MDR_TIMER2);


uint32_t (*interruptHandler)(void);


Timer::Timer(MDR_TIMER_TypeDef* timer)
    {
        periphealTimer = timer;
    }

uint32_t Timer::initTimer()
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
uint32_t Timer::startTimer()
    {
        periphealTimer->CNTRL |=(1 << 0);
        busyFlag = 1;
        return 1;
    }
    
void Timer::setTimer(MDR_TIMER_TypeDef* timer)
    {
        periphealTimer = timer;
    }
uint32_t Timer::setCounterLimit(uint32_t limit_us) 
    {
        periphealTimer->ARR = (limit_us) - 1;
        return 1;
    }
uint32_t Timer::getCounterLimit()
    {
        return ((periphealTimer->ARR + 1));
    }

uint32_t Timer::resetTimerCounter()
	{
		periphealTimer->CNT = 0;
		return 1;
	}
uint32_t Timer::setInterruptHandlerAddress(uint32_t handler)
	{
		interruptHandlerAddress = handler;
		return 1;
	}
uint32_t Timer::getInterruptHandlerAddress()
    {
        return interruptHandlerAddress;
    }
uint32_t Timer::checkBusy()
    {
        return busyFlag;
    }
void Timer::stopTimer()
    {
        periphealTimer->CNTRL &= ~(1 << 0);
    }
/*
uint32_t Timer::SysTickTimerInit()
{
    SysTick->LOAD = 48000-1;
    SysTick->CTRL |= SysTick_CTRL_CLKSOURCE_Msk;
    SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk;

    NVIC_EnableIRQ(SysTick_IRQn);
    SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
}
*/
#if defined(__cplusplus)
extern "C"{
#endif  

void Timer1_IRQHandler(void)
{
    MDR_TIMER1->STATUS &= ~0x002; //IE FLAG = 0
    /*
    uint32_t handlerAddress = MemorySyncTimeoutTimer.getInterruptHandlerAddress();
    if(handlerAddress == 0)
    {

        return;
    }else
    {
        interruptHandler = (uint32_t(*)(void))handlerAddress;
        (*interruptHandler)();
    }
    */
}
/*
void Timer2_IRQHandler()
{
    MDR_TIMER2->STATUS &= ~0x002; //IE FLAG=0
    uint32_t handlerAddress = HardwareTimerForSwTimers.getInterruptHandlerAddress();
    if (handlerAddress == 0)
    {
        return;
    }else
    {
        interruptHandler = (uint32_t(*)(void))handlerAddress;
        (*interruptHandler)();
    }


    //hardTimerEvent = true;
    //hardwareTimerInterruptHandler();

}
*/
// void Timer3_IRQHandler(void)
// {
//     MDR_TIMER3->STATUS &= ~0x002; //IE FLAG = 0
//     uint32_t handlerAddress = 0;
//     if (handlerAddress == 0)
//     {
//         return;
//     }else
//     {
//         interruptHandler = (uint32_t(*)(void))handlerAddress;
//         (*interruptHandler)();
//     }
// }

#if defined(__cplusplus)
}
#endif







