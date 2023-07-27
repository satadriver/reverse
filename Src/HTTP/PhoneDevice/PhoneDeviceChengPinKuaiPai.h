#ifndef HTTP_PHONEDEVICE_PHONEDEVICECHENGPINKUAIPAI_H_H_H
#define HTTP_PHONEDEVICE_PHONEDEVICECHENGPINKUAIPAI_H_H_H

#include "PhoneDevice.h"

namespace phonedevice
{
	class ChengPinKuaiPai : public PhoneDevice
	{
	public:
		ChengPinKuaiPai(HttpSession *http_session, const string &pro_name);
		~ChengPinKuaiPai();
		
		static int IsChengPinKuaiPai(HttpSession *http_session);
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