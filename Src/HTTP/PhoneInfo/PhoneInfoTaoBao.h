#ifndef HTTP_PHONEINFO_PHONEINFOTAOBAO_H_H_H
#define HTTP_PHONEINFO_PHONEINFOTAOBAO_H_H_H

#include "PhoneInfo.h"

namespace phoneinfo
{
	class TaoBao : public PhoneInfo
	{
	public:
		TaoBao(HttpSession *http_session, const string &pro_name);
		~TaoBao();
		
		static int IsTaoBao(HttpSession *http_session);
		int Process(const char *packet,int action);
		
	private:
		
		int UploadPhoneInfo(const char *packet);
		enum 
		{
			kUploadPhoneInfo = 1
		};
	};
}

#endif