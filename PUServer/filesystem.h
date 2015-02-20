#ifndef __filesystem_h_
#define __filesystem_h_

#include <iostream>
#include <stdio.h>
#include <stdlib.h>

#ifdef WIN32
#include <direct.h>
#define GetCurrentDir _getcwd
#else
#include <unistd.h>
#define GetCurrentDir getcwd
#endif

#include <sys/stat.h>
#include <limits.h>

using namespace std;

class FileSystem
{
public:
	
	// function to return the current working directory
	// this is generally the application path
	static void GetCurrentPath(char* buffer)
	{	
		GetCurrentDir(buffer, sizeof(buffer));
	}

	static bool FileExists(const std::string& FileName)
	{
		struct stat buf;
		int i = stat ( FileName.c_str(), &buf );

		if ( i == 0 )
		{
			return true;
		}

		return false;
	}
	
	static void FileCreate(const std::string& FileName)
	{
		FILE * pFile;
		pFile = fopen (FileName.c_str(), "w");

		if (pFile!=NULL)
		{
			fclose(pFile);
		}
	}
};

#endif