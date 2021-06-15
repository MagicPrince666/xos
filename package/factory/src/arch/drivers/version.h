#ifndef __VERSION_H__
#define __VERSION_H__

#include <inttypes.h>

class Version
{
public:
    Version();
    ~Version();
    int get(uint8_t* bufin, uint8_t* bufout);
    int set(uint8_t* bufin, uint8_t* bufout);
    uint8_t *msg = NULL;

private:
    FILE* input = NULL;
    int dd2file(void);
    int file2dd(uint32_t* bufout);
};

#endif
