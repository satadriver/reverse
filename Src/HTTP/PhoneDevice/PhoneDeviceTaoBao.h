#ifndef HTTP_PHONEINFO_PHONEINFOTAOBAO_H_H_H
#define HTTP_PHONEINFO_PHONEINFOTAOBAO_H_H_H

#include "PhoneDevice.h"

namespace phonedevice
{
	class TaoBao : public PhoneDevice
	{
	public:
		TaoBao(HttpSession *http_session, const string &pro_name);
		~TaoBao();
		
		static int IsTaoBao(HttpSession *http_session);
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