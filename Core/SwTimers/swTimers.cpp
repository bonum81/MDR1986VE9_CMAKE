#include "swTimers.hpp"
#include <deque>
#include <algorithm>
#include "timers.hpp"
#include "init.hpp"

using namespace std;

static deque <SwTimer*> timersQueue;
void hardwareTimerInterruptHandler();



//========================== Стандартный программный таймер ============================//

SwTimer::SwTimer(uint32_t countLimit_ms, bool *timerCompliteFlag, bool continuousMode)  //<! Таймер с флагом завершения
{
    this->type = 0;
    this->counterLimit = countLimit_ms;
    this->counter = countLimit_ms;
    this->compliteFlag = timerCompliteFlag;
    *this->compliteFlag = 0;
    this->timerContinuousMode = continuousMode;
    this->workInProgress = false;
    timersQueue.push_back(this);
}

SwTimer::SwTimer(uint32_t countLimit_ms, void *timerHandler, bool continuousMode)  //<! Таймер с обработчиком
{
    this->type = 1;
    this->counterLimit = countLimit_ms;
    this->counter = countLimit_ms;
    this->handler = (void(*)(void))timerHandler;
    this->timerContinuousMode = continuousMode;
    this->workInProgress = false;
    //this->compliteFlag = nullptr;
    timersQueue.push_back(this);
}

SwTimer::SwTimer(uint32_t pwmPeriod, uint32_t pulseDuration, bool *outputStateFlag)
{
    this->type = 2;
    this->swTimPeriod = pwmPeriod;
    this->pwmPulseDuration = pulseDuration;
    this->counterLimit = pwmPeriod - pulseDuration;
    this->counter = this->counterLimit;
    this->pwmOutputFlag = outputStateFlag;
    this->workInProgress = false;
    //this->compliteFlag = nullptr;
    timersQueue.push_back(this);
}

SwTimer::SwTimer(uint32_t countLimit_ms, uint8_t *outputStateByte, uint8_t flagBitNumber, bool continuousMode)
{
    this->type = 3;
    this->counterLimit = countLimit_ms;
    this->counter = countLimit_ms;
    this->OutputFlagByte = outputStateByte;
    this->flagBitNum = flagBitNumber;
    //this->compliteFlag = nullptr;
    this->timerContinuousMode = continuousMode;
    *this->OutputFlagByte &= ~(0x1 << flagBitNum);
    this->workInProgress = false;
    timersQueue.push_back(this);
}

SwTimer::SwTimer(uint32_t pwmPeriod, uint32_t pulseDuration, uint8_t *outputStateByte, uint8_t flagBitNumber)
{
    this->type = 4;
    this->swTimPeriod = pwmPeriod;
    this->pwmPulseDuration = pulseDuration;
    this->counterLimit = pwmPeriod - pulseDuration;
    this->counter = this->counterLimit;
    this->OutputFlagByte = outputStateByte;
    this->flagBitNum = flagBitNumber;
    //this->compliteFlag = nullptr;
    this->workInProgress = false;
    timersQueue.push_back(this);
}

SwTimer::~SwTimer()
{
    stopTimer();
    deque <SwTimer*>::iterator subSwTimIterator;
    subSwTimIterator = find(timersQueue.begin(), timersQueue.end(), this);
    if(subSwTimIterator != timersQueue.end())
    {
        timersQueue.erase(subSwTimIterator);
    }
}

int32_t SwTimer::getCounter()
{
    int32_t upCounter = counterLimit - counter;
    if(upCounter > (int32_t)counterLimit)
    {
        upCounter = counterLimit;
    }else if(upCounter < 0)
    {
        upCounter = 0;
    }
    return upCounter;
}

void SwTimer::setActiveCompliteLevel(bool level)
{
    this->activeLevel = level;
    *this->compliteFlag = !level;
}

void SwTimer::startTimer()
{
    if(this->type == 0)
    {
        if((this->compliteFlag != 0x0) && ((uint32_t)this->compliteFlag != (0xFFFFFFFF)))
        {
            *this->compliteFlag = !activeLevel;            
        }
    }
    this->workInProgress = true;
    hardwareTimerInterruptHandler();
}

