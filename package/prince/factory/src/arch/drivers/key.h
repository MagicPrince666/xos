#ifndef __KEY_H__
#define __KEY_H__

class Key
{
public:
    Key();
    ~Key();
    int keyprocess(uint8_t* bufout, bool sta);
    int key_input_fd = -1;

private:

};

#endif
