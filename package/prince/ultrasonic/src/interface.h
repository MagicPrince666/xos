#pragma once

class Interface
{
public:
    virtual ~Interface() {}
    virtual void Transfer(int num) = 0;
};
