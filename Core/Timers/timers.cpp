#include "timers.hpp"

Timers::Timers(uint32_t value)
{
    this->testValue = value;
};

uint32_t Timers::ReturnValue(void)
{
    return this->testValue;
}