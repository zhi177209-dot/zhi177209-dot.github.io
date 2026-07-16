#include "command.h"

// 指令的最小长度
#define COMMAND_MIN_LENGTH 4

//协议设置
#define FRAME_HEAD1 0xAA
#define PACK_BUF_MAX 50

// 循环缓冲区大小
#define BUFFER_SIZE 128
// 循环缓冲区
uint8_t buffer[BUFFER_SIZE];
// 循环缓冲区读索引
volatile uint8_t readIndex = 0;
// 循环缓冲区写索引
volatile uint8_t writeIndex = 0;


/**
* @brief 增加读索引
* @param length 要增加的长度
*/
void Command_AddReadIndex(uint8_t length) {
    readIndex += length;
    readIndex %= BUFFER_SIZE;
}

/**
* @brief 读取第i位数据 超过缓存区长度自动循环
* @param i 要读取的数据索引
*/

uint8_t Command_Read(uint8_t i) {
    uint8_t index = i % BUFFER_SIZE;
    return buffer[index];
}

/**
* @brief 计算未处理的数据长度
* @return 未处理的数据长度
* @retval 0 缓冲区为空
* @retval 1~BUFFER_SIZE-1 未处理的数据长度
* @retval BUFFER_SIZE 缓冲区已满
*/
//uint8_t Command_GetLength() {
//  // 读索引等于写索引时，缓冲区为空
//  if (readIndex == writeIndex) {
//    return 0;
//  }
//  // 如果缓冲区已满,返回BUFFER_SIZE
//  if (writeIndex + 1 == readIndex || (writeIndex == BUFFER_SIZE - 1 && readIndex == 0)) {
//    return BUFFER_SIZE;
//  }
//  // 如果缓冲区未满,返回未处理的数据长度
//  if (readIndex <= writeIndex) {
//    return writeIndex - readIndex;
//  } else {
//    return BUFFER_SIZE - readIndex + writeIndex;
//  }
//}

uint8_t Command_GetLength() {
    return (writeIndex + BUFFER_SIZE - readIndex) % BUFFER_SIZE;
}


/**
* @brief 计算缓冲区剩余空间
* @return 剩余空间
* @retval 0 缓冲区已满
* @retval 1~BUFFER_SIZE-1 剩余空间
* @retval BUFFER_SIZE 缓冲区为空
*/
uint8_t Command_GetRemain() {
    return BUFFER_SIZE - Command_GetLength();
}

/**
 * @brief 协议封包：帧头+总长+业务数据+累加校验
 * @param raw_data 原始业务数据
 * @param raw_len 原始数据字节长度
 * @param pack_buf 输出：封包完成的完整帧缓存
 * @return 完整帧总长度，0=封包失败（数据过长）
 */
uint8_t Chuli_Data(uint8_t *raw_data, uint8_t raw_len, uint8_t *pack_buf)
{
    if (raw_len + 3 > PACK_BUF_MAX)
        return 0;

    uint8_t index = 0;
    // 帧头
    pack_buf[index++] = FRAME_HEAD1;
    // 整帧总长 = 帧头1 + 长度1 + 数据N + 校验1
    uint8_t total_len = raw_len + 3;
    pack_buf[index++] = total_len;
    // 拷贝原始业务数据
    memcpy(pack_buf + index, raw_data, raw_len);
    index += raw_len;
    // 计算校验：帧头+长度+业务数据
    uint8_t sum = 0;
    for(uint8_t i = 0; i < index; i++)
        sum += pack_buf[i];
    // 写入校验字节
    pack_buf[index++] = sum;

    return total_len;
}



/**
* @brief 向缓冲区写入数据
* @param data 要写入的数据指针
* @param length 要写入的数据长度
* @return 写入的数据长度
*/

uint8_t Command_Write(uint8_t *data, uint8_t length) {
    // 如果缓冲区不足 则不写入数据 返回0
    if (Command_GetRemain() < length) {
        return 0;
    }
	
    // 使用memcpy函数将数据写入缓冲区
    if (writeIndex + length <= BUFFER_SIZE) {
        memcpy(buffer + writeIndex, data, length);
        writeIndex += length;
        writeIndex %= BUFFER_SIZE;
    } else {
        uint8_t firstLength = BUFFER_SIZE - writeIndex;
        memcpy(buffer + writeIndex, data, firstLength);
        memcpy(buffer, data + firstLength, length - firstLength);
        writeIndex = length - firstLength;
    }
    return length;
}



/**
* @brief 协议封包并写入环形发送缓冲区
* @param raw_data 原始业务数据指针
* @param raw_len 原始业务数据长度
* @return 成功写入的完整帧长度，0=失败
*/

//uint8_t Command_Write(uint8_t *raw_data, uint8_t raw_len)
//{
//    uint8_t pack_buf[PACK_BUF_MAX];
//    // 1. 先封包，得到完整帧
//    uint8_t pack_len = Chuli_Data(raw_data, raw_len, pack_buf);
//    if(pack_len == 0)
//        return 0; // 封包失败，数据过长

