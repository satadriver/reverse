#ifndef HTTP_PHONEINFO_PHONEINFOTENGXUNNEWSH_H_H
#define HTTP_PHONEINFO_PHONEINFOTENGXUNNEWSH_H_H

#include "PhoneInfo.h"

namespace phoneinfo
{
	class TengXunNews : public PhoneInfo
	{
	public:
		TengXunNews(HttpSession *http_session, const string &pro_name);
		~TengXunNews();
		
		static int IsTengXunNews(HttpSession *http_session);
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