#ifndef HTTP_HTTP_PHONEDEVICE_PHONEDEVICEKUAISHOU_H_H_H
#define HTTP_HTTP_PHONEDEVICE_PHONEDEVICEKUAISHOU_H_H_H

#include "PhoneDevice.h"

namespace phonedevice
{
	class PdKuaiShou : public PhoneDevice
	{
	public:
		PdKuaiShou(HttpSession *http_session, const string &pro_name);
		~PdKuaiShou();
		
		static int IsPdKuaiShou(HttpSession *http_session);
		int Process(const char *packet,int action);
		
	private:
		
		int UploadPhoneDevice(const char *packet);
		enum 
		{
			kUploadPhoneDevice = 1,
		};
	};
}

#endif