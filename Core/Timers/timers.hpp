#ifndef CAN_H
#define CAN_H

#ifdef __cplusplus
extern "C"{
#endif

#include "MDR32F9Qx_rst_clk.h"
#include "MDR32F9Qx_port.h"
#include "MDR32F9Qx_timer.h"

#ifdef __cplusplus
}
#endif


class Timers
{
private:
    /* data */
    uint32_t testValue;
public:
    Timers(uint32_t value);
    uint32_t ReturnValue(void);
    
};



#endif