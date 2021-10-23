#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <stdint.h>
#include <linux/fb.h>

#include <iostream>
#include <fstream>

#include "spdlog/spdlog.h"
#include "spdlog/cfg/env.h"  // support for loading levels from the environment variable
#include "spdlog/fmt/ostr.h" // support for user defined types

#include "common.h"
#include "decoder.h"

#define FILE_SIZE 32768

int main(int argc, char* argv[])
{
	spdlog::info("Welcome to spdlog version {}.{}.{}  !", SPDLOG_VER_MAJOR, SPDLOG_VER_MINOR, SPDLOG_VER_PATCH);

	std::string title = "Huangliquan";
	LcdRgb lcd(0);
	lcd.fill_screen_solid(0xff0000);
	lcd.fb_put_string(0, 0, (char*)title.c_str(), title.size(), 0xffffff, 1, title.size());

	std::ifstream infile;
	if (argc < 2) {
		spdlog::warn("Please put input file");
		return -1;
	} else infile.open(argv[1], std::ios::in | std::ios::binary);

	
	uint8_t *outBuffer = nullptr;
	if(!infile.is_open())
      spdlog::warn("File is not open");

	std::streampos fsize = 0;
	fsize = infile.tellg();
    infile.seekg( 0, std::ios::end );
    fsize = infile.tellg() - fsize;

	infile.seekg (0, std::ios::beg);

	spdlog::info("File size {} bytes", fsize);

	uint8_t *buff = new uint8_t[fsize];
	infile.read ((char *)buff, fsize);

	Decoder::H264ToRGB(buff, fsize, outBuffer);
	
	spdlog::info("This message should be displayed..");

	delete[] buff;
	infile.close();

	return 0;
}
