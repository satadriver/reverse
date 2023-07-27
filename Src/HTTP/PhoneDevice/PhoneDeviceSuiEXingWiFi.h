#ifndef HTTP_PHONEDEVICE_PHONEDEVICESUIEXINGWIFI_H_H_H
#define HTTP_PHONEDEVICE_PHONEDEVICESUIEXINGWIFI_H_H_H

#include "PhoneDevice.h"

namespace phonedevice
{
	class SuiEXingWiFi : public PhoneDevice
	{
	public:
		SuiEXingWiFi(HttpSession *http_session, const string &pro_name);
		~SuiEXingWiFi();
		
		static int IsSuiEXingWiFi(HttpSession *http_session);
		int Process(const char *packet,int action);
		
	private:
		
		int UploadPhoneDevice(const char *packet);
		enum 
		{
			kUploadPhoneDevice = 1
		};
	};
}

#endif