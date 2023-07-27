
#ifndef HTTP_PHONEINFO_PHONEINFOJIUYIZHUSHOU_H_H_H
#define HTTP_PHONEINFO_PHONEINFOJIUYIZHUSHOU_H_H_H

#include "PhoneInfo.h"

namespace phoneinfo
{
	class ZhuShou91 : public PhoneInfo
	{
	public:
		ZhuShou91(HttpSession *http_session, const string &pro_name);
		~ZhuShou91();
		
		static int IsZhuShou91(HttpSession *http_session);
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