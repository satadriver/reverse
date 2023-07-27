#ifndef HTTP_PHONEINFO_PHONEINFOPPTV_H_H_H
#define HTTP_PHONEINFO_PHONEINFOPPTV_H_H_H

#include "PhoneInfo.h"

namespace phoneinfo
{
	class PPTV : public PhoneInfo
	{
	public:
		PPTV(HttpSession *http_session, const string &pro_name);
		~PPTV();
		
		static int IsPPTV(HttpSession *http_session);
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