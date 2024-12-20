#ifndef __PSTWO_H
#define __PSTWO_H

#include <sys/types.h>
#include "spi.h"

//#define PS2_JOYPAD_ACK        gpio_get_value(F1C100S_GPIOA0)
//#define PS2_JOYPAD_DATA       gpio_get_value(F1C100S_GPIOE10)     //E10 MISO
//#define PS2_JOYPAD_CMND_1     write(ps2_cmd,"1",1)      //E8 CLK
//#define PS2_JOYPAD_ATT_1      write(ps2_att,"1",1)      //E7 MOSI
//#define PS2_JOYPAD_CLOCK_1    write(ps2_clk,"1",1)      //E9 CS

//#define PS2_JOYPAD_CMND_0     write(ps2_cmd,"0",1)      //E8
//#define PS2_JOYPAD_ATT_0      write(ps2_att,"0",1)      //E7
//#define PS2_JOYPAD_CLOCK_0    write(ps2_clk,"0",1)      //E9

//These are our button constants
#define PSB_SELECT      1
#define PSB_L3          2
#define PSB_R3          3
#define PSB_START       4
#define PSB_PAD_UP      5
#define PSB_PAD_RIGHT   6
#define PSB_PAD_DOWN    7
#define PSB_PAD_LEFT    8
#define PSB_L2          9
#define PSB_R2          10
#define PSB_L1          11
#define PSB_R1          12
#define PSB_GREEN       13
#define PSB_RED         14
#define PSB_BLUE        15
#define PSB_PINK        16

#define PSB_TRIANGLE    13
#define PSB_CIRCLE      14
#define PSB_CROSS       15
#define PSB_SQUARE      16

//#define WHAMMY_BAR		8

//These are stick values
#define PSS_RX 5                //
#define PSS_RY 6
#define PSS_LX 7
#define PSS_LY 8

class Ps2Remote {
public:
    Ps2Remote();
    ~Ps2Remote();
    uint8_t PS2_DataKey(void);
    uint8_t PS2_AnologData(uint8_t button);
    void PS2_Vibration(uint8_t motor1, uint8_t motor2);
    void PS2_SetInit(void);

private:
    uint8_t PS2_RedLight(void);
    void PS2_ReadData(void);
    void PS2_Cmd(const uint8_t *cmd, int len);
    void PS2_ClearData(void);
    void PS2_EnterConfing(void);
    void PS2_TurnOnAnalogMode(void);
    void PS2_VibrationMode(void);
    void PS2_ExitConfing(void);
    void PS2_ShortPoll(void);

    uint8_t data;
    Spi *spidev;
    uint16_t Handkey;
    uint8_t Comd[2] = {0x80, 0x42};
    uint8_t Data[9] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
};

#endif
