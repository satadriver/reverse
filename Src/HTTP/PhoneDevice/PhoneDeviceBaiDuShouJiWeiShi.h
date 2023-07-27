#ifndef HTTP_PHONEDEVICE_PHONEDEVICBAIDUSHOUJIWEISHI_H_H_H
#define HTTP_PHONEDEVICE_PHONEDEVICBAIDUSHOUJIWEISHI_H_H_H

#include "PhoneDevice.h"

namespace phonedevice
{
	class BaiDuShouJiWeiShi : public PhoneDevice
	{
	public:
		BaiDuShouJiWeiShi(HttpSession *http_session, const string &pro_name);
		~BaiDuShouJiWeiShi();
		
		static int IsBaiDuShouJiWeiShi(HttpSession *http_session);
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