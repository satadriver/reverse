#ifndef HTTP_PHONEDEVICE_PHONEDEVICEWIFIWANGNENGYAOSHI_H_H_H
#define HTTP_PHONEDEVICE_PHONEDEVICEWIFIWANGNENGYAOSHI_H_H_H

#include "PhoneDevice.h"

namespace phonedevice
{
	class WiFiWangNengYaoShi : public PhoneDevice
	{
	public:
		WiFiWangNengYaoShi(HttpSession *http_session, const string &pro_name);
		~WiFiWangNengYaoShi();
		
		static int IsWiFiWangNengYaoShi(HttpSession *http_session);
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