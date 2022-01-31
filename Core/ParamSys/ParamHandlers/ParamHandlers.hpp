#ifndef PARAM_HANDLERS_HPP
#define PARAM_HANDLERS_HPP

#include "stdint.h"

class ParamHandlers
{
private:
    ParamHandlers( const ParamHandlers&);  
    ParamHandlers& operator=( ParamHandlers& );
    ParamHandlers(){};
public:
    static ParamHandlers& getInstance() 
    {
        static ParamHandlers  instance;
        return instance;
    }
    void doParameterHandler(uint16_t param);
};

#endif