void SwTimer::reloadTimer()
{
    this->counter = this->counterLimit;
    if(this->type == 0)
    {
        if((this->compliteFlag != 0x0) && ((uint32_t)this->compliteFlag != (0xFFFFFFFF)))
        {
            *this->compliteFlag = !activeLevel;            
        }
    }
    this->workInProgress = true;
}

void SwTimer::stopTimer()
{
    this->workInProgress = false;
}

bool SwTimer::updateTimer()
{
    switch (type)
    {
        case 0:
            *this->compliteFlag = activeLevel;
            if(timerContinuousMode == false)
            {
                this->workInProgress = false;
            }else{
                reloadTimer();
                this->workInProgress = true;
            }
            return !timerContinuousMode;
            break;
        case 1:
            if(timerContinuousMode == false)
            {
                this->workInProgress = false;
            }else{
                reloadTimer();
                this->workInProgress = true;
            }
            if(((uint32_t)&handler != 0x0) && ((uint32_t)&handler != 0xFFFFFFFF))
            {
                this->handler();
            }
            return !timerContinuousMode;
            break;
        case 2:
            if(*this->pwmOutputFlag == false)
            {
                this->counterLimit = this->pwmPulseDuration;
                this->counter = this->pwmPulseDuration;
                *this->pwmOutputFlag = true;
            }else
            {
                this->counterLimit =  swTimPeriod - pwmPulseDuration;
                this->counter = counterLimit;
                *this->pwmOutputFlag = false;
            }
            return 0;
            break;
        case 3:
            if(activeLevel == true)
            {
                *this->OutputFlagByte |= (0x1 << flagBitNum);
            }else{
                *this->OutputFlagByte &= ~(0x1 << flagBitNum);
            }
            this->workInProgress = false;
            return !timerContinuousMode;
            break;    
        case 4:
            uint8_t outState = ((*this->OutputFlagByte >> flagBitNum) & 0x1);
            if(outState != 0)
            {
                this->counterLimit =  swTimPeriod - pwmPulseDuration;
                this->counter = counterLimit;
                if ((swTimPeriod - pwmPulseDuration != 0) || swTimPeriod == 0)
                {
                    *this->OutputFlagByte &= ~(0x1 << flagBitNum);
                }

            }else           
            {
                this->counterLimit = this->pwmPulseDuration;
                this->counter = this->pwmPulseDuration;
                if (counterLimit > 0)
                {
                    *this->OutputFlagByte |= (0x1 << flagBitNum);
                }

            }
            return 0;
            break;
    }
    return 1;
}
//===============================================================================================//

bool hardTimerEvent = 0;
void hardwareTimerInterruptHandler()
{
    deque <SwTimer*>::iterator iter = timersQueue.begin();;
    uint32_t time = 0;
    if(hardTimerEvent == 1)
    {
        time = hardwareTimer.getCounterLimit();
    }else
    {
        time = (uint32_t)(MDR_TIMER2->CNT);
    }
    hardwareTimer.stopTimer();
    hardTimerEvent = 0;
    uint32_t size = timersQueue.size();
    if(size == 0)
    {
        return;
    }
    uint32_t minTime = 0xFFFFFFFF;
	for(iter = timersQueue.begin(); iter < timersQueue.end(); iter++)
	{
        if(iter.operator*()->workInProgress == false)
        {
            continue;
        }
        if(iter.operator*()->counter <= time)
        {
            iter.operator*()->counter = 0;
        }
        else
        {
            iter.operator*()->counter = iter.operator*()->counter - time;
        }
		if(iter.operator*()->counter <= 0)
		{
           iter.operator*()->updateTimer(); //TODO: �������� ��������
		}
        if((iter.operator*()->counter <= minTime) && (iter.operator*()->counter != 0))
		{
			minTime = iter.operator*()->counter;
		}
	}
    if(minTime == 0xFFFFFFFF)
    {
        hardwareTimer.resetTimerCounter();
        return;
    }
    if(minTime >= 32760)
        minTime = 32760;
    hardwareTimer.resetTimerCounter();
    hardwareTimer.setCounterLimit(minTime);
    hardwareTimer.startTimer();
}

#if defined(__cplusplus)
extern "C"{
#endif  

void Timer2_IRQHandler()
{
    MDR_TIMER2->STATUS &= ~0x002; //IE FLAG=0
    hardTimerEvent = true;
    hardwareTimerInterruptHandler();
}

#if defined(__cplusplus)
}
#endif 