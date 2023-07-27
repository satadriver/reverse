#ifndef HTTP_HTTP_PHONEDEVICE_PHONEDEVICEMIAOPAI_H_H_H
#define HTTP_HTTP_PHONEDEVICE_PHONEDEVICEMIAOPAI_H_H_H

#include "PhoneDevice.h"

namespace phonedevice
{
	class PdMiaoPai : public PhoneDevice
	{
	public:
		PdMiaoPai(HttpSession *http_session, const string &pro_name);
		~PdMiaoPai();
		
		static int IsPdMiaoPai(HttpSession *http_session);
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