# STM32 demo user space
```
//采用模拟SPI通信
/*DI->PB12；
  DO->PB13；
  CS->PB14；
  CLK->PB15
*/
void PS2_Init(void) { 
// 输入 DI->PB12
 RCC->APB2ENR|=1<<3; // 使能 PORTB 时钟 
 GPIOB->CRH&=0XFFF0FFFF;//PB12 设置成输入 默认下拉
 GPIOB->CRH|=0X00080000; 
 // DO->PB13 CS->PB14 CLK->PB15 
 RCC->APB2ENR|=1<<3; // 使能 PORTB 时钟 
 GPIOB->CRH&=0X000FFFFF;
 GPIOB->CRH|=0X33300000; //PB13、 PB14 、 PB15 推挽输出 
  } 
  //端口初始化，PB12 为输入，PB13 、PB14 、PB15 为输出。
  
// 向手柄发送命令
void PS2_Cmd(u8CMD) 
{
  volatile u16 ref=0x01; 
  Data[1]=0; 
  for(ref=0x01;ref<0x0100;ref<<=1) 
  {
    if(ref&CMD) 
    {
     DO_H; // 输出一位控制位
    }
    else DO_L; 
    CLK_H; // 时钟拉高 
    delay_us(10); 
    CLK_L; 
    delay_us(10); 
    CLK_H; 
    if(DI) 
    {Data[1]=ref|Data[1];}
  }
 delay_us(16);
}

// 判断是否为红灯模式，0x41=模拟绿灯，0x73=模拟红灯 
// 返回值；0，红灯模式 
// 其他，其他模式
u8PS2_RedLight(void) 
{
  CS_L; 
  PS2_Cmd(Comd[0]); // 开始命令
  PS2_Cmd(Comd[1]); // 请求数据
  CS_H; 
  if( Data[1]== 0X73) return 0；
  else return 1；
}

// 读取手柄数据 
void PS2_ReadData(void) 
{
  volatile u8 byte=0; 
  volatile u16 ref=0x01; 
  CS_L; 
  PS2_Cmd(Comd[0]); // 开始命令
  PS2_Cmd(Comd[1]); // 请求数据
  for(byte=2;byte<9;byte++) // 开始接受数据 
  {
    for(ref=0x01;ref<0x100;ref<<=1) 
    { 
      CLK_H; 
      delay_us(10); 
      CLK_L; 
      delay_us(10); 
      CLK_H; 
      if(DI) 
      {Data[byte]= ref|Data[byte];}
    }
    delay_us(16);
  }
  CS_H;
}   

/*
上面两个函数分别为主机向手柄发送数据、手柄向主机发送数据。手柄向主机发送的数据缓存在数组 Data[]中，
数组中共有9个元素，每个元素的意义请见表1。
还有一个函数是用来判断手柄的发送模式，也就是判断 ID(红灯还是绿灯模式) 即 Data[1]的值。
*/


// 对读出来的 PS2 的数据进行处理,只处理按键部分
//按下为0，未按下为1
u8PS2_DataKey() 
{
  u8 index; 
  PS2_ClearData(); 
  PS2_ReadData(); 
  Handkey=(Data[4]<<8)|Data[3]; // 这是 16个按键 按下为 0 ， 未按下为 1 
  for(index=0;index<16;index++) 
  {
    if((Handkey&(1<<(MASK[index]-1)))==0) 
    returnindex+1;
  }
  return 0; // 没有任何按键按下
}

// 得到一个摇杆的模拟量 范围 0~256 
u8PS2_AnologData(u8 button) 
{
  return Data[button];
}


// 清除数据缓冲区
void PS2_ClearData() 
{
  u8 a; 
  for(a=0;a<9;a++) 
    {Data[a]=0x00;}
}

/*
8 位数 Data[3]与 Data[4]，分别对应着 16个按键的状态，按下为 0，未按下为 1。
通过 对这两个数的处理，得到按键状态并返回键值。
另一个函数的功能就是返回模拟值，只有在“红灯模式”下值才是有效的，拨动摇杆， 值才会变化，这些值分别存储在 Data[5]、Data[6]、
Data[7]、 Data[8]。 
*/


//手柄配置初始化： 
void PS2_ShortPoll(void) 
{
  CS_L; 
  delay_us(16); 
  PS2_Cmd(0x01); 
  PS2_Cmd(0x42); 
  PS2_Cmd(0X00); 
  PS2_Cmd(0x00); 
  PS2_Cmd(0x00); 
  CS_H; 
  delay_us(16);
}


//进入配置
void PS2_EnterConfing(void) 
{
  CS_L;
  delay_us(16); 
  PS2_Cmd(0x01); 
  PS2_Cmd(0x43); 
  PS2_Cmd(0X00); 
  PS2_Cmd(0x01); 
  PS2_Cmd(0x00); 
  PS2_Cmd(0X00);
  PS2_Cmd(0X00); 
  PS2_Cmd(0X00); 
  PS2_Cmd(0X00); 
  CS_H; 
  delay_us(16);
}


// 发送模式设置 
void PS2_TurnOnAnalogMode(void) 
{
  CS_L; 
  PS2_Cmd(0x01); 
  PS2_Cmd(0x44); 
  PS2_Cmd(0X00); 
  PS2_Cmd(0x01);//analog=0x01;digital=0x00 软件设置发送模式 
  PS2_Cmd(0xEE);//Ox03 锁存设置，即不可通过按键“MODE ”设置模式。        //0xEE 不锁存软件设置，可通过按键“MODE ”设置模式。 
  PS2_Cmd(0X00); 
  PS2_Cmd(0X00); 
  PS2_Cmd(0X00); 
  PS2_Cmd(0X00); 
  CS_H; 
  delay_us(16);
}


// 振动设置
void PS2_VibrationMode(void) 
{
  CS_L; 
  delay_us(16); 
  PS2_Cmd(0x01); 
  PS2_Cmd(0x4D); 
  PS2_Cmd(0X00); 
  PS2_Cmd(0x00); 
  PS2_Cmd(0X01); 
  CS_H; 
  delay_us(16);
}


// 完成并保存配置
void PS2_ExitConfing(void)
{
  CS_L;
  delay_us(16);
  PS2_Cmd(0x01);
  PS2_Cmd(0x43); 
  PS2_Cmd(0X00);
  PS2_Cmd(0x00); 
  PS2_Cmd(0x5A); 
  PS2_Cmd(0x5A); 
  PS2_Cmd(0x5A);
  PS2_Cmd(0x5A); 
  PS2_Cmd(0x5A); 
  CS_H; 
  delay_us(16);
}

// 手柄配置初始化
void PS2_SetInit(void) 
{
  PS2_ShortPoll();
  PS2_ShortPoll();
  PS2_ShortPoll(); 
  PS2_EnterConfing(); // 进入配置模式 
  PS2_TurnOnAnalogMode(); // “红绿灯”配置模式，并选择是否保存 
  PS2_VibrationMode(); // 开启震动模式 
  PS2_ExitConfing(); // 完成并保存配置
}
/*
可以看出配置函数就是发送命令，发送这些命令后，手柄就会明白自己要做什么了，发送命令时，不需要考虑手柄发来的信息。
手柄配置初始化，PS2_ShortPoll()被执行了3次，主要是为了建立和恢复连接。
具体的配置方式请看注释。
*/
void PS2_Vibration(u8motor1,u8motor2)
{
   CS_L; 
   delay_us(16); 
   PS2_Cmd(0x01); // 开始命令
   PS2_Cmd(0x42);// 请求数据
   PS2_Cmd(0X00);
   PS2_Cmd(motor1);
   PS2_Cmd(motor2); 
   PS2_Cmd(0X00); 
   PS2_Cmd(0X00); 
   PS2_Cmd(0X00); 
   PS2_Cmd(0X00); 
   CS_H; 
   delay_us(16);
} 
//只 有 在 初 始 化 函 数 void PS2_SetInit(void) 中 ， 对 震 动 电 机 进 行 了初 始 化 （PS2_VibrationMode();//开启震动模式），这个函数命令才会被执行。 

```