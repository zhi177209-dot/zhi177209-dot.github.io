#ifndef ERR_H_
#define ERR_H_

#include "main.h"
#include <string.h>
#include "usart.h"


#define ERR_TIMEOUT         0x01
#define ERR_CHECKSUM        0x02
#define ERR_INVALID_HEAD    0x03
#define ERR_INVALID_LENGTH  0x04
#define ERR_UNKNOWN_CMD     0x05
#define ERR_SENSOR_FAIL     0x06

typedef enum
{
    CMD_NONE = 0,          // 没有完整命令
    CMD_OK,                // 解析成功
    CMD_ERR_HEAD,          // 非法帧头
    CMD_ERR_LENGTH,        // 长度非法
    CMD_ERR_CHECKSUM,      // 校验错误
    CMD_ERR_TIMEOUT        // 半包超时
} CommandStatus_t;


void Send_Error(uint8_t err);
void Log_Print(char *msg);


#endif /* INC_COMMAND_H_ */