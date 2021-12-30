#include "ft_protocol.h"
#include "init.h"






uint32_t FT_receive_handler(uint8_t *buffer, uint32_t *len)
{
    if((buffer[0] != 0x10) && (buffer[0] != 0x68))
    {
        *len = 0;
        return 0;
    }
    if(*len >= 8)
    {
        if((buffer[3] == 0x01) && (*len == 9))
        {
            //ReadParam(buffer, len, handler);
            //ReadParam(buffer, len);
            write_LED(LED_CANTX, ENABLE);
            return 0;
        }else if((buffer[3] == 0x04) && (*len == 9))
        {
            //syncMemory(buffer, len, handler);
            //syncMemory(buffer, len);
            return 0;
        }else if((buffer[6] == 0x17) && (*len == 10))
        {
            //AccessLevel(buffer, len, handler);
            //AccessLevel(buffer, len);
            return 0;
        }else if((buffer[6] == 0x17) && (*len == 14))
        {
            //AccessLevel(buffer, len, handler);
            //AccessLevel(buffer, len);
            return 0;
        }else if((buffer[6] == 0x05) && (*len >= 10))
        {
            //WriteParam(buffer, len, handler);
            //WriteParam(buffer, len);
            return 0;
        }else if((buffer[6] == 0x15) && (*len >= 13))
        {
            //ReadMassiveParam(buffer, len, handler);
            //ReadMassiveParam(buffer, len);
            return 0;
        }else if((buffer[6] == 0x16) && (*len == 17))
        {
            //WriteMassiveParam(buffer, len, handler);
            //WriteMassiveParam(buffer, len);
            return 0;
        }
    }
    return 0;
}