#pragma once
#include <Poco/Util/XMLConfiguration.h>
#include <Poco/Util/IniFileConfiguration.h>
#include <Poco/AutoPtr.h>

template <typename T>
class Config
{
public:
	Config (): m_config (new T()) {}
	~Config() {}

	Poco::AutoPtr<T> GetConfigPtr(){ return m_config; }
private:
	Poco::AutoPtr<T>	m_config;
};

extern Config<Poco::Util::XMLConfiguration>	g_GeneralXmlConfig;
extern Config<Poco::Util::IniFileConfiguration> g_GeneralIniConfig;