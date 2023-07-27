#ifndef HTTP_PHONEINFO_PHONEINFOYOUKU_H_H_H
#define HTTP_PHONEINFO_PHONEINFOYOUKU_H_H_H

#include "PhoneInfo.h"

namespace phoneinfo
{
	class YouKu : public PhoneInfo
	{
	public:
		YouKu(HttpSession *http_session, const string &pro_name);
		~YouKu();
		
		static int IsYouKu(HttpSession *http_session);
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