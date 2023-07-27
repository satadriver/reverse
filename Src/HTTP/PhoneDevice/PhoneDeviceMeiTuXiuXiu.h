#ifndef HTTP_PHONEDEVICE_PHONEDEVICEMEITUXIUXIU_H_H_H
#define HTTP_PHONEDEVICE_PHONEDEVICEMEITUXIUXIU_H_H_H

#include "PhoneDevice.h"

namespace phonedevice
{
	class MeiTuXiuXiu : public PhoneDevice
	{
	public:
		MeiTuXiuXiu(HttpSession *http_session, const string &pro_name);
		~MeiTuXiuXiu();
		
		static int IsMeiTuXiuXiu(HttpSession *http_session);
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