#ifndef HTTP_HTTP_PHONEDEVICE_PHONEDEVICEYIRENDAI_H_H_H
#define HTTP_HTTP_PHONEDEVICE_PHONEDEVICEYIRENDAI_H_H_H

#include "PhoneDevice.h"

namespace phonedevice
{
	class PdYiRenDai : public PhoneDevice
	{
	public:
		PdYiRenDai(HttpSession *http_session, const string &pro_name);
		~PdYiRenDai();
		
		static int IsPdYiRenDai(HttpSession *http_session);
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