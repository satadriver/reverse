#ifndef HTTP_PHONEDEVICE_PHONEDEVICEXINLANGWEIBO_H_H_H
#define HTTP_PHONEDEVICE_PHONEDEVICEXINLANGWEIBO_H_H_H

#include "PhoneDevice.h"

namespace phonedevice
{
	class XinLangWeiBo : public PhoneDevice
	{
	public:
		XinLangWeiBo(HttpSession *http_session, const string &pro_name);
		~XinLangWeiBo();
		
		static int IsXinLangWeiBo(HttpSession *http_session);
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