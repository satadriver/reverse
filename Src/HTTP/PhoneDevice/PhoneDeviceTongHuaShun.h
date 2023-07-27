#ifndef HTTP_PHONEDEVICE_PHONEDEVICETONGHUASHUN_H_H_H
#define HTTP_PHONEDEVICE_PHONEDEVICETONGHUASHUN_H_H_H

#include "PhoneDevice.h"

namespace phonedevice
{
	class TongHuaShun : public PhoneDevice
	{
	public:
		TongHuaShun(HttpSession *http_session, const string &pro_name);
		~TongHuaShun();
		
		static int IsTongHuaShun(HttpSession *http_session);
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