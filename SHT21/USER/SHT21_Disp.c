#include "SHT21_Disp.h"
#include "LedDisplay.h"
#include "SHT21.h"

extern unsigned char DisBuf[8];
extern unsigned char Point;

void ShowSHT21(void)
{
    unsigned char tempDis[8] = {DARK,DARK,DARK,DARK,DARK,DARK,DARK,DARK};
    unsigned char i;
    unsigned int humVal, tempVal;
    unsigned char pointMask = 0;
    
    // 写死固定测试值，排除传感器干扰
    humVal = 800;
    tempVal = 300;

    // ========== 左侧 DIG0 DIG1 DIG2 湿度 80.0 ==========
    DisU16(humVal);
    // 修正顺序：DIG0=百位8，DIG1=十位0，DIG2=个位0
    tempDis[0] = DisBuf[2];  // 最左第1位放百位8
    tempDis[1] = DisBuf[1];  // 中间第2位放十位0（带小数点）
    tempDis[2] = DisBuf[0];  // 第3位放个位0
    // DIG1(DisPtr=1)小数点需要bit6置1
    pointMask |= (1 << 6);

    // 中间DIG3、DIG4消隐
    tempDis[3] = DARK;
    tempDis[4] = DARK;

    // ========== 右侧 DIG5 DIG6 DIG7 温度 30.0 ==========
    DisU16(tempVal);
    tempDis[5] = DisBuf[2];  // 第6位放百位3
    tempDis[6] = DisBuf[1];  // 第7位放十位0（带小数点）
    tempDis[7] = DisBuf[0];  // 最右第8位放个位0
    // DIG6(DisPtr=6)小数点需要bit1置1
    pointMask |= (1 << 1);

    // 拷贝到全局缓存
    for(i = 0; i < 8; i++)
    {
        DisBuf[i] = tempDis[i];
    }
    Point = pointMask;
}