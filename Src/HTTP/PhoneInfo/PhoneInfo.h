
#ifndef HTTP_PHONEINFO_PHONEINFO_H_H_H
#define HTTP_PHONEINFO_PHONEINFO_H_H_H

#include "../HttpPro.h"

namespace phoneinfo
{
	class PhoneInfo : public HttpPro
	{
	public:
		PhoneInfo(HttpSession *http_session, const string &pro_name);
		virtual ~PhoneInfo();
		
		static int ProcessPhoneInfo(HttpSession *http_session ,const char *packet);
		
	protected:
		string GetOutputText();
		string app_;
		string imei_;
		string imsi_;		
	};
}
#endif