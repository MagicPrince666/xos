#include "joystick.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <iostream>
#include <string>
#include <sstream>
#include "unistd.h"

Joystick::Joystick()
{
  openPath("/dev/input/js0");
}

Joystick::Joystick(int joystickNumber)
{
  std::stringstream sstm;
  sstm << "/dev/input/js" << joystickNumber;
  openPath(sstm.str());
}

Joystick::Joystick(Xepoll *epoll, std::string devicePath)
: epoll_(epoll)
{
  openPath(devicePath);
  if (!isFound())
  {
    printf("open failed.\n");
    exit(1);
  }
  if(_fd > 0) {
    epoll_->add(_fd, std::bind(&Joystick::JoystickRead, this));
  }
}

Joystick::Joystick(std::string devicePath, bool blocking)
{
  openPath(devicePath, blocking);
}

void Joystick::openPath(std::string devicePath, bool blocking)
{
  // Open the device using either blocking or non-blocking
  _fd = open(devicePath.c_str(), blocking ? O_RDONLY : O_RDONLY | O_NONBLOCK);
}

bool Joystick::sample(JoystickEvent* event)
{
  int bytes = read(_fd, event, sizeof(*event)); 

  if (bytes == -1)
    return false;

  // NOTE if this condition is not met, we're probably out of sync and this
  // Joystick instance is likely unusable
  return bytes == sizeof(*event);
}

bool Joystick::isFound()
{
  return _fd >= 0;
}

Joystick::~Joystick()
{
  close(_fd);
}

int Joystick::JoystickRead()
{
  JoystickEvent event;
  if (sample(&event))
  {
    if (event.isButton())
    {
      printf("Button %u is %s\n",
        event.number,
        event.value == 0 ? "up" : "down");
    } else if (event.isAxis())
    {
      printf("Axis %u is at position %d\n", event.number, event.value);
    }
  }
  return 0;
}

std::ostream& operator<<(std::ostream& os, const JoystickEvent& e)
{
  os << "type=" << static_cast<int>(e.type)
     << " number=" << static_cast<int>(e.number)
     << " value=" << static_cast<int>(e.value);
  return os;
}
