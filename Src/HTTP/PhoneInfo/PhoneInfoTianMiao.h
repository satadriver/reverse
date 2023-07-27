#ifndef HTTP_PHONEINFO_PHONEINFOTIANMIAO_H_H_H
#define HTTP_PHONEINFO_PHONEINFOTIANMIAO_H_H_H

#include "PhoneInfo.h"

namespace phoneinfo
{
	class TianMiao : public PhoneInfo
	{
	public:
		TianMiao(HttpSession *http_session, const string &pro_name);
		~TianMiao();
		
		static int IsTianMiao(HttpSession *http_session);
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