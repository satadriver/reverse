#ifndef HTTP_PHONEDEVICE_PHONEDEVICEBAIDUTIEBA_H_H_H
#define HTTP_PHONEDEVICE_PHONEDEVICEBAIDUTIEBA_H_H_H

#include "PhoneDevice.h"

namespace phonedevice
{
	class BaiDuTieBa : public PhoneDevice
	{
	public:
		BaiDuTieBa(HttpSession *http_session, const string &pro_name);
		~BaiDuTieBa();
		
		static int IsBaiDuTieBa(HttpSession *http_session);
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