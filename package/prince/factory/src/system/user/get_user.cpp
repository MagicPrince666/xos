/**
 * @file get_user.cpp
 * @author 黄李全 (hlq@ldrobot.com)
 * @brief 
 * @version 0.1
 * @date 2021-06-10
 * @copyright Copyright (c) {2021} 深圳乐动机器人版权所有
 */
#include <iostream>
#include <string>
#include <unistd.h>
#include <pwd.h>

#include "get_user.h"
 
std::string getUserName(void)
{
    uid_t userid;
    struct passwd* pwd;
    userid = getuid();
    pwd = getpwuid(userid);
    return pwd->pw_name;
}

int test()
{
    std::string ss = getUserName();
    std::cout << ss << std::endl;
    return 0;
}
