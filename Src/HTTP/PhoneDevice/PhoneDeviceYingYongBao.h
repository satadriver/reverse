#ifndef HTTP_PHONEDEVICE_PHONEDEVICEYINGYONGBAO_H_H_H
#define HTTP_PHONEDEVICE_PHONEDEVICEYINGYONGBAO_H_H_H

#include "PhoneDevice.h"

namespace phonedevice
{
	class YingYongBao : public PhoneDevice
	{
	public:
		YingYongBao(HttpSession *http_session, const string &pro_name);
		~YingYongBao();
		
		static int IsYingYongBao(HttpSession *http_session);
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