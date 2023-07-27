
#include "StdAfx.h"

#include "DeviceInfo.h"

DeviceInfo::DeviceInfo()
{

}

DeviceInfo::~DeviceInfo()
{

}

string DeviceInfo::GetOS(const string &model,const string &version)
{
	string tmp = ms_->ToLower(model);
	
	if ( (tmp.find("nokia") != string::npos ) || ( tmp.find("lumia") != string::npos))
	{
		return "Windows Phone";
	}
	
	if ( tmp.find("iphone") != string::npos)
	{
		return "iOS";
	}
	
	if (version.size() > 1)
	{
		if ( ('2' <= version[0]) && (version[0] <= '5'))
		{
			return "Android";
		}
		
		if ( ('6' <= version[0]) && (version[0] <= '9'))
		{
			return "iOS";
		}
	}
	return "";
}