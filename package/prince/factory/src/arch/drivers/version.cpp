#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <cstring>
#include "version.h"

#define ETH_MAC     0x0
#define XAG_AREA    0x30
#define XAG_HAL     0x50
#define XAG_VER     0x54
#define XAG_TYPE    0x58
#define XAG_SN      0x88
#define KERNELADDR  0x12
#define HEAD1    0x0C000124
#define HEAD2    0x0C000224
#define KERNEL_VERSION 0x040e2300

#define LENGHT 65536 //一次读出64K字节

#define CBTBLK "/dev/mmcblk2p3"
#define CBTFILE "/tmp/spx4.bin"
//#define CBTBLK "art.bin"

#include <iostream>

Version::Version(void)
{
    std::cout << "surperx4 version" << std::endl;

    dd2file();
    input = fopen(CBTFILE, "r+");
    if(input == NULL) {
        std::cout << "file open error" << std::endl;
        exit(1);
    } else {
        msg = new uint8_t[LENGHT];
        //read board message
        if (0 == fseek(input, 0, SEEK_SET)) {
            int ret = fread(msg, sizeof(uint8_t), LENGHT, input);
            std::cout << "file read size = " << ret << std::endl;
        } else {
            std::cout << "read message error!" << std::endl;
        }
    }
}

Version::~Version(void)
{
    std::cout << "surperx4 version deinit" << std::endl;
    if(input != NULL)
        fclose(input);
    if(msg != NULL)
        delete[] msg;
}

//调用dd命令拷贝信息到文件
//    system("dd if=/dev/mmcblk2p3 of=/tmp/spx4.bin bs=1k count=64");
int Version::dd2file(void)
{
    char massge[128] = {0};
    sprintf(massge, "dd if=%s of=%s bs=1k count=64", CBTBLK, CBTFILE);
    if(system(massge) < 0) std::cout << "system open error" << std::endl;
    std::cout << massge << std::endl;
    sync();
    return 0;
}

//调用dd命令把文件写入到块
int Version::file2dd(uint32_t* bufout)
{
    char massge[128] = {0};

    if (0 == fseek(input, 0, SEEK_SET)) {
        fwrite(bufout, sizeof(uint32_t), LENGHT, input);
    }
    fclose(input);
    input = NULL;

    sprintf(massge, "dd if=%s of=%s bs=1k count=64", CBTFILE, CBTBLK);
    if(system(massge) < 0) std::cout << "system open error" << std::endl;
    std::cout << massge << std::endl;
    sync();
    return 0;
}

int Version::get(uint8_t* bufin, uint8_t* bufout)
{
    int ret = 0;
    uint32_t *tempcmd = (uint32_t*)bufout;
    uint32_t *tempin  = (uint32_t*)bufin;

    std::cout << "get surperx4 version" << std::endl;
    if(*tempin == 0x01000224)
        *tempcmd++ = HEAD2;//add head
    else {
        *tempcmd++ = HEAD1;//add head
    }

    *tempcmd++ = *(uint32_t*)(msg + XAG_HAL);//U32 硬件版本 v1.0.0.1
    *tempcmd++ = *(uint32_t*)(msg + XAG_VER);//U32 固件版本 v1.0.0.1
//    *tempcmd++ = *(uint32_t*)(msg + ETH_MAC + 4);//MAC address
//    *tempcmd++ = *(uint32_t*)(msg + ETH_MAC);//MAC address
    *tempcmd++ = *(uint32_t*)(msg + KERNELADDR);//U32 系统版本 v4.19.35.
    bufout[16] = 0x1D;
    ret = 17;

    return ret;
}

int Version::set(uint8_t* bufin, uint8_t* bufout)
{
    int ret = 0;
    uint32_t *tempcmd = (uint32_t*)msg;
    uint32_t *tempdata = (uint32_t*)bufin + 1;

    std::cout << "set surperx4 version" << std::endl;

    *(uint32_t*)(msg + XAG_AREA) = *tempdata++;    //地区码
    *(uint32_t*)(msg + XAG_HAL) = *tempdata++;     //硬件版本
    *(uint32_t*)(msg + XAG_TYPE) = *tempdata++;     //设备类型&型号
    *(uint32_t*)(msg + ETH_MAC + 4) = *tempdata++;     //MAC地址
    *(uint32_t*)(msg + ETH_MAC) = *tempdata++;
    memcpy(msg + XAG_SN, bufin + 24, 12);
//    std::cout << msg + XAG_SN << std::endl;
//    *(uint32_t*)(msg + GET_SN) = *tempdata++; //sn
//    *(uint32_t*)(msg + GET_SN + 4) = *tempdata++;
//    *(uint32_t*)(msg + GET_SN + 8) = *tempdata++;
    *(uint32_t*)(msg + XAG_VER) = 0x06000001;      //固件版本 v6.0.0.1
    *(uint32_t*)(msg + KERNELADDR) = KERNEL_VERSION;

//    printf("area = %x\n", *(uint32_t*)(msg + GET_AREA));
//    printf("hal ver = %x\n", *(uint32_t*)(msg + GET_HAL));
//    printf("MAC address = %04x %04x\n", *(uint32_t*)(msg + ETH_MAC), *(uint32_t*)(msg + ETH_MAC + 6));
    file2dd(tempcmd);

    uint32_t *retcmd = (uint32_t*)bufout;//return message
    *retcmd++ = 0x010b0224;//add head
    bufout[4] = 0x01;
    bufout[5] = 0x1D;
    ret = 6;

    return ret;
}

