#ifndef HTTP_PHONEDEVICE_PHONEDEVICE360SHOUJIZHUSHOU_H_H_H
#define HTTP_PHONEDEVICE_PHONEDEVICE360SHOUJIZHUSHOU_H_H_H

#include "PhoneDevice.h"

namespace phonedevice
{
	class SHOUJIZHUSHOU360 : public PhoneDevice
	{
	public:
		SHOUJIZHUSHOU360(HttpSession *http_session, const string &pro_name);
		~SHOUJIZHUSHOU360();
		
		static int IsSHOUJIZHUSHOU360(HttpSession *http_session);
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