//    // 2. 校验缓冲区剩余空间（用封包后的真实长度判断）
//    uint16_t remain = Command_GetRemain();
//    if (remain < pack_len)
//        return 0;

//    // 3. 环形缓冲区写入完整帧 pack_buf
//    if (writeIndex + pack_len <= BUFFER_SIZE)
//    {
//        memcpy(buffer + writeIndex, pack_buf, pack_len);
//        writeIndex += pack_len;
//    }
//    else
//    {
//        // 尾部剩余空间
//        uint16_t tail_space = BUFFER_SIZE - writeIndex;
//        memcpy(buffer + writeIndex, pack_buf, tail_space);
//        // 头部写入剩余数据
//        memcpy(buffer, pack_buf + tail_space, pack_len - tail_space);
//        writeIndex = pack_len - tail_space;
//    }
//    return pack_len;
//}


/**
* @brief 尝试获取一条指令
* @param command 指令存放指针
* @return 获取的指令长度
* @retval 0 没有获取到指令
*/
//uint8_t Command_GetCommand(uint8_t *command) {
//    // 寻找完整指令
//    while (1) {
//        // 如果缓冲区长度小于COMMAND_MIN_LENGTH 则不可能有完整的指令
//        if (Command_GetLength() < COMMAND_MIN_LENGTH) {
//        return 0;
//        }
//        // 如果不是包头 则跳过 重新开始寻找
//        if (Command_Read(readIndex) != 0xAA) {
//        Command_AddReadIndex(1);
//        continue;
//        }
//        // 如果缓冲区长度小于指令长度 则不可能有完整的指令
//        uint8_t length = Command_Read(readIndex + 1);
//        if (Command_GetLength() < length) {
//        return 0;
//        }
//        // 如果校验和不正确 则跳过 重新开始寻找
//        uint8_t sum = 0;
//        for (uint8_t i = 0; i < length - 1; i++) {
//        sum += Command_Read(readIndex + i);
//        }
//        if (sum != Command_Read(readIndex + length - 1)) {
//        Command_AddReadIndex(1);
//        continue;
//        }
//				/*
//        // 如果找到完整指令 则将指令写入command 返回指令长度
//        for (uint8_t i = 0; i < length; i++) {
//        command[i] = Command_Read(readIndex + i);
//        }
//				*/
//				// 如果找到完整指令 则只提取中间业务数据写入command
//				uint8_t data_len = length - 3;
//				for (uint8_t i = 0; i < data_len; i++)
//				{
//					command[i] = Command_Read(readIndex + 2 + i);
//				}

//        Command_AddReadIndex(length);
//        return length-3;
//    }
//}

#define COMMAND_TIMEOUT_MS 1000

CommandStatus_t Command_GetCommand(uint8_t *command, uint8_t *commandLength)
{
    static uint32_t frameStartTick = 0;
    uint8_t bufferLength;

    *commandLength = 0;

    while (1)
    {
        bufferLength = Command_GetLength();

        if (bufferLength == 0)
        {
            frameStartTick = 0;
            return CMD_NONE;
        }

        if (Command_Read(readIndex) != FRAME_HEAD1)
        {
            Command_AddReadIndex(1);
            frameStartTick = 0;
            return CMD_ERR_HEAD;
        }

        if (frameStartTick == 0)
        {
            frameStartTick = HAL_GetTick();
        }

        if (bufferLength < COMMAND_MIN_LENGTH)
        {
            if (HAL_GetTick() - frameStartTick > COMMAND_TIMEOUT_MS)
            {
                Command_AddReadIndex(1);
                frameStartTick = 0;
                return CMD_ERR_TIMEOUT;
            }

            return CMD_NONE;
        }

        uint8_t length = Command_Read(readIndex + 1);

        if (length < COMMAND_MIN_LENGTH || length > PACK_BUF_MAX)
        {
            Command_AddReadIndex(1);
            frameStartTick = 0;
            return CMD_ERR_LENGTH;
        }

        if (bufferLength < length)
        {
            if (frameStartTick == 0)
            {
                frameStartTick = HAL_GetTick();
            }

            if (HAL_GetTick() - frameStartTick > COMMAND_TIMEOUT_MS)
            {
                Command_AddReadIndex(1);
                frameStartTick = 0;
                return CMD_ERR_TIMEOUT;
            }

            return CMD_NONE;
        }

        frameStartTick = 0;

        uint8_t sum = 0;
        for (uint8_t i = 0; i < length - 1; i++)
        {
            sum += Command_Read(readIndex + i);
        }

        if (sum != Command_Read(readIndex + length - 1))
        {
            Command_AddReadIndex(length);
            frameStartTick = 0;
            return CMD_ERR_CHECKSUM;
        }

        uint8_t data_len = length - 3;

        for (uint8_t i = 0; i < data_len; i++)
        {
            command[i] = Command_Read(readIndex + 2 + i);
        }

        Command_AddReadIndex(length);

        *commandLength = data_len;
        return CMD_OK;
    }
}