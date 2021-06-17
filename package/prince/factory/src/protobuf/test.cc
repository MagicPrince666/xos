/**
 * @file test.cc
 * @author 黄李全 (hlq@ldrobot.com)
 * @brief 
 * @version 0.1
 * @date 2021-06-03
 * @copyright Copyright (c) {2021} 深圳乐动机器人版权所有
 */
#include <iostream>
#include <fstream>
#include <string>

#include "cxx.pb.h"

int test_protobuf(void)
{
    leo::Example example1;
    example1.set_stringval("hello,world");
    example1.set_bytesval("are you ok?");

    leo::Example_EmbeddedMessage *embeddedExample2 = new leo::Example_EmbeddedMessage();

    embeddedExample2->set_int32val(1);
    embeddedExample2->set_stringval("embeddedInfo");
    example1.set_allocated_embeddedexample1(embeddedExample2);

    example1.add_repeatedint32val(2);
    example1.add_repeatedint32val(3);
    example1.add_repeatedstringval("repeated1");
    example1.add_repeatedstringval("repeated2");

    std::string filename = "single_length_delimited_all_example1_val_result";
    std::fstream output(filename, std::ios::out | std::ios::trunc | std::ios::binary);
    if (!example1.SerializeToOstream(&output)) {
        std::cerr << "Failed to write example1." << std::endl;
        exit(-1);
    }

    return 0;
}

