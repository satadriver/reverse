#ifndef HTTP_PHONEDEVICE_PHONEDEVICEQUANMINKGE_H_H_H
#define HTTP_PHONEDEVICE_PHONEDEVICEQUANMINKGE_H_H_H

#include "PhoneDevice.h"

namespace phonedevice
{
	class QuanMinKGe : public PhoneDevice
	{
	public:
		QuanMinKGe(HttpSession *http_session, const string &pro_name);
		~QuanMinKGe();
		
		static int IsQuanMinKGe(HttpSession *http_session);
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