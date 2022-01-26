#ifndef SW_TIMERS_HPP
#define SW_TIMERS_HPP



#ifdef __cplusplus
extern "C"{
#endif

#include "MDR32Fx.h"

#ifdef __cplusplus
}
#endif


class SwTimer
{
private:
    uint32_t timerID = 0;               
    bool     *pwmOutputFlag = 0;
    bool     timerContinuousMode = 0;
    uint8_t  *OutputFlagByte = 0;
    uint8_t  flagBitNum = 0;
    bool     activeLevel = true;
public:
    uint32_t swTimPeriod = 0;
    uint32_t pwmPulseDuration = 0;
    uint32_t counter = 0;
    uint32_t counterLimit = 0;
    uint8_t  type = 0;
    void     (*handler)(void);
    bool     *compliteFlag = 0;
    bool     workInProgress = 0;
    SwTimer  (uint32_t countLimit_ms, bool *timerCompliteFlag, bool continuousMode);
    SwTimer  (uint32_t countLimit_ms, void *timerHandler, bool continuousMode);                       
    SwTimer  (uint32_t pwmPeriod, uint32_t pulseDuration, bool *outputStateFlag); //<! Генератор ШИМ
    SwTimer  (uint32_t countLimit_ms, uint8_t *outputStateByte, uint8_t flagBitNumber, bool continuousMode);
    SwTimer  (uint32_t pwmPeriod, uint32_t pulseDuration, uint8_t *outputStateByte, uint8_t flagBitNumber); //<! Генератор ШИМ
    ~SwTimer ();
    void     setActiveCompliteLevel(bool level);
    void     reloadTimer();
    int32_t  getCounter();
    void     startTimer();
    void     stopTimer();
    bool     updateTimer();
};


#endif