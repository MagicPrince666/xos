#pragma once

class Interface
{
public:
    virtual ~Interface() {}
    virtual void Transfer(bool flag) = 0;
};
