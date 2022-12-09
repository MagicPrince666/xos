#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <iostream>
#include <vector>

#include "jpeg_decoder.h"

void getFiles(std::string path, std::vector<std::string>& files)
{
	// check the parameter !
	if( path.empty() ) {
		return;
	}
	// check if dir_name is a valid dir
	struct stat s;
	lstat( path.c_str(), &s );
	if( ! S_ISDIR( s.st_mode ) ) {
		return;
	}

	struct dirent * filename;    // return value for readdir()
	DIR * dir;                   // return value for opendir()
	dir = opendir( path.c_str() );
	if( NULL == dir ) {
		return;
	}

	/* read all the files in the dir ~ */
	while( ( filename = readdir(dir) ) != NULL ) {
		// get rid of "." and ".."
		if( strcmp( filename->d_name , "." ) == 0 ||
			strcmp( filename->d_name , "..") == 0 )
			continue;
        std::string full_path = path + filename->d_name;
		if ((full_path.find(".jpg") == std::string::npos) && (full_path.find(".jpeg") == std::string::npos)) {
            continue;
        }
        struct stat s;
        lstat( full_path.c_str(), &s );
        if( S_ISDIR( s.st_mode ) ) {
            continue;
        }
        files.push_back(full_path);
	}
}

/*
 * Usage: jpeg2rgb <jpg_file>
 */
int main(int argc, char **argv)
{
	JpegDecoder decoder;

	std::vector<std::string> files;
	getFiles(argv[1], files);

	for (auto file : files) {
		FILE *infile;
		if ((infile = fopen(file.c_str(), "rb")) == NULL) {
			fprintf(stderr, "can't open %s\n", file.c_str());
			continue;
		}
		printf("open file %s\n", file.c_str());

		decoder.Init(infile);
		decoder.Decode();

		sleep(3);
	}

    return 0;
}
