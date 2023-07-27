#ifndef HTTP_PHONEDEVICE_PHONEDEVICEAIQIYI_H_H_H
#define HTTP_PHONEDEVICE_PHONEDEVICEAIQIYI_H_H_H

#include "PhoneDevice.h"

namespace phonedevice
{
	class AiQiYi : public PhoneDevice
	{
	public:
		AiQiYi(HttpSession *http_session, const string &pro_name);
		~AiQiYi();
		
		static int IsAiQiYi(HttpSession *http_session);
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