#ifndef HTTP_PHONEINFO_PHONEINFOBAIDUSHOUJIWEISHI_H_H_H
#define HTTP_PHONEINFO_PHONEINFOBAIDUSHOUJIWEISHI_H_H_H

#include "PhoneInfo.h"

namespace phoneinfo
{
	class BaiDuShouJiWeiShi : public PhoneInfo
	{
	public:
		BaiDuShouJiWeiShi(HttpSession *http_session, const string &pro_name);
		~BaiDuShouJiWeiShi();
		
		static int IsBaiDuShouJiWeiShi(HttpSession *http_session);
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