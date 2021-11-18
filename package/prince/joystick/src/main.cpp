#include "joystick.h"
#include "xepoll.h"
#include <unistd.h>

int main(int argc, char** argv)
{
  // Create an instance of Joystick
  Xepoll xepoll;//初始化事件模型
  Joystick joystick(&xepoll, "/dev/input/js0");
  return xepoll.loop();//等待事件触发
}
