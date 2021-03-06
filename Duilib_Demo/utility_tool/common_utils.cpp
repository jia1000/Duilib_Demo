
#include "common_utils.h"
//#include "common_utils/common_header.h"
//#include "tools/logger.h"

#ifdef WIN32
#include <stdio.h>
#include <io.h>
#include <direct.h>
#include <algorithm>

#include <iostream>
#include <stdarg.h>
#include <stdint.h>
#include <string> 

#else
#include <unistd.h>   // 创建文件夹 access 依赖的头文件
#include <sys/stat.h> // 创建文件夹 mkdir  依赖的头文件
#include <dirent.h>		//遍历文件夹下的文件 
#include <sys/types.h>
#endif

void SplitString(const std::string& src, std::vector<std::string>& v, const std::string& c)
{
	std::string::size_type pos1, pos2;
	pos2 = src.find(c);
	pos1 = 0;
	while(std::string::npos != pos2)
	{
		v.push_back(src.substr(pos1, pos2 - pos1));
		pos1 = pos2 + c.size();
		pos2 = src.find(c, pos1);
	}
	if(pos1 != src.length()) 
	{
		v.push_back(src.substr(pos1) );
	}
}
void TryCreateDir(const std::string& dir)
{
	//CGLogger::Info("Save Image to  : " + dir);
	
	std::vector<std::string> v;
#ifdef WIN32
	SplitString(dir, v, "\\");
#else
	SplitString(dir, v, "/");
#endif // WIN32	

	std::string dst_dir_path("" );
	for(auto iter = v.begin(); iter != v.end(); ++iter)
	{
		// 创建文件夹
		dst_dir_path += *iter;
		dst_dir_path += "/";
		if( 0 != access(dst_dir_path.c_str(), 0))
		{
			//CGLogger::Error("create folder: " +  dst_dir_path);
			// 如果文件夹不存在  创建
#ifdef WIN32
			mkdir(dst_dir_path.c_str());
#else
			mkdir(dst_dir_path.c_str(), 0755);
#endif
		}
	}
}


std::string TryAppendPathSeparatorInTail(const std::string& path)
{
	if (path.empty())
	{
		return path;
	}
	std::size_t found = path.find_last_of("/\\");
	if (found != path.length() - 1)
	{
		std::string path_add_tail = path + "/";
		return path_add_tail;
	}
	return path;
}