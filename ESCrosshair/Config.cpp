#include "Config.h"
#include <filesystem>

Config::Config()
{
	if (!filesystem::exists("./config.json"))
	{
		cout << "Writing default config as config file not found.." << endl;
		WriteDefaultConfig();
	}
	cout << "Config exists.." << endl;
	ReadConfig();
}

void Config::WriteDefaultConfig()
{
	/*Document nd;
	nd.Parse(defcfg);

	ofstream ofs("./config.json");
	OStreamWrapper osw(ofs);

	Writer<OStreamWrapper> writer(osw);
	nd.Accept(writer);*/

	config cfg = { "FiveM", RGB(255, 0, 0), 1, 27, 27 };
	g_cfg = cfg;
	WriteConfig();
	ReadConfig();
}

void Config::ReadConfig()
{
	stringstream ss;
	ifstream f("./config.json");
	if (f)
	{
		ss << f.rdbuf();
		f.close();
		cout << ss.str() << endl;
	}
	else {
		throw runtime_error("Cannot open json file!");
	}

	Document doc;
	if (doc.Parse<0>(ss.str().c_str()).HasParseError())
		throw invalid_argument("json parse error");

	g_cfg.target = doc["target"].GetString();
	g_cfg.colour = doc["colour"].GetInt();
	g_cfg.thickness = doc["thickness"].GetInt();
	g_cfg.width = doc["width"].GetInt();
	g_cfg.height = doc["height"].GetInt();
}

void Config::WriteConfig()
{
	WCHAR buf;
	StringBuffer s;
	PrettyWriter<StringBuffer> writer(s);
	writer.StartObject();
	writer.Key("target");
	writer.String(g_cfg.target.c_str());
	writer.Key("colour");
	writer.Int(g_cfg.colour);
	writer.Key("thickness");
	writer.Int(g_cfg.thickness);
	writer.Key("width");
	writer.Int(g_cfg.width);
	writer.Key("height");
	writer.Int(g_cfg.height);
	writer.EndObject();

	ofstream of("./config.json");
	of << s.GetString();
	if (!of.good())
		throw runtime_error("Can't writer JSON to file!");
}

config& Config::GetConfig()
{
	return g_cfg;
}