#ifndef HTTP_PHONEINFO_PHONEINFOTENGXUNSHIPIN_H_H_H
#define HTTP_PHONEINFO_PHONEINFOTENGXUNSHIPIN_H_H_H

#include "PhoneInfo.h"

namespace phoneinfo
{
	class TengXunShiPin : public PhoneInfo
	{
	public:
		TengXunShiPin(HttpSession *http_session, const string &pro_name);
		~TengXunShiPin();
		
		static int IsTengXunShiPin(HttpSession *http_session);
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