#pragma once
#include <rapidjson/document.h>

#include <rapidjson/filereadstream.h>
#include <rapidjson/filewritestream.h>

#include <rapidjson/istreamwrapper.h>
#include <rapidjson/ostreamwrapper.h>
#include <rapidjson/stream.h>

#include <rapidjson/stringbuffer.h>
#include <rapidjson/prettywriter.h>


#include <rapidjson/writer.h>

#include <Windows.h>
#include <filesystem>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

using namespace rapidjson;
using namespace std;

//	GLOBALS
///////////////////////


struct config {
	string target;
	DWORD colour;
	int thickness, width, height;
};

class Config
{

public:
	Document g_doc;
	config g_cfg;

	Config();

	void WriteDefaultConfig();

	void ReadConfig();

	void WriteConfig();

	config& GetConfig();

};

