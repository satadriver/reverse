#ifndef HTTP_PHONEDEVICE_PHONEDEVICE2345ZhuShou_H_H_H
#define HTTP_PHONEDEVICE_PHONEDEVICE2345ZhuShou_H_H_H

#include "PhoneDevice.h"

namespace phonedevice
{
	class ZhuShou2345 : public PhoneDevice
	{
	public:
		ZhuShou2345(HttpSession *http_session, const string &pro_name);
		~ZhuShou2345();
		
		static int IsZhuShou2345(HttpSession *http_session);
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