#ifndef TIMERS_HPP
#define TIMERS_HPP


#ifdef __cplusplus
extern "C"{
#endif

//Include C libs//

#include "MDR32Fx.h"
#include "MDR32F9Qx_rst_clk.h"
#include "MDR32F9Qx_timer.h"

#ifdef __cplusplus
}
#endif

class Timer
{
private:
    MDR_TIMER_TypeDef* periphealTimer;
    uint32_t interruptHandlerAddress = 0;
    uint8_t busyFlag = 0;
public:
    Timer(MDR_TIMER_TypeDef* timer);
    uint32_t initTimer();
    void setTimer(MDR_TIMER_TypeDef* timer);
    uint32_t setOnePulseModeState(uint8_t state);
    uint32_t startTimer();
    uint32_t setCounterLimit(uint32_t limit_us);
    uint32_t getCounterLimit();
    uint32_t resetTimerCounter();
    uint32_t setInterruptHandlerAddress(uint32_t handler);
    uint32_t getInterruptHandlerAddress();
    uint32_t checkBusy();
    void stopTimer();
    uint32_t SysTickTimerInit();

    ~Timer()
    {

    }
};

//extern Timer MemorySyncTimeoutTimer;
extern Timer hardwareTimer;

#endif