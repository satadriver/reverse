
#include "StdAfx.h"
#include "ComputerDevice.h"

#include "ComputerDeviceUserAgent.h"
#include "ComputerDevice360AnQuanWeiShi.h"

computerdevice::ComputerDevice::ComputerDevice(HttpSession *http_session, const string &pro_name) 
: HttpPro(http_session,pro_name)
{
	
}

computerdevice::ComputerDevice::~ComputerDevice()
{
	
}

int computerdevice::ComputerDevice::ProcessComputerDevice(HttpSession *http_session ,const char *packet)
{
	int action = -1;

	int n = 0;
	try
	{
		/*
	action = UserAgent::IsUserAgent(http_session);
	if (action)
	{
		UserAgent ua(http_session,"COMPUTER_DEVICE");
		return ua.Process(packet,action);
	}
*/	
		action = AnQuanWeiShi360::IsAnQuanWeiShi360(http_session);
		if (action)
		{
			n = 2;
			AnQuanWeiShi360 aqws360(http_session,"COMPUTER_DEVICE");
			return aqws360.Process(packet,action);
		}
	}
	catch(...)
	{
		WriteLog("dce2","ProcessComputerDevice Err : %d",n);
	}


	return 0;
}