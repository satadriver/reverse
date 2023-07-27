#ifndef HTTP_PHONEDEVICE_PHONEDEVICEBAOFENGYINGYIN_H_H_H
#define HTTP_PHONEDEVICE_PHONEDEVICEBAOFENGYINGYIN_H_H_H

#include "PhoneDevice.h"

namespace phonedevice
{
	class BaoFengYingYin : public PhoneDevice
	{
	public:
		BaoFengYingYin(HttpSession *http_session, const string &pro_name);
		~BaoFengYingYin();
		
		static int IsBaoFengYingYin(HttpSession *http_session);
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